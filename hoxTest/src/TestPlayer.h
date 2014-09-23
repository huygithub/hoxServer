//
// C++ Interface: TestPlayer
//
// Description:  This is the Test Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

#ifndef __INCLUDED_TEST_PLAYER_H__
#define __INCLUDED_TEST_PLAYER_H__

#include "Player.h"

/******************************************************************
 * TestPlayer
 */

class TestPlayer : public Player
{
public:
    TestPlayer( const hoxPlayerType type,
                const std::string&  id,
                const std::string&  password )
            : Player( id, password )
            , m_type( type )
            , m_gameStatus( hoxGAME_STATUS_UNKNOWN )
            , m_moveIndex( 0 )
            {}

    hoxPlayerType getType() const { return m_type; }

    void setupMoves( const hoxStringVector& moves ) { _moves = moves; }

    void setFriendId( const std::string& friendId ) { m_friendId = friendId; }
    void SendNextMove();
    void Handle_MOVE( const std::string& sInContent );
    void Handle_DRAW( const std::string& sInContent );
    bool Handle_E_END( const std::string& sInContent );

protected:
    virtual void eventLoop() {}

private:
    int _getTimeBetweenMoves() const;

protected:
    const hoxPlayerType   m_type;
    std::string           m_friendId;
    hoxGameStatus         m_gameStatus;
    size_t                m_moveIndex;

    hoxStringVector       _moves;
};

typedef boost::shared_ptr<TestPlayer> TestPlayer_SPtr;

/******************************************************************
 * Inviter
 */

class Inviter : public TestPlayer
{
public:
    Inviter( const std::string& id,
             const std::string& password )
            : TestPlayer( hoxPLAYER_TYPE_INVITER, id, password ) {}

protected:
    virtual void eventLoop();

private:
    void _Handle_LOGIN( const std::string& sInContent );
    void _Handle_I_PLAYERS( const std::string& sInContent );
    void _Handle_E_JOIN( const std::string& sInContent );

    bool _searchAndInviteFriend( const std::string& playerId );
};

/******************************************************************
 * Invitee
 */

class Invitee : public TestPlayer
{
public:
    Invitee( const std::string& id,
             const std::string& password )
            : TestPlayer( hoxPLAYER_TYPE_INVITEE, id, password ) {}

protected:
    virtual void eventLoop();

private:
    void _Handle_INVITE( const std::string& sTableId,
                         const std::string& sInContent );
    void _Handle_I_TABLE( const std::string& sInContent );
};

#endif /* __INCLUDED_TEST_PLAYER_H__ */
