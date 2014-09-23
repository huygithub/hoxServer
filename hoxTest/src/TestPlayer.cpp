//
// C++ Implementation: TestPlayer
//
// Description:
//    This is the Test Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

#include "TestPlayer.h"
#include "hoxLog.h"
#include "main.h"

//-----------------------------------------------------------------------------
//
//                                  TestPlayer
//
//-----------------------------------------------------------------------------

void
TestPlayer::SendNextMove()
{
    const size_t nMaxIndex = _moves.size();

    hoxLog(LOG_DEBUG, "%s: (%s) %d/%d", __FUNCTION__, m_id.c_str(), m_moveIndex, nMaxIndex);

    if ( m_moveIndex < nMaxIndex )
    {
        const std::string sMove = _moves[m_moveIndex];
        this->sendMove( sMove );
        m_moveIndex += 2;
    }
    else  // ran out of Moves?
    {
        hoxLog(LOG_DEBUG, "%s: (%s) Request DRAW at table [%s]...",
            __FUNCTION__, m_id.c_str(), m_sTableId.c_str());
        this->sendDraw( m_sTableId );
    }
}

void
TestPlayer::Handle_MOVE( const std::string& sInContent )
{
    std::string   tableId, playerId, sMove;
    hoxGameStatus gameStatus = hoxGAME_STATUS_UNKNOWN;

    hoxCommand::Parse_InCommand_MOVE( sInContent,
                                      tableId, playerId, sMove, gameStatus );
    hoxLog(LOG_DEBUG, "%s: (%s) Received [MOVE: %s %s].", __FUNCTION__,
        m_id.c_str(), playerId.c_str(), sMove.c_str());

    if ( gameStatus == hoxGAME_STATUS_IN_PROGRESS )
    {
        st_sleep( _getTimeBetweenMoves() /* in seconds */ );
        this->SendNextMove();
    }
}

void
TestPlayer::Handle_DRAW( const std::string& sInContent )
{
    std::string tableId, playerId;

    hoxCommand::Parse_InCommand_DRAW( sInContent,
                                      tableId, playerId );
    hoxLog(LOG_DEBUG, "%s: (%s) Received [DRAW: %s %s].", __FUNCTION__,
        m_id.c_str(), tableId.c_str(), playerId.c_str());

    this->sendDraw( tableId );
}

bool
TestPlayer::Handle_E_END( const std::string& sInContent )
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

int
TestPlayer::_getTimeBetweenMoves() const
{
    if ( g_config.timeBetweenMoves > 0 ) // Fixed time is enabled?
    {
        return g_config.timeBetweenMoves;
    }

    unsigned nTime = 0;
    if      ( m_moveIndex < 10 ) nTime = hoxUtil::generateRandomNumber(5 /*10*/);
    else if ( m_moveIndex < 30 ) nTime = hoxUtil::generateRandomNumber(20 /*30*/);
    else if ( m_moveIndex < 60 ) nTime = hoxUtil::generateRandomNumber(45 /*60*/);
    else if ( m_moveIndex < 90)  nTime = hoxUtil::generateRandomNumber(60 /*90*/);
    else                         nTime = hoxUtil::generateRandomNumber(10 /*15*/);
    return (int) nTime;
}

//-----------------------------------------------------------------------------
//
//                             Inviter
//
//-----------------------------------------------------------------------------

void
Inviter::eventLoop()
{
    for (;;)
    {
        hoxCommand  inCommand;  // Incoming command from the server.
        this->readIncomingCommand( inCommand );

        const std::string sInType    = inCommand.m_type;
        const std::string sInContent = inCommand["content"];

        hoxLog(LOG_DEBUG, "%s: (%s) Received command [%s: %s].", __FUNCTION__,
            m_id.c_str(), inCommand.m_type.c_str(), sInContent.c_str());

        if      ( sInType == "LOGIN" )     _Handle_LOGIN( sInContent );
        else if ( sInType == "I_PLAYERS" ) _Handle_I_PLAYERS( sInContent );
        else if ( sInType == "E_JOIN" )    _Handle_E_JOIN( sInContent );
        else if ( sInType == "MOVE" )   this->Handle_MOVE( sInContent );
        else if ( sInType == "DRAW" )   this->Handle_DRAW( sInContent );
        else if ( sInType == "E_END" )
        {
            if ( this->Handle_E_END( sInContent ) )  // my game ended?
            {
                break;
            }
        }
    } // for ( ... )

    this->leaveCurrentTable();
}

void
Inviter::_Handle_LOGIN( const std::string& sInContent )
{
    std::string playerId;
    int         nScore = 0;

    hoxCommand::Parse_InCommand_LOGIN( sInContent, playerId, nScore );
    hoxLog(LOG_DEBUG, "%s: %s (%d).", __FUNCTION__, playerId.c_str(), nScore);

    (void) _searchAndInviteFriend( playerId );
}

void
Inviter::_Handle_I_PLAYERS( const std::string& sInContent )
{
    hoxStringList players;
    hoxCommand::Parse_InCommand_I_PLAYERS( sInContent, players );

    for ( hoxStringList::const_iterator it = players.begin();
                                        it != players.end(); ++it )
    {
        if ( _searchAndInviteFriend( *it ) )  // Found a friend?
        {
            break;
        }
    }
}

void
Inviter::_Handle_E_JOIN( const std::string& sInContent )
{
    std::string  tableId, playerId;
    int          nScore;
    hoxColor     color;

    hoxCommand::Parse_InCommand_E_JOIN( sInContent,
                                        tableId, playerId, nScore, color );
    hoxLog(LOG_DEBUG, "%s: (%s) %s (%d) %s.", __FUNCTION__,
        m_id.c_str(), playerId.c_str(), nScore, hoxUtil::ColorToString(color).c_str());

    if ( playerId == m_friendId && color == hoxCOLOR_BLACK )
    {
        m_gameStatus = hoxGAME_STATUS_READY;
        m_moveIndex = 0;  // Send the 1st Move.
        this->SendNextMove();
    }
}

bool
Inviter::_searchAndInviteFriend( const std::string& playerId )
{
    if ( m_friendId.empty() || playerId != m_friendId )
    {
        return false;
    }

    hoxTableInfo tableInfo;
    this->openNewTable( tableInfo, "Red" );

    hoxLog(LOG_DEBUG, "%s: ... invite [%s] to table [%s].", __FUNCTION__,
        m_friendId.c_str(), tableInfo.id.c_str());
    this->inviteOther( m_friendId );
    return true;
}

//-----------------------------------------------------------------------------
//
//                             Invitee
//
//-----------------------------------------------------------------------------

void
Invitee::eventLoop()
{
    for (;;)
    {
        hoxCommand  inCommand;  // Incoming command from the server.
        this->readIncomingCommand( inCommand );

        const std::string sInType    = inCommand.m_type;
        const std::string sTableId   = inCommand["tid"];
        const std::string sInContent = inCommand["content"];

        hoxLog(LOG_DEBUG, "%s: (%s) Received command [%s: %s].", __FUNCTION__,
            m_id.c_str(), inCommand.m_type.c_str(), sInContent.c_str());

        if      ( sInType == "INVITE" )  _Handle_INVITE( sTableId, sInContent );
        else if ( sInType == "I_TABLE" ) _Handle_I_TABLE( sInContent );
        else if ( sInType == "MOVE" )   this->Handle_MOVE( sInContent );
        else if ( sInType == "DRAW" )   this->Handle_DRAW( sInContent );
        else if ( sInType == "E_END" )
        {
            if ( this->Handle_E_END( sInContent ) ) // my game ended?
            {
                break;
            }
        }
    } // for ( ... )

    // Wait some time for my friend (the Inviter) to setup the 'next' game.
    st_sleep( 5 /* in seconds */ );

    this->leaveCurrentTable();
}

void
Invitee::_Handle_INVITE( const std::string& sTableId,
                         const std::string& sInContent )
{
    std::string  inviterId;
    hoxCommand::Parse_InCommand_INVITE( sInContent, inviterId );
    if ( !sTableId.empty() && inviterId == m_friendId )
    {
        this->setMyTable( sTableId );
        this->joinTable( "Black" );
    }
}

void
Invitee::_Handle_I_TABLE( const std::string& sInContent )
{
    hoxTableInfo  tableInfo;
    hoxTableInfo::String_To_Table( sInContent, tableInfo );

    hoxLog(LOG_DEBUG, "%s: (%s) %s vs. %s", __FUNCTION__,
        m_id.c_str(), tableInfo.redId.c_str(), tableInfo.blackId.c_str());

    if (    tableInfo.redId   == m_friendId
         || tableInfo.blackId == m_id ) // Is this my Table?
    {
        m_gameStatus = hoxGAME_STATUS_READY;
        m_moveIndex = 1;  // Point to "my" 1st Move.
    }
}

/************************* END OF FILE ***************************************/
