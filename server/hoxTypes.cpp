//
// C++ Implementation: hoxTypes
//
// Description: Containing simple types commonly used through out the project.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#include <sstream>
#include <iostream>
#include <boost/tokenizer.hpp>
#include "hoxTypes.h"
#include "hoxUtil.h"
#include "hoxTable.h"
#include "hoxSocketAPI.h"

// =========================================================================
//
//                        hoxRequest class
//
// =========================================================================

hoxRequest::hoxRequest( hoxRequestType type /* = hoxREQUEST_UNKNOWN */ )
        : _type( type )
{
}

hoxRequest::hoxRequest( const std::string& requestStr )
        : _type( hoxREQUEST_UNKNOWN )
{
    hoxUtil::parse_network_message( requestStr,
                                    _type,
                                    _parameters );
}

const std::string
hoxRequest::toString() const
{
    std::string result;

    result = "op=" + hoxUtil::requestTypeToString( _type );

    for ( hoxParameters::const_iterator it = _parameters.begin();
                                        it != _parameters.end(); ++it )
    {
        result += "&" + it->first + "=" + it->second;
    }

    return result;
}

// =========================================================================
//
//                        hoxResponse class
//
// =========================================================================

hoxResponse::hoxResponse( hoxRequestType type,
                          hoxResult      code /* = hoxRC_OK */ )
        : _type( type )
        , _code( code )
{
}

hoxResponse::hoxResponse( const hoxResponse& other )
        : _type( other._type )
        , _code( other._code )
{
    _content = other._content;
}

const std::string
hoxResponse::toString( bool bMore /* = false */ ) const
{
    /* Special handling for POLL results. */
    if (     _type == hoxREQUEST_POLL
          && !_content.empty() )
    {
        return _content;
    }

    std::ostringstream outStream;

    outStream << "op=" << hoxUtil::requestTypeToString( _type )
              << "&code=" << _code;
    
    if ( ! _tid.empty() )
    {
        outStream << "&tid=" << _tid;
    }

    if ( bMore )
    {
        outStream << "&more=1";
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // NOTE: Make sure that the output is terminated by only
    //       (and only) TWO end-of-line characters. 
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    outStream << "&content="
              << ( _content.empty() ? "\n" : _content )
              << "\n";

    return outStream.str();
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_LOGIN( const std::string& playerId,
                                 const int          nPlayerScore,
                                 const std::string& sessionId /* = "" */ )
{
    std::ostringstream  outStream;

    outStream << playerId << ";" << nPlayerScore;
    if ( !sessionId.empty() )
    {
        outStream << ";" << sessionId;
    }
    outStream << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_LOGIN ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_LOGOUT( const hoxPlayer_SPtr player )
{
    std::ostringstream  outStream;

    outStream << player->getId()
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_LOGOUT ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_INVITE( const std::string& sInviterId,
                                  const int          nInviterScore,
                                  const std::string& sInviteeId,
                                  const std::string& tableId )
{
    std::ostringstream  outStream;

    outStream << sInviterId << ";"
              << nInviterScore << ";"
              << sInviteeId
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_INVITE ) );
    pResponse->setContent( outStream.str() );

    if ( ! tableId.empty() ) // Table-ID is optional!
    {
        pResponse->setTid( tableId );
    }

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_PLAYER_INFO( const hoxPlayer_SPtr player,
                                       const std::string&   tableId )
{
    std::ostringstream  outStream;

    outStream << player->getId() << ";"
              << player->getScore() << ";"
              << player->getWins() << ";"
              << player->getDraws() << ";"
              << player->getLosses() << ";"
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_PLAYER_INFO ) );
    pResponse->setContent( outStream.str() );

    pResponse->setTid( tableId );

    return pResponse;
}

/*static*/ 
hoxResponse_SPtr
hoxResponse::create_event_I_PLAYERS( const std::string& sEventContent )
{
    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_I_PLAYERS ) );
    pResponse->setContent(  sEventContent.empty() ? "\n"
                                                  : sEventContent );

    return pResponse;
}

/*static*/ 
hoxResponse_SPtr
hoxResponse::create_event_I_TABLE( const hoxTable*  pTable )
{
    std::ostringstream  outStream;

    hoxPlayer_SPtr redPlayer   = pTable->getRedPlayer();
    hoxPlayer_SPtr blackPlayer = pTable->getBlackPlayer();

    hoxTimeInfo redTime;
    hoxTimeInfo blackTime;
    pTable->getCurrentTimers( redTime, blackTime );

    outStream << pTable->getId() << ";"
              << pTable->getGameGroup() << ";"
              << pTable->getGameType() << ";"
              << hoxUtil::timeInfoToString(pTable->getInitialTime()) << ";"
              << hoxUtil::timeInfoToString(redTime) << ";"
              << hoxUtil::timeInfoToString(blackTime) << ";"
              << (redPlayer ? redPlayer->getId() : "") << ";"
              << (redPlayer ? redPlayer->getScore() : 0) << ";"
              << (blackPlayer ? blackPlayer->getId() : "") << ";"
              << (blackPlayer ? blackPlayer->getScore() : 0) << ";";

    hoxPlayerList observers; // Return list of observers also....
    pTable->getObservers( observers );
    for ( hoxPlayerList::const_iterator it = observers.begin();
                                        it != observers.end(); ++it )
    {
        outStream << (*it)->getId() << ";";
    }

    outStream << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_I_TABLE ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/ 
hoxResponse_SPtr
hoxResponse::create_event_LIST( const hoxTableList& tables )
{
    std::ostringstream  outStream;
    hoxPlayer_SPtr      redPlayer;
    hoxPlayer_SPtr      blackPlayer;

    for ( hoxTableList::const_iterator it = tables.begin();
                                       it != tables.end(); ++it )
    {
        redPlayer   = (*it)->getRedPlayer();
        blackPlayer = (*it)->getBlackPlayer();

        outStream << (*it)->getId() << ";"
                  << (*it)->getGameGroup() << ";"
                  << (*it)->getGameType() << ";"
                  << hoxUtil::timeInfoToString((*it)->getInitialTime()) << ";"
                  << hoxUtil::timeInfoToString((*it)->getRedTime()) << ";"
                  << hoxUtil::timeInfoToString((*it)->getBlackTime()) << ";"
                  << (redPlayer ? redPlayer->getId() : "") << ";"
                  << (redPlayer ? redPlayer->getScore() : 0) << ";"
                  << (blackPlayer ? blackPlayer->getId() : "") << ";"
                  << (blackPlayer ? blackPlayer->getScore() : 0) << ";"
                  << "\n";
    }

    if ( tables.empty() )
    {
        outStream << "\n";
    }

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_LIST ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_E_JOIN( const hoxTable*   pTable,
                                  const hoxPlayer_SPtr player,
                                  hoxColor          color )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId() << ";"
              << player->getScore() << ";"
              << hoxUtil::colorToString(color)
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_E_JOIN ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_LEAVE( const hoxTable*   pTable,
                                 const hoxPlayer_SPtr  player )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId()
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_LEAVE ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_MSG( const hoxPlayer_SPtr player,
                               const std::string&   message,
                               const std::string&   tableId /* = "" */ )
{
    std::ostringstream  outStream;

    outStream << player->getId() << ";"
              << message
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_MSG ) );
    pResponse->setContent( outStream.str() );

    if ( ! tableId.empty() ) // Table-ID is optional!
    {
        pResponse->setTid( tableId );
    }

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_PING()
{
    std::ostringstream  outStream;

    outStream << "PONG"
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_PING ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_MOVE( const hoxTable*    pTable,
                                const hoxPlayer_SPtr player,
                                const std::string& sMove,
                                hoxGameStatus      gameStatus )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId() << ";"
              << sMove << ";"
              << hoxUtil::gameStatusToString(gameStatus)
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_MOVE ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_DRAW( hoxResult          code,
                                const hoxTable*    pTable,
                                const hoxPlayer_SPtr player )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId()
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_DRAW,
                                                 code ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_END( const hoxTable*     pTable,
                               const hoxGameStatus gameStatus,
                               const std::string&  sReason )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << hoxUtil::gameStatusToString( gameStatus ) << ";"
              << sReason
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_E_END ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_RESET( const hoxTable* pTable )
{
    std::ostringstream  outStream;

    outStream << pTable->getId()
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_RESET ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_E_SCORE( const hoxTable*   pTable,
                                   const hoxPlayer_SPtr player )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId() << ";"
              << player->getScore()
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_E_SCORE ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_I_MOVES( const hoxTable*      pTable,
                                   const hoxStringList& moves )
{
    std::ostringstream  outStream;
    std::string         sMoves;

    for ( hoxStringList::const_iterator it = moves.begin();
                                        it != moves.end(); ++it )
    {
        if ( ! sMoves.empty() ) sMoves += "/";
        sMoves += (*it);
    }

    outStream << pTable->getId() << ";"
              << sMoves
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_I_MOVES ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_UPDATE( const hoxTable*    pTable,
                                  const hoxPlayer_SPtr player,
                                  const hoxGameType  newGameType,
                                  const hoxTimeInfo& newInitialTime )
{
    std::ostringstream  outStream;

    outStream << pTable->getId() << ";"
              << player->getId() << ";"
              << (newGameType == hoxGAME_TYPE_RATED ? "1" : "0") << ";"
              << hoxUtil::timeInfoToString( newInitialTime )
              << "\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_UPDATE ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_event_POLL( const hoxResponseSList& responseList )
{
    std::ostringstream  outStream;

    int  last_index = responseList.size() - 1;
    int  index = 0;
    bool bMore = false;  // More events after the current event?

    for ( hoxResponseSList::const_iterator it = responseList.begin();
                                           it != responseList.end(); ++it )
    {
        bMore = ( index != last_index );
        outStream << (*it)->toString( bMore );
        ++index;
    }

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_POLL ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}


// =========================================================================
//
//                        hoxHttpRequest
//
// =========================================================================

void
hoxHttpRequest::parseURI( const std::string& sURI )
{
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    typedef boost::char_separator<char> Separator;

    Separator sep(" ");
    Tokenizer tok(sURI, sep);
    int       i = 0;

    for ( Tokenizer::const_iterator it = tok.begin(); it != tok.end(); ++it )
    {
        switch ( i++ )
        {
            case 0: this->method = (*it); break;
            case 1:
            {
                std::string::size_type found = (*it).find_first_of('?');
                if ( found == std::string::npos )
                {
                    this->path = (*it);
                }
                else
                {
                    this->path = (*it).substr(0, found);
                    // TODO: Parse httpRequest.params...
                }
                break;
            }
            default: /* Ignore the rest. */ break;
        }
    }
}

hoxResult
hoxHttpRequest::continueReadRequest( st_netfd_t fd )
{
    hoxResult   result = hoxRC_OK;
    std::string sLine;  // The current request line.
    std::string sKey;   // The current header's key.
    std::string sVal;   // The current header's value.

    /* Read headers */
    for (;;)
    {
        result = hoxSocketAPI::read_line( fd, sLine, POLL_READ_TIMEOUT );
        if ( result != hoxRC_OK ) // Failed to read request?
        {
            return result;
        }
        else if ( sLine.empty() ) // Socket was closed?
        {
            return hoxRC_CLOSED;
        }
        else if ( sLine == "\r" ) // End of headers?
        {
            break;
        }

        std::string::size_type loc = sLine.find_first_of( ':' );
        if( loc != std::string::npos )
        {
            sKey = sLine.substr(0, loc);
            sVal = "";
            if ( loc+1 < sLine.size() )
            {
                sVal = sLine.substr(loc+1);
                hoxUtil::trimLast( sVal, '\r' );
            }
            //hoxLog(LOG_INFO, "%s: .... [%s] : [%s].", __FUNCTION__, sKey.c_str(), sVal.c_str());
            this->headers[sKey] = sVal;
        }
    }

    /* Read the body if it is supplied. */
    const std::string sContentLength = this->headers["Content-Length"];
    if ( !sContentLength.empty() )
    {
        const size_t nBytes = (size_t) ::atoi( sContentLength.c_str() );
        if ( hoxRC_OK != hoxSocketAPI::read_nbytes( fd,
                                                    nBytes,
                                                    this->body ) )
        {
            return hoxRC_ERR;
        }
    }
    return hoxRC_OK;
}

/******************* END OF FILE *********************************************/
