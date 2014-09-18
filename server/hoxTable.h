//
// C++ Interface: hoxTable
//
// Description: The Table
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#ifndef __INCLUDED_HOX_TABLE_H__
#define __INCLUDED_HOX_TABLE_H__

#include <string>
#include <map>
#include "hoxPlayer.h"
#include "hoxTypes.h"

/* Forward declarations. */
class hoxReferee;

/**
 * A Table.
 */
class hoxTable
{
public:
    hoxTable( const std::string& id,
              const hoxTimeInfo& initialTime );
    ~hoxTable();

    const std::string getId() const { return _id; }

    hoxGameGroup getGameGroup() const { return _gameGroup; }
    hoxGameType getGameType() const { return _gameType; }
    hoxTimeInfo getInitialTime() const { return _initialTime; }
    hoxTimeInfo getRedTime() const { return _redTime; }
    hoxTimeInfo getBlackTime() const { return _blackTime; }
    hoxPlayer_SPtr getRedPlayer() const { return _redPlayer; }
    hoxPlayer_SPtr getBlackPlayer() const { return _blackPlayer; }
    void getObservers(hoxPlayerList& observers) const;
    hoxGameStatus getStatus() const { return _status; }
    const hoxStringList& getMoves() const { return _moves; }

    void getCurrentTimers( hoxTimeInfo& redTime,
                           hoxTimeInfo& blackTime ) const;

    /**
     * Callback function from a player who is leaving the table.
     */
    void onLeave_FromPlayer( hoxPlayer_SPtr player );

    /**
     * Callback function from a Player who sends a message.
     *
     * @param player The Player who generates the message.
     * @param pMessage The event containing the message.
     */
    void onMessage_FromPlayer( hoxPlayer_SPtr          player,
                               const hoxResponse_SPtr& pMessage );

    /**
     * Attempt to assign a player to this Table as a specified role.
     *
     * @param player The Player who is requesting to join the Table.
     * @param requestColor The requested Color the Player wants to join as.
     */
    hoxResult assignPlayerAs( hoxPlayer_SPtr player,
                              const hoxColor requestColor );

    /**
     * Send the information of this Table to a Player who just:
     *   (1) Join the Table, or
     *   (2) Re-enter (resume) the Table after being disconnected.
     *
     * @param player The Player who will receive the information.
     */
    void sendInfoToPlayer( hoxPlayer_SPtr player );

    /**
     * Accept Move from a given Player.
     *
     * @param player The player who made the Move.
     * @param sMove The string containing the Move.
     */
    hoxResult acceptMove( hoxPlayer_SPtr     player,
                          const std::string& sMove );

    /**
     * Handle the Request-to-Resign from a given Player.
     *
     * @param player The player who made the request.
     */
    hoxResult handleResignRequest( hoxPlayer_SPtr player );

    /**
     * Handle the Request-to-Draw from a given Player.
     *
     * @param player The player who made the request.
     */
    hoxResult handleDrawRequest( hoxPlayer_SPtr player );

    /**
     * Handle the Request-to-Reset from a given Player.
     *
     * @param player The player who made the request.
     */
    hoxResult handleResetRequest( hoxPlayer_SPtr player );

    /**
     * Handle Update (Table) from a given Player.
     *
     * @param player The player who made the request.
     * @param bRatedGame The Rated/Non-Rated Game option.
     * @param newInitialTime The new Table's Initial-Time.
     */
    hoxResult handleUpdateRequest( hoxPlayer_SPtr     player,
                                   const bool         bRatedGame,
                                   const hoxTimeInfo& newInitialTime );

    /**
     * Check timeout while waiting for the next Move.
     * If there is a timeout, end the Game and notify Players.
     */
    void checkTimeoutOnMove();

    /**
     * Check if this Table has no Player attending.
     */
    bool isEmpty() const { return _allPlayers.empty(); }

private:
    /**
     * Unseat a given player from this table.
     *
     * @param player The player to be unseated.
     */
    void _unassignPlayer( hoxPlayer_SPtr player );

    /**
     * @return true if the Player is newly added (ie. just joined the Table.).
     */
    bool _addPlayer( hoxPlayer_SPtr player, hoxColor role );

    void _removePlayer( hoxPlayer_SPtr player );

    void _updateStatus();
    void _resetMoveTimers( const hoxColor currColor );

    void _detectLongGameAndPerpetualCheck( const hoxColor color,
                                           hoxGameStatus& gameStatus,
                                           std::string&   sReason );

    void _onGameEnded( const hoxGameStatus status,
                       const std::string&  sReason );
    void _onGameReset();

    bool _recordGameResult();
    void _calculateNewScores();

    void _postAll_JoinEvent( hoxPlayer_SPtr player,
                             hoxColor   joinColor ) const;
    void _postAll_LeaveEvent( hoxPlayer_SPtr player ) const;
    void _postAll_MoveEvent( hoxPlayer_SPtr     player,
                             const std::string& sMove,
                             hoxGameStatus      gameStatus );
    void _postAll_DrawEvent( hoxPlayer_SPtr player );
   
    void _postAll_EndEvent( const hoxGameStatus gameStatus,
                            const std::string&  sReason );
    void _postAll_ResetEvent();
    void _postAll_ScoreEvent( const hoxPlayer_SPtr player );
    void _postAll_UpdateEvent( const hoxPlayer_SPtr player,
                               const hoxGameType  newGameType,
                               const hoxTimeInfo& newInitialTime );

private:
    std::string     _id;

    hoxGameGroup    _gameGroup;   // Public / Private
    hoxGameType     _gameType;    // Rated / Unrated / Solo

    hoxTimeInfo     _initialTime; // The initial allowed Game-Time.
    hoxTimeInfo     _redTime;
    hoxTimeInfo     _blackTime;

    hoxPlayer_SPtr  _redPlayer;
    hoxPlayer_SPtr  _blackPlayer;
    hoxPlayerList   _allPlayers;  // Players + Observers
       /* TODO: How about using std::set instead of std::list */

    hoxReferee*     _referee;     // The referee
    hoxGameStatus   _status;      // The game's status.
    hoxStringList   _moves;       // The list of Moves made so far.

    std::string     _drawPlayerId;
        /* The Id of the Player asking to draw.
         * If it is empty, then there is no player requesting.
         */

    time_t          _lastMoveTime; // Timestamp of the last move.

    time_t          _nextMoveExpiry;
        /* The time before which the "next" Player must make a move
         * or will lose by timeout.
         */

    bool            _isPrevMoveCheck;
        /* Whether the previous Move is a check. */

    int             _effectiveMoves;
        /* The number of 'effective' (i.e., non-check) Moves. */

    int             _redChecks;
    int             _blackChecks;
        /* The number of consecutive 'Check' Moves. */
};

/**
 * The Manager of all Tables.
 * This class is implemented as a singleton.
 */
class hoxTableMgr
{
private:
    static hoxTableMgr* s_instance;  // The singleton instance.

    typedef std::map<const std::string, hoxTable_SPtr> TableContainer;
    typedef std::list<int> FreeTableIdList;

public:
    static hoxTableMgr* getInstance();

public:
    ~hoxTableMgr() {}

    hoxTable_SPtr createTable( const hoxTimeInfo& initialTime );
    bool deleteTable(const std::string& tableId);

    /**
     * Find a Table by table-Id.
     *
     * @param tableId The table-Id to find.
     * @return An empty pointer if not found.
     */
    hoxTable_SPtr findTable(const std::string& tableId) const;

    void getTables(hoxTableList& tables) const;

    /**
     * Run a cleanup procedure.
     */
    void runCleanup();

    void manageTables();

private:
    hoxTableMgr() {}

    const std::string _generateNewTableId();

private:
    mutable TableContainer  _tableMap;
    FreeTableIdList         _freeIdList;
};

#endif /* __INCLUDED_HOX_TABLE_H__ */
