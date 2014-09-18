//
// C++ Interface: hoxEnums
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_ENUMS_H_
#define __INCLUDED_HOX_ENUMS_H_

/******************************************************************
 * Useful macros
 */

#define SEC2USEC(s) ((s)*1000000LL)

/******************************************************************/
/******************************************************************/

/**
 * Results (... Return-Code)
 */
enum hoxResult
{
    hoxRC_UNKNOWN = -1,

    hoxRC_OK = 0,
    hoxRC_ERR,          // A generic error.

    hoxRC_TIMEOUT,      // A timeout error.
    hoxRC_HANDLED,      // Something (request, event,...) has been handled.
    hoxRC_CLOSED,       // Something (socket,...) has been closed.
    hoxRC_NOT_VALID,    // Something is not valid.
    hoxRC_NOT_FOUND,    // Something is not found.
    hoxRC_NOT_ALLOWED,  // Some action is not allowed.
    hoxRC_NOT_SUPPORTED // Something is not supported.
};

/**
 * Request types comming from the remote Clients.
 */
enum hoxRequestType
{
    hoxREQUEST_UNKNOWN = -1,

    hoxREQUEST_HELLO,
        /* Get server's info */

    hoxREQUEST_DB_PLAYER_PUT,
        /* Put (create) a new Database Player's info */

    hoxREQUEST_DB_PLAYER_GET,
        /* Get Database Player's info */

    hoxREQUEST_DB_PLAYER_SET,
        /* Set Database Player's info */

    hoxREQUEST_DB_PROFILE_SET,
        /* Set Database Profile's info */

    hoxREQUEST_DB_PASSWORD_SET,
        /* Set Database Player's NEW password */

    hoxREQUEST_HTTP_GET,
        /* HTTP GET request */

    hoxREQUEST_LOG,
        /* Log a message to disk */
};

/**
 * Network constants.
 */
enum hoxNetworkContant
{
    /*
     * !!! Do not change the values nor orders of the following !!!
     */

    hoxNETWORK_MAX_MSG_SIZE         = ( 4 * 1024 ), // 4-KByte buffer
    hoxNETWORK_DEFAULT_SERVER_PORT  = 8000
};

/**
 * Socket related constants.
 */
enum hoxSocketContant
{
    /*
     * !!! Do not change the values the following !!!
     */

    hoxSOCKET_CLIENT_TIMEOUT = 10,   // 10 seconds
        /* Timeout applied to client -> server connection */

    hoxSOCKET_SERVER_ACCEPT_TIMEOUT = 5,    // 5 seconds
        /* Timeout applied to server-socket which is waiting
        * by wxSocketServer::Accept() for new incoming client
        * connections. This timeout is needed so that the server
        * can process the SHUTDOWN request.
        */
};

#endif /* __INCLUDED_HOX_ENUMS_H_ */
