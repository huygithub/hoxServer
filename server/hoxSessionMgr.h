//
// C++ Interface: hoxSessionMgr
//
// Description: The Manager of all Sessions.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#ifndef __INCLUDED_HOX_SESSION_MGR_H__
#define __INCLUDED_HOX_SESSION_MGR_H__

#include <map>
#include "hoxSession.h"

/**
 * The Manager of all Sessions.
 * This class is implemented as a singleton.
 */
class hoxSessionMgr
{
private:
    static hoxSessionMgr* s_instance;  // The singleton instance.

    typedef std::map<std::string, hoxSession_SPtr> SessionContainer;

public:
    static hoxSessionMgr* getInstance();

public:
    ~hoxSessionMgr() {}

    hoxSession_SPtr createSession( hoxClientType  type,
                                   st_netfd_t     nfd,
                                   hoxPlayer_SPtr pPlayer );
    hoxSession_SPtr findSession( const std::string& playerId ) const;
    void postEventToAll( const hoxResponse_SPtr& pEvent,
                         const hoxSession_SPtr   pExcludedSession );
    size_t size() const { return _sessions.size(); }
                         
    /**
     * Build the content for the I_PLAYERS event.
     */
    std::string buildEvent_I_PLAYERS() const;

    /**
     * Properly close the session by doing the following:
     *  (1) Force the session to be logged out.
     *  (2) Delete it.
     *
     * @param session [IN/OUT] The session to be closed.
     *
     * @note Upon exit, the input 'session' pointer will be set to NULL
     *         (since the session itself is deleted by this API).
     */
    void closeAndDeleteSession( hoxSession_SPtr& pSession );

    void manageSessions();

private:
    hoxSessionMgr() {}

    const std::string _generateNewSessionId() const;
    void _deleteSession( hoxSession_SPtr pSession );

private:
    SessionContainer  _sessions;
};

#endif /* __INCLUDED_HOX_SESSION_MGR_H__ */
