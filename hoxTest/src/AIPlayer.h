//
// C++ Interface: AIPlayer
//
// Description: The AI Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created: 04/18/2009
//

#ifndef __INCLUDED_AI_PLAYER_H__
#define __INCLUDED_AI_PLAYER_H__

#include "Player.h"

/* Forward declarations. */
class AIEngineLib;

typedef std::auto_ptr<AIEngineLib> AIEngineLib_APtr;

/**
 * The AI player.
 */
class AIPlayer :  public Player
{
public:
    AIPlayer( const std::string& id,
              const std::string& password,
              const std::string& role = "Red" );
    virtual ~AIPlayer();

    bool loadAIEngine( const std::string& aiName,
                       int                searchDepth = 0 );

protected:
    virtual void eventLoop();

    virtual void        onOpponentMove( const std::string& sMove );
    virtual std::string generateNextMove();

private:
    void _playOneGame( bool& bSessionExpired );

    void _handle_E_JOIN( const std::string& sInContent );
    void _handle_MOVE( const std::string& sInContent );
    void _handle_DRAW( const std::string& sInContent );
    bool _handle_E_END( const std::string& sInContent );

    bool _isSessionExpired() const;
    int _getTimeBetweenMoves() const;

private:
    void*             _aiLib;  // The library handle of AI Engine Plugin.
    AIEngineLib_APtr  _engine;
    const std::string _myRole;
    size_t            _moveNumber;
    time_t            _sessionStart; // Timestamp since the session started.
    time_t            _sessionExpiry; // in seconds. 0 => "never expired"
};

typedef boost::shared_ptr<AIPlayer> AIPlayer_SPtr;

#endif /* __INCLUDED_AI_PLAYER_H__ */
