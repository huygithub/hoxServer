//
// C++ Implementation: hoxTable
//
// Description: The Table
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#include <st.h>
#include "hoxTable.h"
#include "hoxDebug.h"
#include "hoxUtil.h"
#include "hoxLog.h"
#include "hoxReferee.h"
#include "hoxDbClient.h"

// =========================================================================
//                  >>>> Elo Rating System <<<
//
//  References:
//  ----------
//     http://en.wikipedia.org/wiki/Elo_rating_system
//     http://www.chesselo.com
//     http://gobase.org/studying/articles/elo
//
//  Table of Winning Probability:
//  ---------------------------------------------------------
//   Rating Difference | Stronger Player | Weaker Player
//    0                    0.50              0.50
//    25                   0.53	             0.47
//    50                   0.57	             0.43
//    100                  0.64	             0.36
//    150                  0.70              0.30
//    200                  0.76              0.24
//    250                  0.81              0.19
//    300                  0.85              0.15
//    350 (FIDE)           0.89              0.11
//  ----------------------------------------------------------
//
// Calculating K-factor:
// ---------------------
// Use FIDE rules to determine the K-factor:
//
//  (1) K-factor = 25 for players new to the rating list, until they have
//                    completed events with a total of at least 30 games;
//  (2) K-factor = 15 for players with a rating under 2400;
//  (3) K-factor = 10 once the player has reached 2400 and been registered
//                    for at least 30 games. Thereafter it remains permanently
//                    at 10, even if the player' s rating is under 2400 at
//                    a later stage.
//
//
//  Calculating New Rating:
//  -----------------------
//     New Rating = Old Rating + K-factor * (Result - Expected Result)
//
// =========================================================================

struct WinningProbability_t
{
    int    ratingDiff;
    float  strongerPlayer;
    float  weakerPlayer;
};

WinningProbability_t s_winningProbabilities[] =
{
   // Diff | Stronger | Weaker
    { 0,      0.50,   0.50 },
    { 25,     0.53,   0.47 },
    { 50,     0.57,   0.43 },
    { 100,    0.64,	  0.36 },
    { 150,    0.70,   0.30 },
    { 200,    0.76,   0.24 },
    { 250,    0.81,   0.19 },
    { 300,    0.85,   0.15 },
    { 350,    0.89,   0.11 }
};

/**
 * Calculate the K-Factor.
 *
 * @param nOldRating The Old Rating.
 * @param nPlayedGames The number of games that has been played.
 *
 * @return The K-Factor.
 */
int
_Elo_calculate_KFactor( const int nPlayedGames,
                        const int nOldRating )
{
    if      ( nPlayedGames < 30 ) return 25;
    else if ( nOldRating < 2400 ) return 15;
    // FIXME: We ignore the phase:
    //    "... Thereafter it remains permanently at 10,"
    //    "even if the player's rating is under 2400 at a later stage"
    return 10;
}

/**
 * Calculate the new (Elo) Rating Change.
 *
 * @param nRatingDiff The Rating Difference:
 *                    (positive for Stronger, negative for Weaker)
 *
 * @param nOldRating The Old Rating.
 *
 * @param nPlayedGames The number of games that has been played.
 *
 * @param nGameResult The game's score:
 *                    (win=1, draw=0.5, loss=0)
 *
 * @return The New Rating Change.
 */
int
_Elo_calculate_NewRatingChange( const int   nRatingDiff,
                                const int   nOldRating,
                                const int   nPlayedGames,
                                const float fGameResult )
{
    const int nAbsoluteRatingDiff = ( nRatingDiff > 0 ? nRatingDiff
                                                      :(-1 * nRatingDiff) );

    const int nMaxSize = sizeof(s_winningProbabilities) / sizeof (WinningProbability_t);
    float fExpectedResult = 0.50;
    for ( int i = nMaxSize-1; i >= 0; --i )
    {
        if ( nAbsoluteRatingDiff >= s_winningProbabilities[i].ratingDiff )
        {
            fExpectedResult = (   nRatingDiff > 0
                                ? s_winningProbabilities[i].strongerPlayer
                                : s_winningProbabilities[i].weakerPlayer );
            break;
        }
    }

    const int nKFactor = _Elo_calculate_KFactor( nPlayedGames, nOldRating );

    return (int) (nKFactor * (fGameResult - fExpectedResult));
}

// =========================================================================
//
//                        hoxTable
//
// =========================================================================

hoxTable::hoxTable( const std::string& id,
                    const hoxTimeInfo& initialTime )
        : _id( id )
        , _gameGroup( hoxGAME_GROUP_PUBLIC )
        , _gameType( hoxGAME_TYPE_RATED )
        , _initialTime( initialTime )
        , _redTime( initialTime )
        , _blackTime( initialTime )
        , _referee( new hoxReferee() )
        , _status( hoxGAME_STATUS_OPEN )
        , _lastMoveTime( 0 )
        , _nextMoveExpiry( 0 )
        , _isPrevMoveCheck( false )
        , _effectiveMoves( 0 )
        , _redChecks( 0 )
        , _blackChecks( 0 )
{
    hoxLog(LOG_DEBUG, "%s: (%s) ENTER.", __FUNCTION__, _id.c_str());
}

hoxTable::~hoxTable()
{
    hoxLog(LOG_DEBUG, "%s: (%s) ENTER.", __FUNCTION__, _id.c_str());
    delete _referee;
}

void
hoxTable::getObservers( hoxPlayerList& observers ) const
{
    observers.clear();
    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        if ( *it != _redPlayer && *it != _blackPlayer )
        {
            observers.push_back( *it );
        }
    }
}

void
hoxTable::onLeave_FromPlayer( hoxPlayer_SPtr player )
{
    const char* FNAME = "hoxTable::onLeave_FromPlayer";

    hoxCHECK_RET( player, "Player is NULL" );

    /* If the Player is playing, then he loses this game. */

    hoxGameStatus newStatus = hoxGAME_STATUS_UNKNOWN;
    const bool bGameStarted = (_moves.size() >= 2);
    if ( _status == hoxGAME_STATUS_IN_PROGRESS && bGameStarted )
    {
        if      ( player == _redPlayer )   newStatus = hoxGAME_STATUS_BLACK_WIN;
        else if ( player == _blackPlayer ) newStatus = hoxGAME_STATUS_RED_WIN;
    }

    if ( newStatus != hoxGAME_STATUS_UNKNOWN ) // Game ended?
    {
        hoxLog(LOG_DEBUG, "%s: Game ended since [%s] left.", FNAME, player->getId().c_str());
        _onGameEnded( newStatus, "Player left the game" );
    }

    _unassignPlayer( player );
}

void
hoxTable::onMessage_FromPlayer( hoxPlayer_SPtr          player,
                                const hoxResponse_SPtr& pMessage )
{
    const char* FNAME = "hoxTable::onMessage_FromPlayer";

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        if ( *it == player )
        {
            hoxLog( LOG_DEBUG, "%s: Skip [%s] since he is the sender.",
                FNAME, player->getId().c_str() );
            continue;
        }

        (*it)->onNewEvent( pMessage );
    }
}

hoxResult
hoxTable::assignPlayerAs( hoxPlayer_SPtr player,
                          const hoxColor requestColor )
{
    const char* FNAME = "hoxTable::assignPlayerAs";
    hoxCHECK( player, hoxRC_ERR );

    bool bRequestOK =
           ( requestColor == hoxCOLOR_RED   && !_redPlayer )
        || ( requestColor == hoxCOLOR_BLACK && !_blackPlayer )
        || ( requestColor == hoxCOLOR_NONE );
    if ( ! bRequestOK )
    {
        hoxLog(LOG_WARN, "%s: Failed to handle JOIN request from player [%s].",
            FNAME, player->getId().c_str() );
        return hoxRC_ERR;
    }

    _postAll_JoinEvent( player, requestColor );

    /* Update our internal player-list */
    const bool bNewlyAdded = _addPlayer( player, requestColor );

    if ( bNewlyAdded ) // The Player just joined to this Table?
    {
        this->sendInfoToPlayer( player );
    }

    _updateStatus(); // Update the game's status.
    return hoxRC_OK;
}

void
hoxTable::sendInfoToPlayer( hoxPlayer_SPtr player )
{
    hoxResponse_SPtr event = hoxResponse::create_event_I_TABLE( this );
    player->onNewEvent( event );

    if ( ! _moves.empty() ) // Inform about the existing Moves.
    {
        event = hoxResponse::create_event_I_MOVES( this, _moves );
        player->onNewEvent( event );
    }

    if (    _status == hoxGAME_STATUS_RED_WIN
         || _status == hoxGAME_STATUS_BLACK_WIN
         || _status == hoxGAME_STATUS_DRAWN )
    {
        event = hoxResponse::create_event_END( this, _status, "Game ended" );
        player->onNewEvent( event );
    }
}

void
hoxTable::_unassignPlayer( hoxPlayer_SPtr player )
{
    /* Inform other players about this event.
     * NOTE: This should be done BEFORE the player is removed
     *       from the internal player-list.
     */
    _postAll_LeaveEvent( player );

    _removePlayer( player );  // Update our player-list.
    _updateStatus();  // Update the game's status.
}

hoxResult
hoxTable::acceptMove( hoxPlayer_SPtr     player,
                      const std::string& sMove )
{
    const char* FNAME = "hoxTable::acceptMove";

    /* Game-Status Checking: Can the game accept Move? */
    if (   _status != hoxGAME_STATUS_READY 
        && _status != hoxGAME_STATUS_IN_PROGRESS )
    {
        hoxLog(LOG_INFO, "%s: Table is not ready to accept Move.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* Player-Checking: Does the player has the right to move? */

    const hoxColor nextColor = _referee->getNextColor();
    
    if (    ( nextColor == hoxCOLOR_RED && player != _redPlayer )
         || ( nextColor == hoxCOLOR_BLACK && player != _blackPlayer ) )
    {
        hoxLog(LOG_INFO, "%s: It is NOT the player's turn to make a Move.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* Move-Checking: Ask the Referee to validate the Move. */

    hoxMove move = _referee->stringToMove( sMove );
    if ( ! move.isValid() )
    {
        hoxLog(LOG_INFO, "%s: It is not a valid string-representation of Move.", FNAME);
        return hoxRC_NOT_VALID;
    }

    hoxGameStatus gameStatus = hoxGAME_STATUS_UNKNOWN;
    if ( ! _referee->validateMove( move, gameStatus ) )
    {
        hoxLog(LOG_INFO, "%s: Move [%s] is not valid.", FNAME, move.toString().c_str());
        return hoxRC_NOT_VALID;
    }

    /* Perpetual-Check detection. */

    std::string sReason;  // The reason that (if) game ended.

    if (    gameStatus == hoxGAME_STATUS_RED_WIN
         || gameStatus == hoxGAME_STATUS_BLACK_WIN )
    {
        sReason = "Game ended after the last Move";
    }
    else
    {
        _detectLongGameAndPerpetualCheck( nextColor, gameStatus, sReason );
    }
    hoxLog(LOG_DEBUG, "%s: PERPETUAL: %s (R:%d B:%d E:%d)", __FUNCTION__,
        hoxUtil::colorToString(nextColor).c_str(), _redChecks, _blackChecks, _effectiveMoves);

    /* Move is fine. Record the Move and prepare for the next one. */

    _status = gameStatus;
    _moves.push_back( sMove );
    _resetMoveTimers( nextColor );

    /* Inform other players about the new Move */
    _postAll_MoveEvent( player, sMove, gameStatus );

    /* Check for End-Game status. */

    if (    gameStatus == hoxGAME_STATUS_RED_WIN
         || gameStatus == hoxGAME_STATUS_BLACK_WIN
         || gameStatus == hoxGAME_STATUS_DRAWN )
    {
        _onGameEnded( gameStatus, sReason );
    }

    return hoxRC_OK;
}

hoxResult
hoxTable::handleResignRequest( hoxPlayer_SPtr player )
{
    const char* FNAME = "hoxTable::handleResignRequest";

    /* Game-Status Checking: Can the game accept the request? */
    if ( _status != hoxGAME_STATUS_IN_PROGRESS )
    {
        hoxLog(LOG_INFO, "%s: Table is not in progress.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* If the Player is playing, then he loses this game. */

    hoxGameStatus newStatus = hoxGAME_STATUS_UNKNOWN;

    if      ( player == _redPlayer )   newStatus = hoxGAME_STATUS_BLACK_WIN;
    else if ( player == _blackPlayer ) newStatus = hoxGAME_STATUS_RED_WIN;
    else
    {
        hoxLog(LOG_INFO, "%s: The player is not playing.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    hoxLog(LOG_DEBUG, "%s: Game ended since [%s] resigned.", FNAME,
        player->getId().c_str());
    _onGameEnded( newStatus, "Player resigned" );

    return hoxRC_OK;
}

hoxResult
hoxTable::handleDrawRequest( hoxPlayer_SPtr player )
{
    const char* FNAME = "hoxTable::handleDrawRequest";

    /* Game-Status Checking: Can the game accept the request? */
    if ( _status != hoxGAME_STATUS_IN_PROGRESS )
    {
        hoxLog(LOG_INFO, "%s: Table is not in progress.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* Player-Checking: Does the player has the right to request? */

    if (    player != _redPlayer
         && player != _blackPlayer )
    {
        hoxLog(LOG_INFO, "%s: The player is not playing.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* If this is the 1st request, then inform all other players. */
    if ( _drawPlayerId.empty() )
    {
        _drawPlayerId = player->getId();
        hoxLog(LOG_DEBUG, "%s: Sending Draw-Request from player = [%s]...",
            FNAME, _drawPlayerId.c_str());
        _postAll_DrawEvent( player );
    }
    /* If the player is requesting AGAIN, do nothing. */
    else if ( player->getId() == _drawPlayerId )
    {
        hoxLog(LOG_DEBUG, "%s: The player has already requested.", FNAME);
        // Do nothing.
    }
    /* Otherwise, both two players agree to draw. */
    else
    {
        _onGameEnded( hoxGAME_STATUS_DRAWN, "Players agreed to draw" );
    }

    return hoxRC_OK;
}

hoxResult
hoxTable::handleResetRequest( hoxPlayer_SPtr player )
{
    const char* FNAME = "hoxTable::handleResetRequest";

    const bool bGameStarted = (_moves.size() >= 2);
    if ( _status == hoxGAME_STATUS_IN_PROGRESS && bGameStarted )
    {
        hoxLog(LOG_INFO, "%s: Table is currently in progress.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }
    if ( player != _redPlayer && player != _blackPlayer )
    {
        hoxLog(LOG_INFO, "%s: The player is not playing.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }
    
    /* Special case: The RED player gives up the right to move first. */
    if ( player == _redPlayer && _status == hoxGAME_STATUS_READY )
    {
        hoxLog(LOG_INFO, "%s: Switch roles by RESET.", FNAME);
        _redPlayer = _blackPlayer; // Status is "ready", so there must be two players!
        _blackPlayer = player;
        // After reserving the seats (see above), the players first give up the
        // existing roles and then "re-join".
        _postAll_JoinEvent( _redPlayer, hoxCOLOR_NONE );
        _postAll_JoinEvent( _blackPlayer, hoxCOLOR_NONE );
        _postAll_JoinEvent( _redPlayer, hoxCOLOR_RED );
        _postAll_JoinEvent( _blackPlayer, hoxCOLOR_BLACK );
    }
    else if ( _status == hoxGAME_STATUS_READY || _status == hoxGAME_STATUS_OPEN )
    {
        hoxLog(LOG_INFO, "%s: The game is ready. No need to reset.", FNAME);
    }
    else
    {
        hoxLog(LOG_DEBUG, "%s: Reset the Game at [%s] as requested by [%s].", FNAME,
            this->getId().c_str(), player->getId().c_str());
        _onGameReset();
    }

    return hoxRC_OK;
}

hoxResult
hoxTable::handleUpdateRequest( hoxPlayer_SPtr     player,
                               const bool         bRatedGame,
                               const hoxTimeInfo& newInitialTime )
{
    const char* FNAME = "hoxTable::handleUpdateRequest";

    /* Game-Status Checking: Can the game accept the request? */
    if (    _status != hoxGAME_STATUS_OPEN
         && _status != hoxGAME_STATUS_READY )
    {
        hoxLog(LOG_INFO, "%s: Table is already in progress or has ended.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* Player-Checking: Make sure the board Player satifies one of the 
     *                  following conditions:
	 *  (1) He is the RED player, or...
     *  (2) He is the BLACK player and there is no RED player.
	 */

    bool bActionAllowed = (    player == _redPlayer 
		                   || (player == _blackPlayer && !_redPlayer) );
    if ( ! bActionAllowed )
    {
        hoxLog(LOG_INFO, "%s: The player not allowed to change Table Options.", FNAME);
        return hoxRC_NOT_ALLOWED;
    }

    /* Update timers. */

    hoxLog(LOG_DEBUG, "%s: Update Rated-Game = [%d], Timers = [%s] at [%s] as requested by [%s].", FNAME,
        bRatedGame,
        hoxUtil::timeInfoToString(newInitialTime).c_str(),
        this->getId().c_str(), 
        player->getId().c_str());

    _gameType = ( bRatedGame 
                 ? hoxGAME_TYPE_RATED 
                 : hoxGAME_TYPE_NONRATED );    

    _initialTime = newInitialTime;
    _redTime     = _initialTime;
    _blackTime   = _initialTime;

    /* Inform other players about the new Options */
    _postAll_UpdateEvent( player, _gameType, newInitialTime );

    return hoxRC_OK;
}

void
hoxTable::checkTimeoutOnMove()
{
    const char* FNAME = "hoxTable::checkTimeoutOnMove";

    /* Do nothing if the game is not yet started. */
    const bool bGameStarted = (_moves.size() >= 2);
    if (   _status != hoxGAME_STATUS_IN_PROGRESS
        || !bGameStarted )
    {
        return;
    }

    const time_t now = st_time();
    if ( now <= _nextMoveExpiry ) return;

    hoxLog(LOG_DEBUG, "%s: Timeout detected. Seconds passed = [%d].",
        FNAME, (now > _nextMoveExpiry));

    const hoxColor nextColor = _referee->getNextColor();
    const hoxGameStatus newStatus = ( nextColor == hoxCOLOR_RED
                                     ? hoxGAME_STATUS_BLACK_WIN
                                     : hoxGAME_STATUS_RED_WIN );

    hoxLog(LOG_DEBUG, "%s: Game ended due to timeout. [%s] lost.",
        FNAME, hoxUtil::colorToString(nextColor).c_str());
    _onGameEnded( newStatus, "Move timeout" );
}

void
hoxTable::getCurrentTimers( hoxTimeInfo& redTime,
                            hoxTimeInfo& blackTime ) const
{
    const time_t now        = st_time();
    const time_t elapseTime = (_moves.size() > 2 ? (now - _lastMoveTime)
                                                 : 0 );
    const hoxColor nextColor = _referee->getNextColor();
    redTime   = _redTime;
    blackTime = _blackTime;

    hoxTimeInfo* pNextTime = ( nextColor == hoxCOLOR_RED ? &redTime
                                                         : &blackTime );
    /* Determine the current time of the "next" player. */
    pNextTime->nGame -= elapseTime;
    if ( pNextTime->nGame < 0 )
    {
        pNextTime->nFree += pNextTime->nGame;
        if ( pNextTime->nFree < 0 ) pNextTime->nFree = 0;
        pNextTime->nGame = 0;
    }
    pNextTime->nMove -= elapseTime;
    if ( pNextTime->nMove < 0 ) pNextTime->nMove = 0;
}

bool
hoxTable::_addPlayer( hoxPlayer_SPtr player,
                      hoxColor    role )
{
    bool bNewlyAdded = false;

    hoxPlayerList::const_iterator foundIt = 
        std::find( _allPlayers.begin(), _allPlayers.end(), player );

    if ( foundIt == _allPlayers.end() ) // not found?
    {
        _allPlayers.push_back( player );
        bNewlyAdded = true;
    }

    // "Cache" the RED and BLACK players for easy access.
    if ( role == hoxCOLOR_RED )
    {
        _redPlayer = player;
        if ( _blackPlayer == player ) _blackPlayer.reset();
    }
    else if ( role == hoxCOLOR_BLACK )
    {
        _blackPlayer = player;
        if ( _redPlayer == player ) _redPlayer.reset();
    }
    else
    {
        if ( _redPlayer   == player ) _redPlayer.reset();
        if ( _blackPlayer == player ) _blackPlayer.reset();
    }

    return bNewlyAdded;
}

void
hoxTable::_removePlayer( hoxPlayer_SPtr player )
{
    _allPlayers.remove( player );

    // Update our "cache" variables.
    if      ( _redPlayer == player )    _redPlayer.reset();
    else if ( _blackPlayer == player )  _blackPlayer.reset();
}

void
hoxTable::_updateStatus()
{
    /* Start the game if there are a RED and a BLACK players */

    if ( _status == hoxGAME_STATUS_OPEN )
    {
        if ( _redPlayer != NULL && _blackPlayer != NULL )
        {
            _status = hoxGAME_STATUS_READY;
        }
    }
    else if ( _status == hoxGAME_STATUS_READY )
    {
        if ( _redPlayer == NULL || _blackPlayer == NULL )
        {
            _status = hoxGAME_STATUS_OPEN;
        }
    }
}

void
hoxTable::_resetMoveTimers( const hoxColor currColor )
{
    if ( _status != hoxGAME_STATUS_IN_PROGRESS ) return;

    /* Update the timestamp of the last Move. */
    const time_t now        = st_time();
    const time_t elapseTime = (_moves.size() > 2 ? (now - _lastMoveTime)
                                                 : 0 );
    _lastMoveTime = now;

    /* Start timers only after each Player made a Move. */
    const bool bGameStarted = (_moves.size() >= 2);
    if ( ! bGameStarted ) return;

    hoxTimeInfo* pCurrTime = &_blackTime;
    hoxTimeInfo* pNextTime = &_redTime;
    if ( currColor == hoxCOLOR_RED )
    {
        pCurrTime = &_redTime;
        pNextTime = &_blackTime;
    }

    /* Update the "current" Player's time. */
    pCurrTime->nGame -= elapseTime;
    if ( pCurrTime->nGame < 0 )
    {
        pCurrTime->nFree += pCurrTime->nGame; // Consume some of the Free time.
        if ( pCurrTime->nFree < 0 ) pCurrTime->nFree = 0;
        pCurrTime->nGame = 0;
    }

    /* Update the "next" Player's time. */
    if ( pNextTime->nGame == 0 ) pNextTime->nFree = _initialTime.nFree;
    else                         pNextTime->nMove = _initialTime.nMove;
    int nRemain = pNextTime->nGame + pNextTime->nFree;
    if ( nRemain > pNextTime->nMove ) nRemain = pNextTime->nMove;
    _nextMoveExpiry = now + nRemain;

    hoxLog(LOG_DEBUG, "%s: Turn : [%s], (%d / %d / %d) vs (%d / %d / %d)",
        __FUNCTION__, hoxUtil::colorToString(currColor).c_str(),
        pCurrTime->nGame, pCurrTime->nMove, pCurrTime->nFree,
        pNextTime->nGame, pNextTime->nMove, pNextTime->nFree);

}

void
hoxTable::_detectLongGameAndPerpetualCheck( const hoxColor color,
                                            hoxGameStatus& gameStatus,
                                            std::string&   sReason )
{
    /* Side-effects:
     *    The following member variables are effected:
     *     + The number of 'effective' moves.
     *     + The number of RED/BLACK perpetual moves.
     */

    const bool bCurrentCheck = _referee->isLastMoveCheck();

    if ( bCurrentCheck )
    {
        int perpetualChecks = 0;
        if ( color == hoxCOLOR_RED ) perpetualChecks = ++_redChecks;
        else                         perpetualChecks = ++_blackChecks;

        if ( perpetualChecks > hoxPERPETUAL_CHECKS_MAX )
        {
            gameStatus = ( color == hoxCOLOR_RED ? hoxGAME_STATUS_BLACK_WIN
                                                 : hoxGAME_STATUS_RED_WIN );
            sReason = "Perpetual Check detected";
            hoxLog(LOG_INFO, "%s: Game ended [%s]. %s.", __FUNCTION__,
                hoxUtil::gameStatusToString(gameStatus).c_str(), sReason.c_str());
        }
    }
    else /* non-check Move ? */
    {
        if ( color == hoxCOLOR_RED ) _redChecks   = 0;
        else                         _blackChecks = 0;

        if ( !_isPrevMoveCheck )
        {
            if ( ++_effectiveMoves > hoxEFFECTIVE_MOVES_MAX )
            {
                gameStatus = hoxGAME_STATUS_DRAWN;
                sReason = "Long Game detected";
                hoxLog(LOG_INFO, "%s: Game ended [%s]. %s.", __FUNCTION__,
                    hoxUtil::gameStatusToString(gameStatus).c_str(), sReason.c_str());
            }
        }
    }

    // Save the "last" check.
    _isPrevMoveCheck = bCurrentCheck;
}

void
hoxTable::_onGameEnded( const hoxGameStatus status,
                        const std::string&  sReason )
{
    hoxLog(LOG_DEBUG, "%s: Game ended: status [%s], reason [%s].", __FUNCTION__,
        hoxUtil::gameStatusToString(status).c_str(), sReason.c_str());

    _status       = status;
    _drawPlayerId = "";

    _postAll_EndEvent( _status, sReason );

    bool bGuestTable = false;
    if (   _redPlayer->getType()   == hoxPLAYER_TYPE_GUEST
        || _blackPlayer->getType() == hoxPLAYER_TYPE_GUEST )
    {
        bGuestTable = true;
    }

    bool bScoreChanged = false;
    if ( _gameType == hoxGAME_TYPE_RATED && !bGuestTable )
    {
        bScoreChanged = _recordGameResult();
    }

    if ( bScoreChanged )
    {
        _postAll_ScoreEvent( _redPlayer );
        _postAll_ScoreEvent( _blackPlayer );
    }
}

void
hoxTable::_onGameReset()
{
    const char* FNAME = "hoxTable::_onGameReset";

    hoxLog(LOG_DEBUG, "%s: Game reset.", FNAME);

    _status       = hoxGAME_STATUS_OPEN;
    _drawPlayerId = "";

    /* Update the game's status. */
    _updateStatus();

    /* Reset timers. */
    _redTime   = _initialTime;
    _blackTime = _initialTime;

    /* Reset the game (move-list,...) */
    _referee->resetGame();
    _moves.clear();

    /* Reset move counts. */
    _isPrevMoveCheck = false;
    _effectiveMoves  = 0;
    _redChecks       = 0;
    _blackChecks     = 0;

    /* Notify all players. */
    _postAll_ResetEvent();
}

bool
hoxTable::_recordGameResult()
{
    const char* FNAME = "hoxTable::_recordGameResult";
    bool bScoreChanged = true;
    std::string sResult_RED;   /* Game-result: (W)in / (D)raw / (L)oss */
    std::string sResult_BLACK;

    hoxLog(LOG_INFO, "%s: Table [%s]: [%s] vs. [%s] => Result [%s].", 
        FNAME, _id.c_str(), 
        _redPlayer->getId().c_str(), _blackPlayer->getId().c_str(),
        hoxUtil::gameStatusToString(_status).c_str());

    /* Calculate the new Score for each player. */

    _calculateNewScores();

    switch ( _status )
    {
        case hoxGAME_STATUS_RED_WIN:
            _redPlayer->setWins( _redPlayer->getWins() + 1 );
            _blackPlayer->setLosses( _blackPlayer->getLosses() + 1 );
            sResult_RED   = "W";
            sResult_BLACK = "L";
            break;

        case hoxGAME_STATUS_BLACK_WIN:
            _redPlayer->setLosses( _redPlayer->getLosses() + 1 );
            _blackPlayer->setWins( _blackPlayer->getWins() + 1 );
            sResult_RED   = "L";
            sResult_BLACK = "W";
            break;

        case hoxGAME_STATUS_DRAWN:
            _redPlayer->setDraws( _redPlayer->getDraws() + 1 );
            _blackPlayer->setDraws( _blackPlayer->getDraws() + 1 );
            sResult_RED   = "D";
            sResult_BLACK = "D";

        default:
            bScoreChanged = false;
    }

    /* Save the new Score(s) and game-result to Database. */

    hoxDbClient::set_player_info( _redPlayer, sResult_RED );
    hoxDbClient::set_player_info( _blackPlayer, sResult_BLACK );

    return bScoreChanged;
}

void hoxTable::_calculateNewScores()
{
    /* References: Using "Elo Rating System":
     *   http://www.chesselo.com
     *   http://gobase.org/studying/articles/elo/
     */

    // NOTE: Focus more on the "experienced" player.
    const bool bUseRED =
        _redPlayer->getPlayedGames() > _blackPlayer->getPlayedGames();

    const int nRatingDiff =
        ( bUseRED ? _redPlayer->getScore() - _blackPlayer->getScore()
                  : _blackPlayer->getScore() - _redPlayer->getScore() );

    const int nOldRating =
        ( bUseRED ? _redPlayer->getScore() : _blackPlayer->getScore() );

    const int nPlayedGames =
        ( bUseRED ? _redPlayer->getPlayedGames() : _blackPlayer->getPlayedGames() );

    float fGameResult = 0.0;
    switch ( _status )
    {
        case hoxGAME_STATUS_RED_WIN:   fGameResult = bUseRED ? 1.0 : 0.0; break;
        case hoxGAME_STATUS_BLACK_WIN: fGameResult = bUseRED ? 0.0 : 1.0; break;
        default: /* DRAWN */           fGameResult = 0.5;
    }

    const int nRatingChange =
        _Elo_calculate_NewRatingChange( nRatingDiff, nOldRating,
                                        nPlayedGames, fGameResult );

    if ( bUseRED )
    {
        _redPlayer->setScore( _redPlayer->getScore() + nRatingChange );
        _blackPlayer->setScore( _blackPlayer->getScore() - nRatingChange );
    }
    else
    {
        _redPlayer->setScore( _redPlayer->getScore() - nRatingChange );
        _blackPlayer->setScore( _blackPlayer->getScore() + nRatingChange );
    }
}

void
hoxTable::_postAll_JoinEvent( hoxPlayer_SPtr player,
                              hoxColor   joinColor ) const
{
    const hoxResponse_SPtr event =
        hoxResponse::create_event_E_JOIN( this, player, joinColor );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_LeaveEvent( hoxPlayer_SPtr player ) const
{
    const hoxResponse_SPtr event = hoxResponse::create_event_LEAVE( this, player );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_MoveEvent( hoxPlayer_SPtr     player,
                              const std::string& sMove,
                              hoxGameStatus      gameStatus )
{
    const hoxResponse_SPtr event = 
        hoxResponse::create_event_MOVE( this, player, sMove, gameStatus );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        if ( *it == player ) { continue; /* Skip the sender */ } 
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_DrawEvent( hoxPlayer_SPtr player )
{
    const hoxResponse_SPtr event = 
        hoxResponse::create_event_DRAW( hoxRC_OK, this, player );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        if ( *it == player ) { continue; /* Skip the sender */ }
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_EndEvent( const hoxGameStatus gameStatus,
                             const std::string&  sReason )
{
    const hoxResponse_SPtr event = 
        hoxResponse::create_event_END( this, gameStatus, sReason );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_ResetEvent()
{
    const hoxResponse_SPtr event = hoxResponse::create_event_RESET( this );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_ScoreEvent( const hoxPlayer_SPtr player )
{
    const hoxResponse_SPtr event = 
        hoxResponse::create_event_E_SCORE( this, player );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

void
hoxTable::_postAll_UpdateEvent( const hoxPlayer_SPtr player,
                                const hoxGameType  newGameType,
                                const hoxTimeInfo& newInitialTime )
{
    const hoxResponse_SPtr event = 
        hoxResponse::create_event_UPDATE( this, player, 
                                          newGameType, newInitialTime );

    for ( hoxPlayerList::const_iterator it = _allPlayers.begin();
                                        it != _allPlayers.end(); ++it )
    {
        (*it)->onNewEvent( event );
    }
}

// =========================================================================
//
//                        hoxTableMgr
//
// =========================================================================

/* Define the static singleton instance. */
hoxTableMgr* hoxTableMgr::s_instance = NULL;

/*static*/
hoxTableMgr*
hoxTableMgr::getInstance()
{
    if ( hoxTableMgr::s_instance == NULL )
    {
        hoxTableMgr::s_instance = new hoxTableMgr();
    }
    return hoxTableMgr::s_instance;
}

hoxTable_SPtr
hoxTableMgr::createTable( const hoxTimeInfo& initialTime )
{
    const std::string newTableId = _generateNewTableId();

    hoxTable_SPtr pTable( new hoxTable( newTableId, initialTime ) );

    _tableMap[newTableId] = pTable;
    return pTable;
}

bool
hoxTableMgr::deleteTable( const std::string& tableId )
{
    TableContainer::iterator foundIt = _tableMap.find( tableId );
    if ( foundIt == _tableMap.end() )
    {
        return false;
    }

    _tableMap.erase( foundIt );
    _freeIdList.push_back( hoxUtil::stringToInt(tableId) );
    return true;
}

hoxTable_SPtr
hoxTableMgr::findTable( const std::string& tableId ) const
{
    hoxTable_SPtr pTable;

    TableContainer::const_iterator foundIt = _tableMap.find( tableId );
    if ( foundIt != _tableMap.end() )
    {
        pTable = foundIt->second;
    }

    return pTable;
}

void
hoxTableMgr::getTables( hoxTableList& tables ) const
{
    tables.clear();
    for ( TableContainer::const_iterator it = _tableMap.begin();
                                         it != _tableMap.end(); ++it )
    {
        tables.push_back( it->second );
    }
}

void
hoxTableMgr::runCleanup()
{
    const char* FNAME = "hoxTableMgr::runCleanup";
    hoxTable_SPtr  pTable;

    /* NOTE: The loop below, we remove items from a STD container within a loop.
     *       Here is a discussion about such a usage:
     *          http://www.codeguru.com/forum/printthread.php?t=446029
     */
    for ( TableContainer::iterator it = _tableMap.begin();
                                   it != _tableMap.end(); /* nothing here */ )
    {
        pTable = it->second;
        if ( pTable->isEmpty() )
        {
            hoxLog(LOG_DEBUG, "%s: Purge the empty table [%s].", FNAME, pTable->getId().c_str());
            _tableMap.erase( it++ );
            _freeIdList.push_back( hoxUtil::stringToInt(pTable->getId()) );
        }
        else
        {
           ++it;
        }
    }
}

void
hoxTableMgr::manageTables()
{
    for ( TableContainer::const_iterator it = _tableMap.begin();
                                         it != _tableMap.end(); ++it )
    {
        it->second->checkTimeoutOnMove();
    }
}

const std::string
hoxTableMgr::_generateNewTableId()
{
    int nId = 0;

    if ( _freeIdList.empty() )
    {
        nId = _tableMap.size() + 1;
    }
    else
    {
        nId = _freeIdList.front();
        _freeIdList.pop_front();
    }

    return hoxUtil::intToString( nId );
}

/******************* END OF FILE *********************************************/
