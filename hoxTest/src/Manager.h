//
// C++ Interface: Manager
//
// Description:  This is the Manager of the test.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

#ifndef __INCLUDED_MANAGER_H__
#define __INCLUDED_MANAGER_H__

#include "hoxCommon.h"
#include "TestPlayer.h"

/******************************************************************
 * Game
 */

class Game
{
public:
    Game( const int id ) : m_id( id )
                         , m_status( hoxGAME_STATUS_UNKNOWN ) {}

    const int         m_id;

    hoxStringVector   m_moves;
    TestPlayer_SPtr   m_red;
    TestPlayer_SPtr   m_black;
    hoxGameStatus     m_status;
};

/******************************************************************
 * Manager
 */

class Manager
{
    typedef std::list<Game>  GameList;

public:
    static Manager* instance();

    TestPlayer_SPtr createTestPlayer( const int thread_id, Game*& game );
    void onPlayerDisconnected( TestPlayer_SPtr player, Game* game );

private:
    static Manager* _instance;
    Manager();

    void _loadGames();
    void _parseMovesString( const std::string& sMoves,
                            hoxStringVector&   vMoves ) const;
    void _generatePlayerInfo( const int    thread_id,
                              std::string& pid,
                              std::string& password );
    Game* _selectFreeGame();
    void _assignPlayerToGame( TestPlayer_SPtr player, Game* game ) const;

private:
    GameList        _games;
    hoxStringList   _availableIDs; // Available predefined Test IDs.
    std::string     _groupPassword;
    Game*           _openGame; // Point to the Table that needs a 2nd player.
};

#endif /* __INCLUDED_MANAGER_H__ */
