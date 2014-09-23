//
// C++ Implementation: AIPlayer
//
// Description: The AI Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created: 04/18/2009
//

#include "AIPlayer.h"
#include "AIEngineLib.h"
#include "hoxLog.h"
#include "hoxDebug.h"
#include <dlfcn.h>

//-----------------------------------------------------------------------------
//
//                             AIPlayer
//
//-----------------------------------------------------------------------------

AIPlayer::AIPlayer( const std::string& id,
                    const std::string& password,
                    const std::string& role  /* = "Red" */ )
        : Player( id, password )
        , _aiLib( NULL )
        , _myRole( role )
        , _moveNumber( 0 )
        , _sessionStart( 0 )
        , _sessionExpiry( 2 * 3600 ) // NOTE: 2-hour session.
{
    _sessionStart = st_time();
}

AIPlayer::~AIPlayer()
{
    if ( _aiLib )
    {
        _engine.reset();
        ::dlclose( _aiLib );
        _aiLib = NULL;
    }
}

void
AIPlayer::eventLoop()
{
    bool bSessionExpired = false;
    while ( ! bSessionExpired )
    {
        _moveNumber = 0;
        _playOneGame( bSessionExpired );
    }
}

void
AIPlayer::_playOneGame( bool& bSessionExpired )
{
    hoxTableInfo tableInfo;
    this->openNewTable( tableInfo, _myRole );

    MoveList moves;  /* just an empty list */
    if ( _engine.get() ) _engine->initGame("" /* fen */, moves );

    const int nTimeout         = (5 * 60); // NOTE: 5-minute timeout.
    bool      bKeepAliveNeeded = false;

    while ( ! (bSessionExpired = _isSessionExpired()) )
    {
        hoxCommand inCommand;  // Incoming command from the server.
        try
        {
            if ( bKeepAliveNeeded )
            {
                hoxLog(LOG_DEBUG, "%s: (%s) Sending Keep-Alive...", __FUNCTION__, m_id.c_str());
                this->sendKeepAlive();
                bKeepAliveNeeded = false;
            }
            this->readIncomingCommand( inCommand, nTimeout );
        }
        catch ( const TimeoutException& ex )
        {
            hoxLog(LOG_DEBUG, "%s: (%s) Timeout reading incoming command. (%s).",
                __FUNCTION__, m_id.c_str(), ex.what());
            bKeepAliveNeeded = true;
            continue;
        }

        const std::string sInType    = inCommand.m_type;
        const std::string sInContent = inCommand["content"];

        hoxLog(LOG_DEBUG, "%s: (%s) Received command [%s: %s].", __FUNCTION__,
            m_id.c_str(), inCommand.m_type.c_str(), sInContent.c_str());

        if      ( sInType == "E_JOIN" ) _handle_E_JOIN( sInContent );
        else if ( sInType == "MOVE" )   _handle_MOVE( sInContent );
        else if ( sInType == "DRAW" )   _handle_DRAW( sInContent );
        else if ( sInType == "E_END" )
        {
            if ( _handle_E_END( sInContent ) )  // my game ended?
            {
                break;
            }
        }
    } // while ( ... )

    st_sleep( 10 /* seconds */ ); // Take a break before playing again.
    this->leaveCurrentTable();
}

/**
 * AI HaQiKiD thread.
 */
bool
AIPlayer::loadAIEngine( const std::string& aiName,
                        int                searchDepth /* = 0 */ )
{
    /* References:
     * ----------
     *    + Article: "Dynamic Class Loading for C++ on Linux"
     *         http://www.linuxjournal.com/article/3687
     */

    hoxLog(LOG_DEBUG, "%s: ENTER. AI-Name = [%s].", __FUNCTION__, aiName.c_str());

    hoxCHECK_MSG( _aiLib == NULL, false, "AI Engine had already been loaded" );

    const std::string sPluginPath = "../plugins";
#ifdef __APPLE__
    const char* ext = ".dylib";
#else
    const char* ext = ".so";
#endif
    const std::string sPluginFile = sPluginPath + "/" + aiName + ext;
    _aiLib = ::dlopen( sPluginFile.c_str(), RTLD_NOW );
    if ( ! _aiLib )
    {
        hoxLog(LOG_ERROR, "%s: Failed to load AI Plugin [%s]. (%s).",
            __FUNCTION__, sPluginFile.c_str(), ::dlerror());
        return false;
    }

    const char* szFuncName = "CreateAIEngineLib";
    PICreateAIEngineLibFunc pfnCreate =
        (PICreateAIEngineLibFunc) ::dlsym( _aiLib, szFuncName );
    if ( ! pfnCreate )
    {
        hoxLog(LOG_ERROR, "%s: Function [%s] not found in [%s]. (%s)",
            __FUNCTION__, szFuncName, sPluginFile.c_str(), ::dlerror());
        ::dlclose( _aiLib );
        return false;
    }

    _engine.reset( pfnCreate() );
    _engine->initEngine( searchDepth );

    hoxLog(LOG_INFO, "%s: Successfully loaded AI Plugin [%s].", __FUNCTION__,
        sPluginFile.c_str());
    return true;  // success
}

void
AIPlayer::onOpponentMove( const std::string& sMove )
{
    if ( _engine.get() ) _engine->onHumanMove( sMove );
}

std::string
AIPlayer::generateNextMove()
{
    std::string sMove;
    if ( _engine.get() )
    {
        ++_moveNumber;
        st_sleep( _getTimeBetweenMoves() /* in seconds */ );
        sMove = _engine->generateMove();
    }

    return sMove;
}

void
AIPlayer::_handle_E_JOIN( const std::string& sInContent )
{
    std::string  tableId, playerId;
    int          nScore;
    hoxColor     color;

    hoxCommand::Parse_InCommand_E_JOIN( sInContent,
                                        tableId, playerId, nScore, color );
    hoxLog(LOG_DEBUG, "%s: (%s) %s (%d) %s.", __FUNCTION__, m_id.c_str(),
        playerId.c_str(), nScore, hoxUtil::ColorToString(color).c_str());

    if ( _myRole == "Red" && color == hoxCOLOR_BLACK )
    {
        const std::string sNextMove = this->generateNextMove();
        hoxLog(LOG_DEBUG, "%s: Generated Move = [%s].", __FUNCTION__, sNextMove.c_str());
        this->sendMove( sNextMove );
    }
}

void
AIPlayer::_handle_MOVE( const std::string& sInContent )
{
    std::string   tableId, playerId, sMove;
    hoxGameStatus gameStatus = hoxGAME_STATUS_UNKNOWN;

    hoxCommand::Parse_InCommand_MOVE( sInContent,
                                      tableId, playerId, sMove, gameStatus );
    hoxLog(LOG_DEBUG, "%s: (%s) Received [MOVE: %s %s].", __FUNCTION__,
        m_id.c_str(), playerId.c_str(), sMove.c_str());

    this->onOpponentMove( sMove );
    if ( gameStatus == hoxGAME_STATUS_IN_PROGRESS )
    {
        const std::string sNextMove = this->generateNextMove();
        hoxLog(LOG_DEBUG, "%s: (%s) Generated next Move = [%s].", __FUNCTION__,
            m_id.c_str(), sNextMove.c_str());
        this->sendMove( sNextMove );
    }
}

void
AIPlayer::_handle_DRAW( const std::string& sInContent )
{
    std::string tableId, playerId;

    hoxCommand::Parse_InCommand_DRAW( sInContent,
                                      tableId, playerId );
    hoxLog(LOG_DEBUG, "%s: (%s) Received [DRAW: %s %s].", __FUNCTION__,
        m_id.c_str(), tableId.c_str(), playerId.c_str());
    st_sleep( 10 /* in seconds */ );
    this->sendDraw( tableId );
}

bool
AIPlayer::_handle_E_END( const std::string& sInContent )
{
    std::string    tableId, sReason;
    hoxGameStatus  gameStatus;

    hoxCommand::Parse_InCommand_E_END( sInContent,
                                        tableId, gameStatus, sReason );
    hoxLog(LOG_DEBUG, "%s: %s %s (%s).", __FUNCTION__,
        tableId.c_str(), hoxUtil::GameStatusToString(gameStatus).c_str(),
        sReason.c_str());

    if ( this->isMyTable( tableId ) )  // my game has ended?
    {
        return true;
    }
    return false;
}

bool
AIPlayer::_isSessionExpired() const
{
    if ( _sessionExpiry == 0 ) return false;  // No expiry at all!

    const time_t sessionLength = st_time() - _sessionStart;
    return ( sessionLength > _sessionExpiry );
}

int
AIPlayer::_getTimeBetweenMoves() const
{
    unsigned nTime = 0;
    if      ( _moveNumber < 3 )  nTime = hoxUtil::generateRandomInRange(2, 5);
    else if ( _moveNumber < 10 ) nTime = hoxUtil::generateRandomInRange(1, 5  /*5*/);
    else if ( _moveNumber < 30 ) nTime = hoxUtil::generateRandomInRange(2, 15 /*20*/);
    else if ( _moveNumber < 50 ) nTime = hoxUtil::generateRandomInRange(3, 20 /*35*/);
    else if ( _moveNumber < 70)  nTime = hoxUtil::generateRandomInRange(1, 30 /*40*/);
    else if ( _moveNumber < 90)  nTime = hoxUtil::generateRandomInRange(3, 20 /*35*/);
    else                         nTime = hoxUtil::generateRandomInRange(1, 10 /*10*/);
    return (int) nTime;
}

/************************* END OF FILE ***************************************/
