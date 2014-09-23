//
// C++ Immplementation: Manager
//
// Description:  This is the Manager of the test.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

#include "Manager.h"
#include "main.h"
#include "hoxLog.h"
#include "hoxDebug.h"
#include <fstream>

/******************************************************************
 * Constants
 */

#define GAMES_FILE   "./games.txt"

//-----------------------------------------------------------------------------
//
//                                  Manager
//
//-----------------------------------------------------------------------------

Manager* Manager::_instance = NULL;  /* Singleton variable */

Manager* 
Manager::instance()
{
    if ( _instance == NULL )
    {
        _instance = new Manager;
    }
    return _instance;
}

Manager::Manager()
        : _openGame( NULL )
{
    _loadGames();
    _availableIDs  = g_config.groupTestIds;
    _groupPassword = g_config.groupPassword;
}

TestPlayer_SPtr
Manager::createTestPlayer( const int thread_id,
                           Game*&    game )
{
    TestPlayer_SPtr player;
    hoxPlayerType   playerType = hoxPLAYER_TYPE_INVITER;

    if ( _openGame == NULL ) // No table is open?
    {
        game = _selectFreeGame();
        _openGame = game;
        // ... play RED and will be an Inviter
    }
    else // A table is open and waiting for a BLACK player?
    {
        hoxASSERT_MSG(!_openGame->m_black, "Black Player must NOT be set");
        game = _openGame;
        _openGame = NULL;
        playerType = hoxPLAYER_TYPE_INVITEE; // ... and play BLACK
    }

    if ( ! game )
    {
        hoxLog(LOG_INFO, "%s: [#%d]: No 'free' game available.", __FUNCTION__, thread_id);
        return player;
    }
    
    // Generate a Player based on Role.
    std::string pid, password;

    _generatePlayerInfo( thread_id, pid, password );
    hoxLog(LOG_INFO, "%s: [#%d]: pid=%s, password=%s", __FUNCTION__,
            thread_id, pid.c_str(), password.c_str());

    if ( playerType == hoxPLAYER_TYPE_INVITER ) {
        player.reset( new Inviter( pid, password ) );
    } else {
        player.reset( new Invitee( pid, password ) );
    }
    _assignPlayerToGame( player, game );

    return player;
}

void
Manager::_assignPlayerToGame( TestPlayer_SPtr player,
                              Game*           game ) const
{
    hoxASSERT_MSG(game, "Game must be selected first");

    const std::string pid = player->getId();

    if ( player->getType() == hoxPLAYER_TYPE_INVITER )
    {
        game->m_status = hoxGAME_STATUS_OPEN;
        game->m_red = player;
        hoxLog(LOG_INFO, "%s: Assign [%s] to game [%d].", __FUNCTION__, pid.c_str(), game->m_id);
        hoxASSERT_MSG(!game->m_black, "Black Player must NOT be set");
    }
    else
    {
        game->m_status = hoxGAME_STATUS_READY;
        game->m_black = player;
        hoxASSERT_MSG(game->m_red, "Red Player must be set first");
        const std::string friendId = game->m_red->getId();
        hoxLog(LOG_INFO, "%s: [%s] joins game [%d] with friend [%s].",
            __FUNCTION__, pid.c_str(), game->m_id, friendId.c_str());

        player->setFriendId( friendId );
        game->m_red->setFriendId( pid );
    }

    player->setupMoves( game->m_moves );
}

void
Manager::onPlayerDisconnected( TestPlayer_SPtr player,
                               Game*           game )
{
    if ( ! game )
    {
        hoxLog(LOG_WARN, "%s: No game assigned to [%s]. Do nothing.",
                __FUNCTION__, player->getId().c_str());
        return;
    }

    hoxLog(LOG_INFO, "%s: [%s] left game [%d].", __FUNCTION__,
        player->getId().c_str(), game->m_id);

    if      ( player == game->m_red )   game->m_red.reset();
    else if ( player == game->m_black ) game->m_black.reset();

    player->setFriendId("");

    // Push back IDs to be reused, including those Guest IDs that were
    // generated during runtime.
    _availableIDs.push_back( player->getId() );
    
    if ( !game->m_red && !game->m_black )
    {
        game->m_status = hoxGAME_STATUS_UNKNOWN;
    }
}

Game*
Manager::_selectFreeGame()
{
    std::vector<Game*> freeGames;

    for ( GameList::iterator it = _games.begin(); it != _games.end(); ++it )
    {
        if (    it->m_status == hoxGAME_STATUS_UNKNOWN
             && !it->m_red && !it->m_black )
        {
            freeGames.push_back( &(*it) );
        }
    }

    const size_t nSize = freeGames.size();
    if ( nSize == 0 ) return NULL;

    unsigned randIndex = hoxUtil::generateRandomNumber( nSize );
    --randIndex;   // Adjust to 0-based index
    hoxASSERT_MSG(randIndex < nSize, "Invalid game index");

    return freeGames[randIndex];
}

void
Manager::_generatePlayerInfo( const int    thread_id,
                              std::string& pid,
                              std::string& password )
{
    /* Use one of the test IDs, if available.
     * Otherwise, generate a 'test' ID.
     */
    if ( !_availableIDs.empty() )
    {
        pid = _availableIDs.front();
        _availableIDs.pop_front();
        password = _groupPassword;
    }
    else
    {
        char szTemp[32];
        unsigned randNum = hoxUtil::generateRandomNumber( 9999 );
        snprintf(szTemp, sizeof(szTemp), "Guest#t%04u%d", randNum, thread_id);
        pid = std::string( szTemp );
        password  = "_dummy_password_"; // Not needed for guests.
    }
}

void
Manager::_loadGames()
{
    std::ifstream ifs( GAMES_FILE );
    std::string   sLine;
    size_t        i = 0;
    while ( std::getline(ifs, sLine) )
    {
        if ( sLine.empty() || sLine[0] == '#' ) continue;

        Game game( ++i );
        _parseMovesString( sLine, game.m_moves );
        hoxLog(LOG_DEBUG, "%s: Loading game #%d...", __FUNCTION__, i);
        _games.push_back( game );
    }
    hoxLog(LOG_INFO, "%s: # of loaded games = [%d].", __FUNCTION__, _games.size());
}

void
Manager::_parseMovesString( const std::string& sMoves,
                            hoxStringVector&   vMoves ) const
{
    vMoves.clear();

    hoxTokenizer tok( sMoves, hoxSeparator("/") );

    for ( hoxTokenizer::const_iterator it = tok.begin(); it != tok.end(); ++it )
    {
        vMoves.push_back( *it );
    }
}

/************************* END OF FILE ***************************************/
