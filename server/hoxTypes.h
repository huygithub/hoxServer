//
// C++ Interface: hoxTypes
//
// Description: Containing simple types commonly used through out the project.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#ifndef __INCLUDED_HOX_TYPES_H__
#define __INCLUDED_HOX_TYPES_H__

#include <string>
#include <list>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include <st.h>
#include "hoxEnums.h"
#include "hoxDebug.h"

/* Forward declarations */
class hoxPlayer;
class hoxSession;
class hoxTable;
class hoxResponse;
class hoxTimeInfo;

/**
 * Typedef(s)
 */
typedef boost::shared_ptr<hoxPlayer>  hoxPlayer_SPtr;
typedef boost::shared_ptr<hoxSession> hoxSession_SPtr;
typedef std::list<hoxPlayer_SPtr>     hoxPlayerList;

typedef boost::shared_ptr<hoxTable> hoxTable_SPtr;
typedef std::list<hoxTable_SPtr> hoxTableList;
typedef std::set<hoxTable_SPtr>  hoxTableSet;

typedef boost::shared_ptr<hoxResponse> hoxResponse_SPtr;
typedef std::list<hoxResponse_SPtr> hoxResponseSList;

typedef std::list<std::string> hoxStringList;

/**
 * Container for parameters.
 */
typedef std::map<const std::string, std::string> hoxParameters;

/**
 * Request comming from the remote Players.
 */
class hoxRequest
{
public:
    hoxRequest(hoxRequestType type = hoxREQUEST_UNKNOWN);
    hoxRequest(const std::string& requestStr);

    bool isValid() const { return _type != hoxREQUEST_UNKNOWN; }

    const hoxRequestType getType() const { return _type; }
    const hoxParameters& getParameters() const { return _parameters; }
    const std::string getParam(const std::string& key) const
        { return const_cast<hoxRequest*>(this)->_parameters[key]; }

    void setParam(const std::string& key, const std::string& value)
        { _parameters[key] = value; }

    const std::string toString() const;

private:
    hoxRequestType  _type;
    hoxParameters   _parameters;
};
typedef boost::shared_ptr<hoxRequest> hoxRequest_SPtr;
typedef std::list<hoxRequest_SPtr>    hoxRequestSList;

/**
 * Response being returned to the remote Players.
 */
class hoxResponse
{
public:
    explicit hoxResponse( hoxRequestType type,
                          hoxResult      code = hoxRC_OK );
    hoxResponse( const hoxResponse& other ); // Copy constructor.
    ~hoxResponse() {}

    void setCode(hoxResult code) { _code = code; }
    void setTid(const std::string& tid) { _tid = tid; } 
    void setContent(const std::string& content) { _content = content; }

    const std::string getContent() const { return _content; }

    const std::string toString(bool bMore = false) const;

    /* ---------- */
    /* Static API */
    /* ---------- */
public:
    static hoxResponse_SPtr
    create_event_LOGIN( const std::string& playerId,
                        const int          nPlayerScore,
                        const std::string& sessionId = "" );

    static hoxResponse_SPtr
    create_event_LOGOUT( const hoxPlayer_SPtr player );

    static hoxResponse_SPtr
    create_event_INVITE( const std::string& sInviterId,
                         const int          nInviterScore,
                         const std::string& sInviteeId,
                         const std::string& tableId );

    static hoxResponse_SPtr
    create_event_PLAYER_INFO( const hoxPlayer_SPtr player,
                              const std::string&   tableId );

    static hoxResponse_SPtr
    create_event_I_PLAYERS( const std::string& sEventContent );

    static hoxResponse_SPtr
    create_event_I_TABLE( const hoxTable*  pTable );

    static hoxResponse_SPtr
    create_event_LIST( const hoxTableList& tables );

    static hoxResponse_SPtr
    create_event_E_JOIN( const hoxTable*  pTable,
                         const hoxPlayer_SPtr player,
                         hoxColor         color );

    static hoxResponse_SPtr
    create_event_LEAVE( const hoxTable*  pTable,
                        const hoxPlayer_SPtr player );

    static hoxResponse_SPtr
    create_event_MSG( const hoxPlayer_SPtr player,
                      const std::string&   message,
                      const std::string&   tableId = "" );

    static hoxResponse_SPtr
    create_event_PING();

    static hoxResponse_SPtr
    create_event_MOVE( const hoxTable*    pTable,
                       const hoxPlayer_SPtr player,
                       const std::string& sMove,
                       hoxGameStatus      gameStatus );

    static hoxResponse_SPtr
    create_event_DRAW( hoxResult          code,
                       const hoxTable*    pTable,
                       const hoxPlayer_SPtr player );

    static hoxResponse_SPtr
    create_event_END( const hoxTable*     pTable,
                      const hoxGameStatus gameStatus,
                      const std::string&  sReason );

    static hoxResponse_SPtr
    create_event_RESET( const hoxTable* pTable );

    static hoxResponse_SPtr
    create_event_E_SCORE( const hoxTable*   pTable,
                          const hoxPlayer_SPtr player );

    static hoxResponse_SPtr
    create_event_I_MOVES( const hoxTable*      pTable,
                          const hoxStringList& moves );

    static hoxResponse_SPtr
    create_event_UPDATE( const hoxTable*    pTable,
                         const hoxPlayer_SPtr player,
                         const hoxGameType  newGameType,
                         const hoxTimeInfo& newInitialTime );

    static hoxResponse_SPtr
    create_event_POLL( const hoxResponseSList& responseList );

private:
    const hoxRequestType  _type;
    hoxResult             _code;
    std::string           _tid;   // Table-Id (if applicable).
    std::string           _content;
};

/**
 * Game's Time-info.
 */
class hoxTimeInfo
{
  public:
    int  nGame;  // Game-time.
    int  nMove;  // Move-time.
    int  nFree;  // Free-time.

    hoxTimeInfo() : nGame(0), nMove(0), nFree(0) {}
    void Clear() { nGame = nMove = nFree = 0; }
    bool IsEmpty() const
        { return (nGame == 0) && (nMove == 0) && (nFree == 0); }
};

/**
 * HTTP Request.
 *
 * TODO: We should reconsider the design of this struct!
 */
class hoxHttpRequest
{
public:
    std::string    method;   // GET, POST
    std::string    path;     // The resource-path.
    hoxParameters  params;   // URI parameters.
    hoxParameters  headers;  // HTTP headers.
    std::string    body;     // The body.

    void parseURI( const std::string& sURI );
    hoxResult continueReadRequest( st_netfd_t fd );
};

#endif /* __INCLUDED_HOX_TYPES_H__ */
