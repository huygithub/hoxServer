//
// C++ Interface: hoxPlayer
//
// Description: The Player
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#ifndef __INCLUDED_HOX_PLAYER_H__
#define __INCLUDED_HOX_PLAYER_H__

#include <string>
#include <boost/enable_shared_from_this.hpp>
#include "hoxTypes.h"

/**
 * A Player.
 */
class hoxPlayer : public boost::enable_shared_from_this<hoxPlayer>
{
public:
    hoxPlayer(const std::string&  id,
              const hoxPlayerType type = hoxPLAYER_TYPE_NORMAL );
    ~hoxPlayer();

    const std::string getId() const { return _id; }
    const hoxPlayerType getType() const { return _type; }

    void setScore(int score) { _score = score; }
    int  getScore() const { return _score; }

    void setWins(int val) { _wins = val; }
    int  getWins() const { return _wins; }

    void setDraws(int val) { _draws = val; }
    int  getDraws() const { return _draws; }

    void setLosses(int val) { _losses = val; }
    int  getLosses() const { return _losses; }

    int  getPlayedGames() const { return _wins + _draws + _losses; }

    void setSession(hoxSession_SPtr session) { _session = session; }
    void clearSession() { _session.reset(); }

    void setHPassword(const std::string& hpw) { _hpassword = hpw; }
    const std::string getHPassword() const { return _hpassword; }

    /**
     * On receiving a new event: put the even into outgoing queue.
     *
     * @param event The new event.
     */
    void onNewEvent( const hoxResponse_SPtr& event );

    /**
     * Request to join a Table as a specified role.
     *
     * @param table        The Table to join.
     * @param requestColor The request role (color).
     */
    void joinTableAs( const hoxTable_SPtr& pTable,
                      const hoxColor       requestColor );

    /**
     * Leave a given Table.
     *
     * @param pTable The Table from which this Player wants to leave.
     */
    void leaveTable( const hoxTable_SPtr& pTable );

    /**
     * Leave all Table(s) that this Player is attending.
     */
    void leaveAllTables();

    /**
     * Query to check if the Player is at a given Table.
     *
     * @param pTable The Table that is being checked.
     */
    bool isAtTable( const hoxTable_SPtr& pTable ) const;

    /**
     * Do a Move to a given Table.
     *
     * @param tableId The Table-Id.
     * @param sMove The string containing the Move.
     */
    void doMove( const std::string&  tableId,
                 const std::string&  sMove );

    /**
     * Offer a Resign in a given Table.
     *
     * @param tableId The Table-Id.
     */
    void offerResign( const std::string&  tableId );

    /**
     * Offer a Draw in a given Table.
     *
     * @param tableId The Table-Id.
     */
    void offerDraw( const std::string&  tableId );

    /**
     * Reset in a given Table.
     *
     * @param tableId The Table-Id.
     */
    void resetTable( const std::string&  tableId );

    /**
     * Update the Option of a given Table.
     *
     * @param tableId The Table-Id.
     * @param bRatedGame The Rated/Non-Rated Game option.
     * @param newInitialTime The new Table's Initial-Time.
     */
    hoxResult updateTable( const std::string& tableId,
                           const bool         bRatedGame,
                           const hoxTimeInfo& newInitialTime );

    /**
     * Attempt to resume playing after having re-connected.
     */
    void resumePlayingIfNeeded();
                           
private:
    /**
     * Find a Table from a given Table-Id.
     *
     * @param bThrowErrorIfNotFound If true, then this function will throw
     *                              an exception if a Table is not found.
     */
    hoxTable_SPtr _findTable( const std::string& tableId,
                              bool bThrowErrorIfNotFound = true ) const;

private:
    const std::string   _id;
    const hoxPlayerType _type;
    int                 _score;
    int                 _wins;
    int                 _draws;
    int                 _losses;

    std::string         _hpassword;
            /* Hashed password. Required for authentication. */

    hoxSession_SPtr     _session;

    hoxTableSet         _tables;
};

#endif /* __INCLUDED_HOX_PLAYER_H__ */
