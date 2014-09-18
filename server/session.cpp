//
// C++ Implementation: session
//
// Description: The handler of a Session.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#include <netinet/in.h>
#include <arpa/inet.h>

#include <st.h>
#include "hoxEnums.h"
#include "hoxLog.h"
#include "hoxExcept.h"
#include "hoxDebug.h"
#include "hoxTypes.h"
#include "hoxSocketAPI.h"
#include "hoxSessionMgr.h"
#include "hoxUtil.h"
#include "hoxTable.h"
#include "hoxDbClient.h"
#include "hoxFileMgr.h"
#include <boost/tokenizer.hpp>
#include <sstream>
#include <map>

/******************************************************************
 * Constants
 */

#define NEW_PLAYER_SCORE   1500  /* The initial score of new Players. */

#define SESSION_MANAGER_INTERVAL 5 /* Session manager interval (in seconds) */
#define TABLE_MANAGER_INTERVAL   1 /* Table manager interval (in seconds) */

/******************************************************************
 * Extern declaration
 */

extern int my_index;       /* Current process index: defined in main.cpp */
extern pid_t my_pid;       /* Current process pid: defined in main.cpp   */

/******************************************************************
 * Implementation
 */

/**
 * Send back a response to the client.
 */
void
_write_response( st_netfd_t       nfd,
                 hoxResponse_SPtr pResponse,
                 hoxClientType    clientType )
{
    std::string sResp = pResponse->toString();

    if ( clientType == hoxCLIENT_TYPE_HTTP )
    {
        sResp = hoxPollingSession::build_http_response( sResp );
    }
    else if ( clientType == hoxCLIENT_TYPE_FLASH )
    {
        sResp += '\0';
    }

    (void) hoxSocketAPI::write_data( nfd, sResp );
}

/**
 * Handle the special REGISTER request.
 */
void
_handle_request_REGISTER( st_netfd_t             nfd,
                          const hoxRequest_SPtr& pRequest,
                          hoxClientType          clientType )
{
    const char* FNAME = __FUNCTION__;
    hoxResult result = hoxRC_UNKNOWN;
    hoxResponse_SPtr  pResponse;
    std::string       sContent;

    hoxLog(LOG_INFO, "%s: ENTER.", FNAME);

    try
    {
        const std::string sPlayerId = pRequest->getParam("pid");
        const std::string hpassword = pRequest->getParam("password");
        const std::string sEmail    = pRequest->getParam("email");

        if ( sPlayerId.empty() || hpassword.empty() )
        {
            throw hoxError(hoxRC_ERR, "Player-ID or Password is empty");
        }

        hoxPlayer_SPtr pPlayer( new hoxPlayer( sPlayerId ) );

        /* Check if the Player's ID has been taken. */
        if ( hoxRC_OK == hoxDbClient::get_player_info( pPlayer ) )
        {
            throw hoxError(hoxRC_NOT_FOUND, "Player-ID not available");
        }

        pPlayer->setScore( NEW_PLAYER_SCORE );
        pPlayer->setHPassword( hpassword );

        /* Put (save/create) this new Player into the Database. */
        if ( hoxRC_OK != hoxDbClient::put_player_info( pPlayer, sEmail ) )
        {
            throw hoxError(hoxRC_ERR, "Failed to register new player into DB");
        }

        sContent = "Successfully registered new account: [" + sPlayerId + "]";
        result = hoxRC_OK;
    }
    catch( hoxError error )
    {
        result = error.code();
        sContent = error.what();
        hoxLog(LOG_WARN, "%s: Error caught [%s].", FNAME, error.toString().c_str());
    }


    /* Return the response. */
    pResponse.reset( new hoxResponse( pRequest->getType(), result ) );
    pResponse->setContent( sContent + std::string("\n") );
    _write_response( nfd, pResponse, clientType );
}

/**
 * Read the FIRST incoming request from a given socket.
 */
hoxResult
_read_first_request( const int        thread_id,
                     st_netfd_t       nfd,
                     hoxClientType&   clientType,
                     hoxRequest_SPtr& pRequest )
{
    const struct in_addr* from = (struct in_addr *) st_netfd_getspecific(nfd);
    hoxResult    result = hoxRC_UNKNOWN;
    std::string  sRequest;

    clientType = hoxCLIENT_TYPE_HOXCHESS; // The default client-type.

    result = hoxSocketAPI::read_line( nfd, sRequest, PERSIST_READ_TIMEOUT );
    if ( sRequest.empty() ) /* Check for socket-close condition. */
    {
        return hoxRC_CLOSED; // Empty request due to socket-close
    }
    else if ( result != hoxRC_OK )
    {
        hoxLog(LOG_INFO, "%s: (Thread %d) Failed to read request from [%s].",
            __FUNCTION__, thread_id, inet_ntoa(*from) );
        return result;
    }

    /* Check if this is a HTTP (GET/POST) request. */

    hoxHttpRequest httpRequest;

    if ( sRequest.find("GET")  == 0 || sRequest.find("POST") == 0 )
    {
        httpRequest.parseURI( sRequest );
        result = httpRequest.continueReadRequest( nfd );
        if ( result != hoxRC_OK )
        {
            return result;
        }

        if ( httpRequest.method == "GET" )
        {
            hoxPollingSession::handle_http_GET( nfd, httpRequest );
            return hoxRC_HANDLED;
        }
        else // ... POST method, the 'real' request is in the body.
        {
            sRequest = httpRequest.body;
            hoxUtil::trimLast( sRequest, '\n' );
            clientType = hoxCLIENT_TYPE_HTTP;
        }
    }

    /* Parse the request as HOXChess-specific request. */

    hoxLog(LOG_DEBUG, "%s: Request: [%s]", __FUNCTION__, sRequest.c_str());

    pRequest.reset( new hoxRequest( sRequest ) );
    if ( ! pRequest->isValid() )
    {
        hoxLog(LOG_INFO, "%s: Request [%s] is invalid.", __FUNCTION__, sRequest.c_str());
        return hoxRC_NOT_VALID;
    }

    /* Detect and specially handle Flash-based clients. */
    if ( pRequest->getType() == hoxREQUEST_LOGIN )
    {
        const std::string sVersion = pRequest->getParam("version");
        hoxLog(LOG_INFO, "%s: LOGIN: client version = [%s].", __FUNCTION__, sVersion.c_str());
        if ( sVersion.find("FLASHCHESS") == 0 ) {
            clientType = hoxCLIENT_TYPE_FLASH;
        } else if ( sVersion.find("hoxTest") == 0 ) {
            clientType = hoxCLIENT_TYPE_TEST;
        }
    }

    return hoxRC_OK;
}

/**
 * Authenticate a normal Player.
 * Retrieve the player's info from the Database and compare password.
 */
hoxPlayer_SPtr
_authenticate_player( const std::string& sPlayerId,
                      const std::string& hpassword )
{
    hoxPlayer_SPtr pPlayer( new hoxPlayer( sPlayerId ) );

    if ( hoxRC_OK != hoxDbClient::get_player_info( pPlayer ) )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Failed to get player's info");
    }

    if ( pPlayer->getScore() == 0 && pPlayer->getHPassword().empty() )
    {
        hoxLog(LOG_INFO, "%s: Player [%s] not found in DB.", __FUNCTION__, sPlayerId.c_str());
        throw hoxError(hoxRC_NOT_FOUND, "Authentication failed");
    }
    else if ( hpassword != pPlayer->getHPassword() )
    {
        hoxLog(LOG_INFO, "%s: Player [%s] entered wrong password.", __FUNCTION__, sPlayerId.c_str());
        throw hoxError(hoxRC_NOT_VALID, "Wrong password");
    }

    return pPlayer;
}

/**
 * Authenticate a Guest.
 * @note: No need to do anything as we already checked for
 *        existing session (with the same guest-ID).
 */
hoxPlayer_SPtr
_authenticate_guest( const std::string& sPlayerId )
{
    hoxPlayer_SPtr pPlayer( new hoxPlayer(sPlayerId, hoxPLAYER_TYPE_GUEST) );
    pPlayer->setScore( NEW_PLAYER_SCORE );

    hoxLog(LOG_INFO, "%s: Guest [%s] authenticated with score = [%d].",
        __FUNCTION__, sPlayerId.c_str(), pPlayer->getScore() );
    return pPlayer;
}

/**
 * Authenticate a request.
 */
hoxSession_SPtr
_authenticate_request( st_netfd_t             nfd,
                       const hoxRequest_SPtr& pRequest,
                       const hoxClientType    clientType,
                       hoxResponse_SPtr&      pResponse )
{
    hoxSession_SPtr pSession;
    hoxPlayer_SPtr  pPlayer;

    const hoxRequestType requestType = pRequest->getType();
    const std::string    sPlayerId   = pRequest->getParam("pid");
    const std::string    sSessionId  = pRequest->getParam("sid");
    const std::string    hpassword   = pRequest->getParam("password");

    const bool bIsGuest = ( sPlayerId.find("Guest#") == 0 );

    try
    {
        pSession = hoxSessionMgr::getInstance()->findSession( sPlayerId );
        if ( pSession )
        {
            if ( pSession->getId() == sSessionId )
            {
                hoxLog(LOG_DEBUG, "%s: Reuse session [%s %s].",
                    __FUNCTION__, sSessionId.c_str(), sPlayerId.c_str());
                // FIXME: Need to update the network connection 'nfd'.
                return pSession;
            }
        }

        /* Make sure that the request is LOGIN. */
        if ( requestType != hoxREQUEST_LOGIN )
        {
            throw hoxError(hoxRC_NOT_ALLOWED, "Not yet authenticated");
        }

        /* Perform authentication. */
        if ( bIsGuest ) pPlayer = _authenticate_guest( sPlayerId );
        else            pPlayer = _authenticate_player( sPlayerId, hpassword );

        /* If there is an existing Session, then close it first. */
        if ( pSession )
        {
            if ( !bIsGuest && pSession->resumeConnection( nfd, clientType ) )
            {
                hoxLog(LOG_DEBUG, "%s: Resume session (after LOGIN) [%s %s].",
                    __FUNCTION__, sSessionId.c_str(), sPlayerId.c_str());
                return pSession;
            }

            hoxLog(LOG_INFO, "%s: Closing existing session [%s]...", __FUNCTION__, pSession->getId().c_str());
            hoxSessionMgr::getInstance()->closeAndDeleteSession( pSession );
            hoxASSERT_MSG(!pSession, "Existing session should have been deleted");
        }

        /* Create a new session. */
        pSession = hoxSessionMgr::getInstance()->createSession( clientType, nfd, pPlayer );
        if ( ! pSession )
        {
            throw hoxError(hoxRC_ERR, "Failed to create a Session");
        }
    }
    catch( const hoxError error )
    {
        pResponse.reset( new hoxResponse( requestType, error.code() ) );
        pResponse->setContent( error.what() + std::string("\n") );
        hoxLog(LOG_INFO, "%s: Error caught [%s].", __FUNCTION__, error.toString().c_str());
        pSession.reset();
    }

    return pSession;
}

/**
 * Handle session.
 */
void
handle_session( const int   thread_id,
                st_netfd_t  nfd )
{
    hoxClientType    clientType;
    hoxRequest_SPtr  pRequest;
    hoxResponse_SPtr pResponse;

    /* Read the 1st request. */

    hoxResult result = _read_first_request( thread_id, nfd, clientType, pRequest );
    if ( result != hoxRC_OK )
    {
        return;
    }

    /* Handle the special REGISTER request. */

    if ( pRequest->getType() == hoxREQUEST_REGISTER )
    {
        _handle_request_REGISTER( nfd, pRequest, clientType );
        return;
    }

    /* (1) Authenticate the request, and ...
     * (2) Create a Session for the client.
     */

    hoxSession_SPtr pSession = _authenticate_request( nfd, pRequest, clientType, pResponse );
    if ( ! pSession )
    {
        hoxLog(LOG_INFO, "%s: Failed to authenticate.", __FUNCTION__);
        _write_response( nfd, pResponse, clientType );
        return;  // *** Done after writing error response.
    }

    try
    {
        pSession->handleFirstRequest( pRequest );
        pSession->runEventLoop();
    }
    catch( const hoxError error )
    {
        hoxLog(LOG_INFO, "%s: Error caught [%s].", __FUNCTION__, error.toString().c_str());
    }

    /* ================================================
     * Cleanup session if required.
     * ================================================ */
    if ( pSession )
    {
        if ( pSession->getState() == hoxSESSION_STATE_SHUTDOWN
          || clientType == hoxCLIENT_TYPE_TEST )
        {
            hoxLog(LOG_DEBUG, "%s: Delete session [%s]...", __FUNCTION__, pSession->getId().c_str());
            hoxSessionMgr::getInstance()->closeAndDeleteSession( pSession );
            hoxASSERT_MSG(!pSession, "Existing session should have been deleted");
        }
    }
}

/**
 * The "session-manager" thread.
 */
void*
session_manager_thread( void* arg )
{
    for (;;)
    {
        st_sleep( SESSION_MANAGER_INTERVAL );
        hoxSessionMgr::getInstance()->manageSessions();
    }

    /* NOTREACHED */
    return NULL;
}

/**
 * The "table-manager" thread.
 */
void*
table_manager_thread( void* arg )
{
    for (;;)
    {
        st_sleep( TABLE_MANAGER_INTERVAL );
        hoxTableMgr::getInstance()->manageTables();
    }

    /* NOTREACHED */
    return NULL;
}

/******************* END OF FILE *********************************************/
