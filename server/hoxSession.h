//
// C++ Interface: hoxSession
//
// Description: The Session of a Player.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#ifndef __INCLUDED_HOX_SESSION_H__
#define __INCLUDED_HOX_SESSION_H__

#include <boost/enable_shared_from_this.hpp>
#include <st.h>
#include "hoxTypes.h"

/**
 * A Session for a Player.
 */
class hoxSession : public boost::enable_shared_from_this<hoxSession>
{
public:
    hoxSession( const std::string& id,
                hoxSessionType     type,
                st_netfd_t         nfd,
                hoxPlayer_SPtr     pPlayer );
    virtual ~hoxSession();

    const std::string getId()     const { return _id; }
    hoxSessionType    getType()   const { return _type; }
    hoxSessionState   getState()  const { return _state; }
    hoxPlayer_SPtr    getPlayer() const { return _player; }

    const time_t getUpdateTime()  const { return _updateTime; }

    virtual bool resumeConnection( st_netfd_t nfd, hoxClientType clientType )
        { return false; }

    virtual void handleFirstRequest( const hoxRequest_SPtr& firstRequest );
    virtual void runEventLoop();

    virtual void markForShutdown();
    virtual void onDisconnected() {}
    virtual void onDeleted();  // before being deleted.

    virtual void addResponse( const hoxResponse_SPtr& response ) = 0;
    virtual void handleShutdown();
    virtual hoxResponse_SPtr getPendingEvents() = 0;

protected:
    void updateTimeStamp() { _updateTime = st_time(); }

    virtual void handleRequest( const hoxRequest_SPtr& pRequest,
                                hoxResponse_SPtr&      pResponse );

    void handle_LOGIN( const hoxRequest_SPtr& pRequest, hoxResponse_SPtr& pResponse );
    void handle_LOGOUT( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_LIST( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_NEW( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_JOIN( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_LEAVE( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_UPDATE( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_MSG( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_PING( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_MOVE( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_RESIGN( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_DRAW( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_RESET( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_INVITE( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );
    void handle_PLAYER_INFO( const hoxRequest_SPtr&  pRequest, hoxResponse_SPtr& pResponse );

    virtual void closeIO() {}
    virtual hoxResult readRequest( hoxRequest_SPtr& pRequest );
    virtual hoxResult writeResponse( const hoxResponse_SPtr& pResponse ) = 0;

protected:
    const std::string    _id;
    const hoxSessionType _type;
    hoxSessionState      _state;
    st_netfd_t           _nfd;
    hoxPlayer_SPtr       _player;

    hoxResponseSList     _responseList;
    time_t               _updateTime;  // Last update timestamp.
};

/**
 * A PERSISTENT Session for a Player.
 */
class hoxPersistentSession : public hoxSession
{
public:
    hoxPersistentSession( const std::string& id,
                          st_netfd_t         nfd,
                          hoxPlayer_SPtr     pPlayer,
                          hoxSessionType     type = hoxSESSION_TYPE_PERSISTENT );

    virtual ~hoxPersistentSession();

    virtual bool resumeConnection( st_netfd_t nfd, hoxClientType clientType );
    virtual void addResponse( const hoxResponse_SPtr& response );
    virtual void onDisconnected();
    virtual hoxResponse_SPtr getPendingEvents();

    void handleWrite();

protected:
    virtual void closeIO();
    virtual hoxResult writeResponse( const hoxResponse_SPtr& response );

private:
    st_thread_t   _readThread;
    st_thread_t   _writeThread;
    st_cond_t     _writeCond;    // Write condition-variable.
};

/**
 * A FLASH -based, PERSISTENT Session for a Player.
 */
class hoxFlashSession : public hoxPersistentSession
{
public:
    hoxFlashSession( const std::string& id,
                     st_netfd_t         nfd,
                     hoxPlayer_SPtr     pPlayer )
            : hoxPersistentSession( id, nfd, pPlayer, hoxSESSION_TYPE_FLASH ) {}

    virtual ~hoxFlashSession() {}

    virtual void handleFirstRequest( const hoxRequest_SPtr& firstRequest );

protected:
    virtual hoxResult readRequest( hoxRequest_SPtr& pRequest );
    virtual hoxResult writeResponse( const hoxResponse_SPtr& response );
};

/**
 * A POLLING Session for a Player.
 */
class hoxPollingSession : public hoxSession
{
public:
    hoxPollingSession( const std::string& id,
                       st_netfd_t         nfd,
                       hoxPlayer_SPtr     pPlayer )
            : hoxSession( id, hoxSESSION_TYPE_POLLING, nfd, pPlayer ) {}
                      
    virtual ~hoxPollingSession() {}

    virtual void addResponse( const hoxResponse_SPtr& response );
    virtual hoxResponse_SPtr getPendingEvents();

public: /* static API */
    static std::string
        build_http_response( const std::string& sResponseContent,
                             const std::string  sContentType = "text/html" );
    static void
        handle_http_GET( st_netfd_t            nfd,
                         const hoxHttpRequest& httpRequest );

protected:
    virtual void handleRequest( const hoxRequest_SPtr& pRequest,
                                hoxResponse_SPtr&      pResponse );

    virtual hoxResult readRequest( hoxRequest_SPtr& pRequest );
    virtual hoxResult writeResponse( const hoxResponse_SPtr& pResponse );
};

#endif /* __INCLUDED_HOX_SESSION_H__ */
