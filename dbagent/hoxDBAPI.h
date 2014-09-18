//
// C++ Interface: hoxDBAPI
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_DB_API_H_
#define __INCLUDED_HOX_DB_API_H_

#include <string>
#include "hoxEnums.h"

namespace hoxDBAPI
{

    class Player_t
    {
    public:
        Player_t() : score( -1 ), wins(0), draws(0), losses(0) {}

        std::string   id;    // Player-Id
        std::string   hpw;   // Hashed password.
        int           score;
        int           wins;
        int           draws;
        int           losses;
        std::string   email;
    };

    /**
     * Put (create) the Info of a NEW Player.
     *
     * @param playerInfo The new Player's Info to be put (saved) to Database.
     * @param sEmail The new Player's Email (optional).
     */
    hoxResult
    put_player_info( const Player_t&    playerInfo,
                     const std::string& sEmail );

    /**
     * Get Info of a Player.
     *
     * @param pid The Player-Id.
     * @param playerInfo The Info retrieved from Database.
     */
    hoxResult
    get_player_info( const std::string& pid,
                     Player_t&          playerInfo );

    /**
     * Set Info of a Player.
     *
     * @param playerInfo The Info to be saved to Database.
     * @param sGameResult The game's result: (W)in / (D)raw / (L)oss
     */
    hoxResult
    set_player_info( const Player_t&    playerInfo,
                     const std::string& sGameResult );

    /**
     * Set Info of a Profile.
     *
     * @param pid The Player-Id.
     * @param sEmail The player's email.
     * @param sPassword [OPTIONAL] The player's password.
     */
    hoxResult
    set_profile_info( const std::string& pid,
                      const std::string& sEmail,
                      const std::string& sPassword );

    /**
     * Set new Password of a Player.
     *
     * @param playerInfo The Info to be saved to Database.
     */
    hoxResult
    set_player_password( const Player_t& playerInfo );

} /* namespace hoxDBAPI */

#endif /* __INCLUDED_HOX_DB_API_H_ */
