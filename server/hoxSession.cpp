//
// C++ Implementation: hoxSession
//
// Description: The Session of a Player.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#include "hoxSession.h"
#include "hoxSessionMgr.h"
#include "hoxPlayer.h"
#include "hoxTable.h"
#include "hoxLog.h"
#include "hoxSocketAPI.h"
#include "hoxExcept.h"
#include "hoxUtil.h"
#include "hoxFileMgr.h"
#include <sstream>

// =========================================================================
//
//                        hoxSession
//
// =========================================================================

hoxSession::hoxSession( const std::string& id,
                        hoxSessionType     type,
                        st_netfd_t         nfd,
                        hoxPlayer_SPtr     pPlayer )
        : _id( id )
        , _type( type )
        , _state( hoxSESSION_STATE_ACTIVE )
        , _nfd( nfd )
        , _player( pPlayer )
{
    const char* FNAME = "hoxSession::hoxSession";
    hoxLog(LOG_INFO, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());
    this->updateTimeStamp();
}

hoxSession::~hoxSession()
{
    const char* FNAME = "hoxSession::~hoxSession";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());
}

void
hoxSession::handleFirstRequest( const hoxRequest_SPtr& firstRequest )
{
    const char* FNAME = "hoxSession::handleFirstRequest";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    hoxResponse_SPtr pResponse;

    this->handleRequest( firstRequest, pResponse );
    if ( pResponse )
    {
        if ( hoxRC_OK != this->writeResponse( pResponse ) )
        {
            this->onDisconnected();
            throw hoxError(hoxRC_ERR, "Failed to write outgoing response");
        }
    }
}

void
hoxSession::runEventLoop()
{
    const char* FNAME = "hoxSession::runEventLoop";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    while ( _state == hoxSESSION_STATE_ACTIVE )
    {
        hoxRequest_SPtr pRequest;
        if ( hoxRC_OK != this->readRequest( pRequest ) )
        {
            this->onDisconnected();
            break;
        }

        hoxResponse_SPtr pResponse;
        this->handleRequest( pRequest, pResponse );
        if ( pResponse )
        {
            (void) this->writeResponse( pResponse );
        }

        st_sleep( ST_UTIME_NO_WAIT ); // Yield so that others can run.
    }
}

hoxResult
hoxSession::readRequest( hoxRequest_SPtr& pRequest )
{
    const char* FNAME = "hoxSession::readRequest";
    hoxResult   result;
    std::string sRequest;

    result = hoxSocketAPI::read_line( _nfd, sRequest, PERSIST_READ_TIMEOUT );
    if ( result == hoxRC_EINTR ) /* Check for interrupt condition. */
    {
        hoxLog(LOG_INFO, "%s: (%s:%s) Empty request due to thread-interrupt.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return hoxRC_EINTR;
    }
    else if ( sRequest.empty() ) /* Check for socket-close condition. */
    {
        hoxLog(LOG_DEBUG, "%s: (%s:%s) Empty request due to socket-close.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return hoxRC_CLOSED;
    }
    else if ( result != hoxRC_OK )
    {
        hoxLog(LOG_INFO, "%s: (%s:%s) Failed to read request (rc=%d).",
            FNAME, _id.c_str(), _player->getId().c_str(), result);
        return hoxRC_ERR;
    }

    hoxLog(LOG_DEBUG, "%s: (%s:%s) Request: [%s]",
        FNAME, _id.c_str(), _player->getId().c_str(), sRequest.c_str());

    pRequest.reset( new hoxRequest( sRequest ) );
    if ( ! pRequest->isValid() )
    {
        hoxLog(LOG_INFO, "%s: Request [%s] is invalid.", FNAME, sRequest.c_str());
        return hoxRC_NOT_VALID;
    }

    return hoxRC_OK;
}

void
hoxSession::markForShutdown()
{
    const char* FNAME = "hoxSession::markForShutdown";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());
    _state = hoxSESSION_STATE_SHUTDOWN;
}

void
hoxSession::onDeleted()
{
    const char* FNAME = "hoxSession::onDeleted";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    _player->clearSession();
}

void
hoxSession::handleShutdown()
{
    const char* FNAME = "hoxSession::handleShutdown";
    hoxLog(LOG_INFO, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    _player->leaveAllTables();
    _state = hoxSESSION_STATE_SHUTDOWN;

    /* Inform others about the event. */
    hoxResponse_SPtr pResponse = hoxResponse::create_event_LOGOUT( _player );
    hoxSessionMgr::getInstance()->postEventToAll( pResponse,
                                                  shared_from_this() /* excluded */ );

    if ( _nfd != NULL )
    {
        this->closeIO();
    }
}

void
hoxSession::handleRequest( const hoxRequest_SPtr& pRequest,
                           hoxResponse_SPtr&      pResponse )
{
    const char* FNAME = "hoxSession::handleRequest";

    this->updateTimeStamp();  // Keep the session alive.

    const hoxRequestType requestType = pRequest->getType();
    try
    {
        switch( requestType )
        {
            case hoxREQUEST_LOGIN:  handle_LOGIN( pRequest, pResponse ); break;
            case hoxREQUEST_LOGOUT: handle_LOGOUT( pRequest, pResponse ); break;
            case hoxREQUEST_LIST:   handle_LIST( pRequest, pResponse ); break;
            case hoxREQUEST_NEW:    handle_NEW( pRequest, pResponse ); break;
            case hoxREQUEST_JOIN:   handle_JOIN( pRequest, pResponse ); break;
            case hoxREQUEST_LEAVE:  handle_LEAVE( pRequest, pResponse ); break;
            case hoxREQUEST_UPDATE: handle_UPDATE( pRequest, pResponse ); break;
            case hoxREQUEST_MSG:    handle_MSG( pRequest, pResponse ); break;
            case hoxREQUEST_PING:   handle_PING( pRequest, pResponse ); break;
            case hoxREQUEST_MOVE:   handle_MOVE( pRequest, pResponse ); break;
            case hoxREQUEST_RESIGN: handle_RESIGN( pRequest, pResponse ); break;
            case hoxREQUEST_DRAW:   handle_DRAW( pRequest, pResponse ); break;
            case hoxREQUEST_RESET:  handle_RESET( pRequest, pResponse ); break;
            case hoxREQUEST_INVITE: handle_INVITE( pRequest, pResponse ); break;
            case hoxREQUEST_PLAYER_INFO: handle_PLAYER_INFO( pRequest, pResponse ); break;
            case hoxREQUEST_POLL:  /* Handle this request below... */ break;

            default: throw hoxError(hoxRC_NOT_SUPPORTED, "Unsupported Request");
        }
    }
    catch( hoxTableError error )
    {
        pResponse.reset( new hoxResponse( requestType, error.code() ) );
        pResponse->setTid( error.tid() );
        pResponse->setContent( error.what() + std::string("\n") );
        hoxLog(LOG_WARN, "%s: Table-Error caught [%s].", FNAME, error.toString().c_str());
    }
    catch( hoxError error )
    {
        pResponse.reset( new hoxResponse( requestType, error.code() ) );
        pResponse->setContent( error.what() + std::string("\n") );
        hoxLog(LOG_WARN, "%s: Error caught [%s].", FNAME, error.toString().c_str());
    }

    /* Write a response if any (some requests do not have any response). */
    if ( pResponse )
    {
        this->addResponse( pResponse );
        pResponse.reset();
    }
}

void
hoxSession::handle_LOGIN( const hoxRequest_SPtr& pRequest,
                          hoxResponse_SPtr&      pResponse )
{
    hoxPlayer_SPtr player = this->getPlayer();

    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * Since session-ID is secret to the Player, only the Player
     * should know about the ID.
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */

    // ... Notify others about the new online Player.
    hoxResponse_SPtr pResponseWithoutID =
        hoxResponse::create_event_LOGIN( player->getId(), player->getScore() );
    hoxSessionMgr::getInstance()->postEventToAll( pResponseWithoutID,
                                                  shared_from_this() /* excluded */ );

    // ... Only the Player knows his session-ID.
    pResponse = hoxResponse::create_event_LOGIN( player->getId(), player->getScore(),
                                                 this->getId() );
    player->onNewEvent( pResponse );

    // Inform the Player about the list of existing online Players.
    const std::string sEventContent = hoxSessionMgr::getInstance()->buildEvent_I_PLAYERS();
    pResponse = hoxResponse::create_event_I_PLAYERS( sEventContent );
    player->onNewEvent( pResponse );

    // If the Player was disconnected, attempt to resume playing.
    player->resumePlayingIfNeeded();

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_LOGOUT( const hoxRequest_SPtr&  pRequest,
                           hoxResponse_SPtr&       pResponse )
{
    this->markForShutdown();
}

void
hoxSession::handle_LIST( const hoxRequest_SPtr&  pRequest,
                         hoxResponse_SPtr&       pResponse )
{
    hoxTableList tables;
    hoxTableMgr::getInstance()->getTables( tables );

    pResponse = hoxResponse::create_event_LIST( tables );
}

void
hoxSession::handle_NEW( const hoxRequest_SPtr&  pRequest,
                        hoxResponse_SPtr&       pResponse )
{
    std::ostringstream outStream;

    hoxPlayer_SPtr player = this->getPlayer();

    const std::string itimes = pRequest->getParam("itimes");
    const hoxTimeInfo initialTime = hoxUtil::stringToTimeInfo( itimes );
    const std::string sRequestColor = pRequest->getParam("color");

    /* Get the requested color (Red, Black, or Observer). */

    hoxColor requestColor = hoxUtil::stringToColor( sRequestColor );
    if ( requestColor == hoxCOLOR_UNKNOWN )
        requestColor = hoxCOLOR_RED;  // Default: Play RED

    hoxTable_SPtr pNewTable =
        hoxTableMgr::getInstance()->createTable( initialTime );
    if ( ! pNewTable )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Table cannot be created");
    }

    player->joinTableAs( pNewTable, requestColor );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_JOIN( const hoxRequest_SPtr&  pRequest,
                         hoxResponse_SPtr&       pResponse )
{
    hoxTable_SPtr    pTable;

    hoxPlayer_SPtr player = this->getPlayer();

    const std::string tableId = pRequest->getParam("tid");
    const std::string sRequestColor = pRequest->getParam("color");
    hoxColor          requestColor = hoxCOLOR_UNKNOWN;

    /* Get the requested color (Red, Black, or Observer). */

    requestColor = hoxUtil::stringToColor( sRequestColor );
    if ( requestColor == hoxCOLOR_UNKNOWN )
    {
        requestColor = hoxCOLOR_NONE;  // Default: Observer
    }

    /* Find the Table. */

    pTable = hoxTableMgr::getInstance()->findTable(tableId);
    if ( ! pTable )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Table not found");
    }

    player->joinTableAs( pTable, requestColor );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_LEAVE( const hoxRequest_SPtr&  pRequest,
                          hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");

    hoxTable_SPtr pTable = hoxTableMgr::getInstance()->findTable(tableId);
    if ( ! pTable )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Table not found");
    }

    _player->leaveTable( pTable );

    /* Trigger the table-clean routine. */
    hoxTableMgr::getInstance()->runCleanup();

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_UPDATE( const hoxRequest_SPtr&  pRequest,
                           hoxResponse_SPtr&       pResponse )
{
    hoxPlayer_SPtr player = this->getPlayer();

    const std::string tableId = pRequest->getParam("tid");
    const bool bRatedGame = ( pRequest->getParam("rated") == "1" );
    const hoxTimeInfo initialTime = 
        hoxUtil::stringToTimeInfo( pRequest->getParam("itimes") );

    hoxResult result = player->updateTable( tableId, 
                                            bRatedGame, initialTime );
    if ( result != hoxRC_OK )
    {
        throw hoxError(hoxRC_ERR, "Update (Table) not OK");
    }

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_MSG( const hoxRequest_SPtr&  pRequest,
                        hoxResponse_SPtr&       pResponse )
{
    hoxPlayer_SPtr player = this->getPlayer();

    /* There are two cases:
     *  (1) The message targets a given Table (all players in it).
     *      We call the message a "table" message.
     *  (2) The message targets only a specific player.
     *      We call the message a "private" message.
     */
    const std::string tableId = pRequest->getParam("tid");
    const std::string otherId = pRequest->getParam("oid");
    const std::string message = pRequest->getParam("msg");

    const hoxResponse_SPtr pMessage = 
        hoxResponse::create_event_MSG( player, message, tableId );

    /* Case (1): "table" message. */

    if ( ! tableId.empty() )
    {
        hoxTable_SPtr pTable = hoxTableMgr::getInstance()->findTable(tableId);
        if ( ! pTable )
        {
            throw hoxError(hoxRC_NOT_FOUND, "Table not found");
        }
        pTable->onMessage_FromPlayer( player, pMessage );
    }
    /* Case (2): "private" message. */
    else
    {
        hoxSession_SPtr foundSession =
            hoxSessionMgr::getInstance()->findSession( otherId );
        if ( ! foundSession )
        {
            throw hoxError(hoxRC_NOT_FOUND, "Player not found");
        }
        hoxPlayer_SPtr otherPlayer = foundSession->getPlayer();
        otherPlayer->onNewEvent( pMessage );
    }

    /* For the message sender, return nothing. */
    pResponse.reset();
}

void
hoxSession::handle_PING( const hoxRequest_SPtr&  pRequest,
                         hoxResponse_SPtr&       pResponse )
{
    pResponse = hoxResponse::create_event_PING();
}

void
hoxSession::handle_MOVE( const hoxRequest_SPtr&  pRequest,
                         hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");
    const std::string sMove   = pRequest->getParam("move");

    _player->doMove( tableId, sMove );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_RESIGN( const hoxRequest_SPtr&  pRequest,
                           hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");
    _player->offerResign( tableId );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_DRAW( const hoxRequest_SPtr&  pRequest,
                         hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");
    _player->offerDraw( tableId );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_RESET( const hoxRequest_SPtr&  pRequest,
                          hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");
    _player->resetTable( tableId );

    pResponse.reset();  // Return "nothing".
}

void
hoxSession::handle_INVITE( const hoxRequest_SPtr&  pRequest,
                           hoxResponse_SPtr&       pResponse )
{
    hoxPlayer_SPtr player = this->getPlayer();

    const std::string sInviterId    = player->getId();
    const int         nInviterScore = player->getScore();
    const std::string sInviteeId = pRequest->getParam("oid");
    const std::string tableId    = pRequest->getParam("tid"); // OPTIONAL!

    /* Lookup the invitee using session-manager.
     * NOTE: Currently, the server supports inviting "online" players.
     */

    hoxSession_SPtr foundSession = 
        hoxSessionMgr::getInstance()->findSession( sInviteeId );
    if ( ! foundSession )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Invitee not found");
    }

    hoxPlayer_SPtr inviteePlayer = foundSession->getPlayer();

    /* Simply forward the request to the invitee. */
    hoxResponse_SPtr pInvitation =
        hoxResponse::create_event_INVITE( sInviterId, nInviterScore,
                                          sInviteeId, tableId );
    inviteePlayer->onNewEvent( pInvitation );

    /* For the inviter, return nothing. */
    pResponse.reset();
}

void
hoxSession::handle_PLAYER_INFO( const hoxRequest_SPtr&  pRequest,
                                hoxResponse_SPtr&       pResponse )
{
    const std::string tableId = pRequest->getParam("tid");
    const std::string infoPlayerId = pRequest->getParam("oid");

    /* Lookup the "info" Player using session-manager.
     * NOTE: Currently, the server supports looking up "online" players.
     */

    hoxSession_SPtr foundSession = 
        hoxSessionMgr::getInstance()->findSession( infoPlayerId );
    if ( ! foundSession )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Player not found");
    }

    hoxPlayer_SPtr infoPlayer = foundSession->getPlayer();

    /* Return:
     *       The requested Player's Info.
     */

    pResponse = hoxResponse::create_event_PLAYER_INFO( infoPlayer,
                                                       tableId );
}

// =========================================================================
//
//                        hoxPersistentSession
//
// =========================================================================

/**
 * Handle write Thread.
 */
void*
_handle_write( void *arg )
{
    hoxPersistentSession* pSession = (hoxPersistentSession*) arg;
    pSession->handleWrite();
    return NULL;
}

hoxPersistentSession::hoxPersistentSession( const std::string& id,
                                            st_netfd_t         nfd,
                                            hoxPlayer_SPtr     pPlayer,
                                            hoxSessionType     type /* = hoxSESSION_TYPE_PERSISTENT */ )
        : hoxSession( id,
                      type,
                      nfd,
                      pPlayer )
        , _readThread( NULL )
        , _writeThread( NULL )
        , _writeCond( NULL )
{
    _readThread = st_thread_self();
    _writeCond = st_cond_new();

    _writeThread = st_thread_create( _handle_write, (void *) this,
                                     1 /* joinable */, 0 /* stack-size */ );
    if ( _writeThread == NULL )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to create WRITE thread.", __FUNCTION__);
        _state = hoxSESSION_STATE_SHUTDOWN;  // Signal a "bad" session.
        return;
    }
}

hoxPersistentSession::~hoxPersistentSession()
{
    st_cond_destroy( _writeCond );
}

bool
hoxPersistentSession::resumeConnection( st_netfd_t    nfd,
                                        hoxClientType clientType )
{
    const char* FNAME = "hoxPersistentSession::resumeConnection";

    const bool okToResume = ( clientType == hoxCLIENT_TYPE_HOXCHESS );
    if ( ! okToResume )
    {
        return false;
    }

    // Close the existing connection, if any.
    if ( _nfd != NULL )
    {
        hoxLog(LOG_INFO, "%s: (%s:%s) Force to close existing connection.", FNAME, _id.c_str(), _player->getId().c_str());
        _state = hoxSESSION_STATE_DISCONNECT;
        if (_readThread != NULL)
        {
            hoxLog(LOG_INFO, "%s: (%s:%s) Interrupt the READ thread...", FNAME, _id.c_str(), _player->getId().c_str());
            st_thread_interrupt( _readThread );
            // NOTE: We cannot use "st_thread_join()" on the READ write because
            //       it will lead to deadlock with two threads (READ and WRITE)
            //       waiting for each other.
            const int maxWaitTimes = 5; // in seconds.
            for (int i = 0; i < maxWaitTimes; ++i)
            {
                st_sleep(1);  // Wait 1 second at a time.
                if (_readThread == NULL) break;
            }
            hoxLog(LOG_INFO, "%s: (%s:%s) Done closing the READ thread (closed=%d)",
                FNAME, _id.c_str(), _player->getId().c_str(), (_readThread == NULL ? 1 : 0));
        }
        this->closeIO();
    }

    _nfd = nfd;
    _readThread = st_thread_self();

    _responseList.clear(); // NOTE: Remove old events.
    _state = hoxSESSION_STATE_ACTIVE;

    _writeThread = st_thread_create( _handle_write, (void *) this,
                                    1 /* joinable */, 0 /* stack-size */ );
    if ( _writeThread == NULL )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to create WRITE thread.", __FUNCTION__);
        _state = hoxSESSION_STATE_SHUTDOWN;  // Signal a "bad" session.
        return false;
    }

    return true;
}

void
hoxPersistentSession::addResponse( const hoxResponse_SPtr& response )
{
    if ( _state == hoxSESSION_STATE_ACTIVE )
    {
        _responseList.push_back( response );  // Make a copy...
    }
    st_cond_signal( _writeCond );
}

void
hoxPersistentSession::handleWrite()
{
    const char* FNAME = "hoxPersistentSession::handleWrite";

    while ( _state == hoxSESSION_STATE_ACTIVE )
    {
        if ( _responseList.empty() )
        {
            st_cond_wait( _writeCond );
            continue;    // NOTE: Double-check one more time.
        }

        hoxResponse_SPtr response( _responseList.front() );
        _responseList.pop_front();
        if ( hoxRC_OK != this->writeResponse( response ) )
        {
            continue;  // NOTE: Still allow to continue.
        }

        /* Keep alive to avoid being disconnected if the Player is only
         * observing the game(s).
         */
        this->updateTimeStamp();

        st_sleep( ST_UTIME_NO_WAIT ); // yield so that others can run.
    }

    /* Cleanup the WRITE connection. */
    _writeThread = NULL;
    hoxLog(LOG_DEBUG, "%s: (%s) Closed WRITE connection.", FNAME, _id.c_str());
}

hoxResult
hoxPersistentSession::writeResponse( const hoxResponse_SPtr& response )
{
    const std::string sResp = response->toString();
    return hoxSocketAPI::write_data( _nfd, sResp );
}

void
hoxPersistentSession::onDisconnected()
{
    const char* FNAME = "hoxPersistentSession::onDisconnected";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    _state = hoxSESSION_STATE_DISCONNECT;
    this->closeIO();
}

void
hoxPersistentSession::closeIO()
{
    const char* FNAME = "hoxPersistentSession::closeIO";

    hoxLog(LOG_DEBUG, "%s: Closing READ connection...", FNAME);
    _readThread = NULL;

    if ( _writeThread != NULL )
    {
        st_cond_signal( _writeCond );
        if ( 0 != st_thread_join( _writeThread, NULL ) )
        {
            hoxLog(LOG_SYS_WARN, "%s: Failed waiting for WRITE thread to end.", FNAME);
        }
    }

    _nfd = NULL;
}

hoxResponse_SPtr
hoxPersistentSession::getPendingEvents()
{
    hoxResponse_SPtr pResponse;  // No event!
    return pResponse;
}


// =========================================================================
//
//                        hoxFlashSession
//
// =========================================================================

void
hoxFlashSession::handleFirstRequest( const hoxRequest_SPtr& firstRequest )
{
    const char* FNAME = "hoxFlashSession::handleFirstRequest";
    hoxResult result;

    /* Consume the NULL terminator. */
    std::string sData;
    result = hoxSocketAPI::read_nbytes( _nfd, 1, sData /* NO TIME OUT */ );
    if ( result != hoxRC_OK
         || sData.size() != 1 || sData.at(0) != '\0' )
    {
        hoxLog(LOG_INFO, "%s: (%s:%s) NULL terminator not found.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return;
    }

    hoxSession::handleFirstRequest( firstRequest );
}

hoxResult
hoxFlashSession::readRequest( hoxRequest_SPtr& pRequest )
{
    const char* FNAME = "hoxFlashSession::readRequest";
    hoxResult result = hoxSession::readRequest( pRequest );
    if ( result != hoxRC_OK )
    {
        return result;
    }

    /* Consume the NULL terminator. */
    std::string sData;
    result = hoxSocketAPI::read_nbytes( _nfd, 1, sData /* NO TIME OUT */ );
    if ( result != hoxRC_OK
         || sData.size() != 1 || sData.at(0) != '\0' )
    {
        hoxLog(LOG_DEBUG, "%s: (%s:%s) NULL terminator not found.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return hoxRC_CLOSED;
    }

    return hoxRC_OK;
}

hoxResult
hoxFlashSession::writeResponse( const hoxResponse_SPtr& response )
{
    std::string sResp = response->toString();
    sResp += '\0';

    return hoxSocketAPI::write_data( _nfd, sResp );
}


// =========================================================================
//
//                        hoxPollingSession
//
// =========================================================================

void
hoxPollingSession::addResponse( const hoxResponse_SPtr& response )
{
    _responseList.push_back( response );  // Make a copy...
}

hoxResponse_SPtr
hoxPollingSession::getPendingEvents()
{
    hoxResponse_SPtr pResponse = hoxResponse::create_event_POLL( _responseList );
    _responseList.clear();
    return pResponse;
}

void
hoxPollingSession::handleRequest( const hoxRequest_SPtr& pRequest,
                                  hoxResponse_SPtr&      pResponse )
{
    hoxSession::handleRequest( pRequest, pResponse );

    /* For HTTP requests, return pending events now. */
    pResponse = this->getPendingEvents();
}

hoxResult
hoxPollingSession::readRequest( hoxRequest_SPtr& pRequest )
{
    const char* FNAME = "hoxPollingSession::readRequest";
    hoxResult   result;
    std::string sRequest;

    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    result = hoxSocketAPI::read_line( _nfd, sRequest, PERSIST_READ_TIMEOUT );
    if ( sRequest.empty() ) /* Check for socket-close condition. */
    {
        hoxLog(LOG_DEBUG, "%s: (%s:%s) Empty request due to socket-close.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return hoxRC_CLOSED;
    }
    else if ( result != hoxRC_OK )
    {
        hoxLog(LOG_INFO, "%s: (%s:%s) Failed to read request.",
            FNAME, _id.c_str(), _player->getId().c_str());
        return hoxRC_ERR;
    }

    hoxLog(LOG_DEBUG, "%s: (%s:%s) Request: [%s]",
        FNAME, _id.c_str(), _player->getId().c_str(), sRequest.c_str());

    /* Check if this is a HTTP (GET/POST) request. */

    hoxHttpRequest httpRequest;
    
    if ( sRequest.find("GET") == 0 || sRequest.find("POST") == 0 )
    {
        httpRequest.parseURI( sRequest );
        result = httpRequest.continueReadRequest( _nfd );
        if ( result != hoxRC_OK )
        {
            return result;
        }

        if ( httpRequest.method == "GET" )
        {
            hoxPollingSession::handle_http_GET( _nfd, httpRequest );
            return hoxRC_HANDLED;
        }
        else // ... POST method, the 'real' request is in the body.
        {
            sRequest = httpRequest.body;
            hoxUtil::trimLast( sRequest, '\n' );
        }
    }

    /* Parse the request. */
    pRequest.reset( new hoxRequest( sRequest ) );
    if ( ! pRequest->isValid() )
    {
        hoxLog(LOG_INFO, "%s: Request [%s] is invalid.", FNAME, sRequest.c_str());
        return hoxRC_NOT_VALID;
    }

    return hoxRC_OK;
}

hoxResult
hoxPollingSession::writeResponse( const hoxResponse_SPtr& response )
{
    const char* FNAME = "hoxPollingSession::writeResponse";
    hoxLog(LOG_DEBUG, "%s: (%s:%s) ENTER.", FNAME, _id.c_str(), _player->getId().c_str());

    const std::string sResp =
        hoxPollingSession::build_http_response( response->toString() );
    return hoxSocketAPI::write_data( _nfd, sResp );
}

/**
 * Build the HTTP response to send back to the client.
 */
std::string
hoxPollingSession::build_http_response( const std::string& sResponseContent,
                                        const std::string  sContentType /* = "text/html" */ )
{
    std::ostringstream outStream;
    outStream << "HTTP/1.1 200 OK" << "\r\n"
              << "Date: " << hoxUtil::getHttpDate() << "\r\n"
              << "Server: games.playxiangqi.com v1.0" << "\r\n"
              << "Content-Length: " << sResponseContent.size() << "\r\n"
              << "Content-Type: " << sContentType << "; charset=UTF-8" << "\r\n"
              //<< "Connection: close" << "\r\n"
              << "\r\n"
              << sResponseContent;

    return outStream.str();
}

void
hoxPollingSession::handle_http_GET( st_netfd_t            nfd,
                                    const hoxHttpRequest& httpRequest )
{
    hoxFile_SPtr pFile = hoxFileMgr::getInstance()->getFile( httpRequest.path );
    hoxASSERT_MSG( pFile, "File pointer must have been set");

    const std::string sResp =
        hoxPollingSession::build_http_response( pFile->m_sContent, pFile->m_sType );
    (void) hoxSocketAPI::write_data( nfd, sResp );
}

/******************* END OF FILE *********************************************/
