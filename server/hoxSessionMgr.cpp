//
// C++ Implementation: hoxSessionMgr
//
// Description: The Manager of all Sessions.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#include <sstream>
#include "hoxSessionMgr.h"
#include "hoxPlayer.h"
#include "hoxTable.h"
#include "hoxLog.h"
#include "hoxUtil.h"

/* Define the static singleton instance. */
hoxSessionMgr* hoxSessionMgr::s_instance = NULL;

/*static*/
hoxSessionMgr*
hoxSessionMgr::getInstance()
{
    if ( hoxSessionMgr::s_instance == NULL )
    {
        hoxSessionMgr::s_instance = new hoxSessionMgr();
    }
    return hoxSessionMgr::s_instance;
}

hoxSession_SPtr
hoxSessionMgr::createSession( hoxClientType  type,
                              st_netfd_t     nfd,
                              hoxPlayer_SPtr pPlayer )
{
    const char* FNAME = "hoxSessionMgr::createSession";
    hoxSession_SPtr pSession;

    hoxCHECK_MSG( pPlayer, pSession, "Player is NULL" );
    const std::string id = _generateNewSessionId();

    if ( type == hoxCLIENT_TYPE_HOXCHESS || type == hoxCLIENT_TYPE_TEST )
    {
        pSession.reset( new hoxPersistentSession( id, nfd, pPlayer ) );
    }
    else if ( type == hoxCLIENT_TYPE_FLASH )
    {
        pSession.reset( new hoxFlashSession( id, nfd, pPlayer ) );
    }
    else
    {
        pSession.reset( new hoxPollingSession( id, nfd, pPlayer ) );
    }

    if ( pSession->getState() != hoxSESSION_STATE_ACTIVE )
    {
        hoxLog(LOG_WARN, "%s: Failed to create a session.", FNAME);
        pSession.reset();
        return pSession;
    }

    pPlayer->setSession( pSession );

    /* Store the session */
    _sessions[pSession->getId()] = pSession;

    return pSession;
}

void
hoxSessionMgr::_deleteSession( hoxSession_SPtr pSession )
{
    pSession->onDeleted();
    _sessions.erase( pSession->getId() );
}

hoxSession_SPtr
hoxSessionMgr::findSession( const std::string& playerId ) const
{
    hoxSession_SPtr foundSession;
    hoxSession_SPtr pSession;  // A temporary session holder.

    // Only check for non-shutdown sessions.
    for ( SessionContainer::const_iterator it = _sessions.begin();
                                           it != _sessions.end(); ++it )
    {
        pSession = it->second;
        if (   pSession->getState() != hoxSESSION_STATE_SHUTDOWN
            && pSession->getPlayer()->getId() == playerId )
        {
            foundSession = pSession;
            break;
        }
    }

    return foundSession;
}

void
hoxSessionMgr::postEventToAll( const hoxResponse_SPtr& pEvent,
                               const hoxSession_SPtr   pExcludedSession )
{
    for ( SessionContainer::iterator it = _sessions.begin();
                                     it != _sessions.end(); ++it )
    {
        if (    pExcludedSession
             && pExcludedSession != it->second )
        {
            it->second->addResponse( pEvent );
        }
    }
}

std::string
hoxSessionMgr::buildEvent_I_PLAYERS() const
{
    std::ostringstream outStream;
    hoxPlayer_SPtr     player;

    for ( SessionContainer::const_iterator it = _sessions.begin();
                                           it != _sessions.end(); ++it )
    {
        player = it->second->getPlayer();
        outStream << player->getId() << ";"
                  << player->getScore()
                  << "\n";
    }

    return outStream.str();
}

void
hoxSessionMgr::closeAndDeleteSession( hoxSession_SPtr& pSession )
{
    const char* FNAME = "hoxSessionMgr::closeAndDeleteSession";
    hoxLog(LOG_INFO, "%s: ENTER. [%s].", FNAME, pSession->getId().c_str());

    pSession->handleShutdown();

    hoxTableMgr::getInstance()->runCleanup();
    _deleteSession( pSession );

    pSession.reset(); // Finally, invalidate the pointer.
}

void
hoxSessionMgr::manageSessions()
{
    const char* FNAME = "hoxSessionMgr::manageSessions";
    hoxSession_SPtr pSession;

    /* To avoid race condition..., delete the expired sessions
     * that were detected in the last run of this function.
     *
     * NOTE: The loop below, we remove items from a STD container within a loop.
     *       Here is a discussion about such a usage:
     *          http://www.codeguru.com/forum/printthread.php?t=446029
     */
    for ( SessionContainer::iterator it = _sessions.begin();
                                     it != _sessions.end(); /* empty */ )
    {
        pSession = it->second;
        if ( pSession->getState() == hoxSESSION_STATE_SHUTDOWN )
        {
            hoxLog(LOG_INFO, "%s: Purged expired session [%s].", FNAME, pSession->getId().c_str());
            pSession->onDeleted();
            _sessions.erase( it++ );
        }
        else
        {
           ++it;
        }
    }

    /* Check for expired sessions. */
    const time_t now = st_time();
    time_t       elapseTime;
    for ( SessionContainer::iterator it = _sessions.begin();
                                     it != _sessions.end(); ++it )
    {
        pSession = it->second;
        elapseTime = now - pSession->getUpdateTime();
        if ( elapseTime > SESSION_EXPIRY )
        {
            hoxLog(LOG_INFO, "%s: Found expired session [%s].", FNAME, pSession->getId().c_str());
            pSession->handleShutdown();
        }
    }
}

const std::string
hoxSessionMgr::_generateNewSessionId() const
{
    const unsigned int MAX_SESSION_ID = 1000000;
    std::string sNewId;

    for (;;)
    {
        const int randNum = hoxUtil::generateRandomNumber( MAX_SESSION_ID );
        sNewId = hoxUtil::intToString( randNum );

        if ( _sessions.find( sNewId ) == _sessions.end() ) // not yet used?
        {
            break;  // Good. We will use this new ID.
        }
    }

    return sNewId;
}

/******************* END OF FILE *********************************************/
