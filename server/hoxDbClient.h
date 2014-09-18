//
// C++ Interface: hoxDbClient
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_DB_CLIENT_H_
#define __INCLUDED_HOX_DB_CLIENT_H_

#include <string>
#include "hoxTypes.h"

namespace hoxDbClient
{
    /**
     * Initialize this client.
     *
     */
    hoxResult initialize( const char* szHost, 
                          int         nPort );

    /**
     * De-initialize this client.
     *
     */
    hoxResult deinitialize();

    /**
     * Send a HELLO request to the DB-Agent.
     *
     */
    hoxResult send_HELLO();

    /**
     * Put (create) a new Player's Info.
     *
     */
    hoxResult put_player_info( hoxPlayer_SPtr     player,
                               const std::string& sEmail );

    /**
     * Get the info. of a Player.
     *
     */
    hoxResult get_player_info( hoxPlayer_SPtr player );

    /**
     * Set the info. of a Player.
     *
     */
    void set_player_info( const hoxPlayer_SPtr player,
                          const std::string&   sGameResult );

    /**
     * Set the new Password of a Player.
     *
     */
    hoxResult set_player_password( const hoxPlayer_SPtr player );

    /**
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!! SPECIAL API !!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *
     * Ask the WWW server to authenticate a given Player.
     *
     * @param sPlayerId The Player's Id.
     * @param sHPassword The Player's hashed password.
     */
    hoxResult WWW_authenticate( const std::string& sPlayerId,
                                const std::string& sHPassword );

    /**
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!! HTTP API    !!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */

    hoxResult get_http_file( const std::string& sPath,
                             std::string&       sFileContent );

    /**
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!! LOG API     !!!!!!!!!!!!!!!!!!!!
     *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */

    hoxResult log_msg( const std::string& sMsg );

} /* namespace hoxDbClient */

#endif /* __INCLUDED_HOX_DB_CLIENT_H_ */
