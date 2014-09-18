//
// C++ Implementation: hoxDbClient
//
// Description: Client API to communicate to "DB-Agent".
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#include "hoxDbClient.h"
#include "hoxLog.h"
#include "hoxDebug.h"
#include "hoxTypes.h"
#include "hoxPlayer.h"
#include "hoxUtil.h"
#include "hoxSocketAPI.h"

#include <st.h>
#include <string>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <boost/tokenizer.hpp>

/* Constants. */
#define WWW_HOST      "www.playxiangqi.com"
#define WWW_PORT      80

/* -----------------------------------------------------------------------
 *
 *     Helper Data Types
 *
 * ----------------------------------------------------------------------- */

class Player_t
{
public:
    Player_t() : score(0), wins(0), draws(0), losses(0) {}

    std::string   id;    // Player-Id
    std::string   hpw;   // Hashed password.
    int           score;
    int           wins;
    int           draws;
    int           losses;
};

/* -----------------------------------------------------------------------
 *
 *     Private API
 *
 * ----------------------------------------------------------------------- */

namespace
{
    bool         s_bInitialized = false;
                    /* Has the module been initialized? */

    st_netfd_t   s_nfd          = NULL;  
                    /* Client socket descriptor. */

    st_thread_t  s_writeThread  = NULL;
                   /* The "write" helper thread. */

    bool         s_bShutdownWriteThread = false;
                  /* Is 'shutdown' in effect for DB-Write thread? */
    
    st_mutex_t   s_mutex = NULL;
                  /* Provide exclusve access to DB/Disk resources */

    hoxRequestSList s_requestList;
    st_cond_t       s_writeCond = NULL;  // Write condition-variable.

    /* --------------- HELPER Data structure --------------------- */

    class Lock /* To provide exclusive access */
    {
    public:
        Lock() /*: _mutex( st_mutex_new() ) */
            {
                st_mutex_lock( s_mutex );
            }

        ~Lock()
            {
                st_mutex_unlock( s_mutex );
                //st_mutex_destroy( _mutex );
                //_mutex = NULL;
            }

    private:
        //st_mutex_t   _mutex;
    };


    /* --------------- API  -------------------------------------- */

    /**
     * Open a outgoing client socket.
     *
     * @return NULL if error.
     */
    st_netfd_t
    _open_client_socket( const char* szHost,
                         const int   nPort )
    {
        const char* FNAME = __FUNCTION__;

        struct hostent*    he = NULL;
        int                sock = -1;
        st_netfd_t         nfd = NULL;
        struct sockaddr_in rmt_addr;    /* Remote address */

        hoxLog(LOG_DEBUG, "%s: ENTER. Server-addess = [%s:%d].", FNAME, szHost, nPort);

         /* Get the host info. */
        if ( NULL == ( he = gethostbyname( szHost )) )
        {
            hoxLog(LOG_SYS_ERROR, "%s: Failed to get host-info", FNAME);
            return NULL;
        }

        /* Create a socket. */
        sock = socket( PF_INET, SOCK_STREAM, 0 );
        if (sock < 0) 
        {
            hoxLog(LOG_SYS_ERROR, "%s: Failed to create socket", FNAME);
            return NULL;
        }

        nfd = st_netfd_open_socket( sock );
        if ( nfd == NULL )
        {
            hoxLog(LOG_SYS_ERROR, "%s: Failed to open socket", FNAME);
            close(sock);
            return NULL;
        }

        /* Initialize the remote server address. */
        rmt_addr.sin_family = AF_INET;    // host byte order 
        rmt_addr.sin_port = htons( nPort );  // short, network byte order 
        rmt_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(rmt_addr.sin_zero, '\0', sizeof rmt_addr.sin_zero);

        if ( st_connect( nfd, 
                        (struct sockaddr *)&rmt_addr,
                         sizeof(rmt_addr),
                         ST_UTIME_NO_TIMEOUT ) < 0 ) 
        {
            hoxLog(LOG_SYS_ERROR, "%s: Failed to connect to the server", FNAME);
            st_netfd_close( nfd );
            return NULL;
        }

        return nfd;
    }

    /**
     * Read from a socket a "line" terminated by TWO '\n' characters.
     *
     * @TODO  We should merge this version with the 1-new-line-char one.
     */
    hoxResult
    _read_line( const st_netfd_t  nfd,
                std::string&      sResult )
    {
        const char* FNAME = __FUNCTION__;
        std::string commandStr;

	    /* Read a line until "\n\n" */

	    bool     bSawOne = false;  // Has seen one '\n'?
        char     c;
        ssize_t  nRead = 0;

        for (;;)
        {
            nRead = st_read( nfd, &c, sizeof(c), ST_UTIME_NO_TIMEOUT );
            if ( nRead == 1 )
            {
			    if ( !bSawOne && c == '\n' )
			    {
				    bSawOne = true;
			    }
			    else if ( bSawOne && c == '\n' )
			    {
				    sResult = commandStr;
				    return hoxRC_OK;  // Done.
			    }
                else
                {
                    bSawOne = false;
                    commandStr += c;

                    // Impose some limit.
                    if ( commandStr.size() >= hoxNETWORK_MAX_MSG_SIZE )
                    {
                        hoxLog(LOG_ERROR, "%s: Maximum message's size [%d] reached. Likely to be an error.", 
                            FNAME, hoxNETWORK_MAX_MSG_SIZE);
                        hoxLog(LOG_ERROR, "%s: Partial read message (64 bytes) = [%s ...].", 
                            FNAME, commandStr.substr(0, 64).c_str());
                        break;
                    }
                }
            }
            else
            {
                hoxLog(LOG_SYS_WARN, "%s: Fail to read 1 byte from the network", FNAME);
                hoxLog(LOG_WARN, "%s: Result message accumulated so far = [%s].", FNAME, commandStr.c_str());
                break;
            }
        }

        return hoxRC_ERR;
    }

    /**
     * Send a request over a specified socket.
     *
     */
    hoxResult
    _send_request_over_socket( const st_netfd_t   nfd, 
                               const hoxRequest   request,
                               std::string&       sResponse,
                               const std::string& sData = "")
    {
        const char* FNAME = __FUNCTION__;
        //hoxLog(LOG_DEBUG, "%s: ENTER.", FNAME);

        /* Serialize the request (ie., make sure it has the "right" 
         * outgoing format).
         */
        std::string sRequest = request.toString() + "\n";
        if ( !sData.empty() ) sRequest.append( sData );

        const int nToSend = sRequest.size();
        ssize_t   nSent = 0;

        //hoxLog(LOG_DEBUG, "%s: Sending request [%s]...", FNAME, sRequest.c_str());
        nSent = st_write( nfd, 
                          sRequest.data(), 
                          nToSend, 
                          ST_UTIME_NO_TIMEOUT );
        if ( nSent < nToSend )
        {
            hoxLog(LOG_SYS_WARN, "%s: Failed to write to socket", FNAME);
            return hoxRC_ERR;
        }

        //hoxLog(LOG_DEBUG, "%s: Reading response...", FNAME);
        if ( hoxRC_OK != _read_line( nfd, sResponse ) )
        {
            hoxLog(LOG_SYS_WARN, "%s: Failed to read from socket", FNAME);
            return hoxRC_ERR;
        }

        //hoxLog(LOG_DEBUG, "%s: Received: [%s].", FNAME, sResponse.c_str());

        return hoxRC_OK;
    }

    /**
     * Parse a string for playe-info.
     *
     */
    hoxResult
    _parse_str_player_info( const std::string& input,
                            Player_t&          playerInfo )
    {
        typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
        typedef boost::char_separator<char> Separator;

        Separator sep(";");
        Tokenizer tok(input, sep);
        int       i = 0;

        for ( Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
        {
            switch (i++)
            {
                case 0: playerInfo.id = (*it); break;
                case 1: playerInfo.score = ::atoi( it->c_str() ); break;
                case 2: playerInfo.wins = ::atoi( it->c_str() ); break;
                case 3: playerInfo.draws = ::atoi( it->c_str() ); break;
                case 4: playerInfo.losses = ::atoi( it->c_str() ); break;
                case 5: playerInfo.hpw = (*it); break;
                default: /* Ignore the rest. */ break;
            }
        }

        return hoxRC_OK;
    }

    /**
     * Handle write Thread.
     */
    void*
    _handle_db_write( void * /*arg*/ )
    {
        const char* FNAME = __FUNCTION__;
        hoxResult result = hoxRC_UNKNOWN;

        while ( ! s_bShutdownWriteThread )
        {
            if ( s_requestList.empty() )
            {
                st_cond_wait( s_writeCond );
                continue;    // NOTE: Double-check one more time.
            }

            hoxRequest_SPtr pRequest( s_requestList.front() );
            s_requestList.pop_front();

            hoxLog(LOG_DEBUG, "%s: Got a new request [%s].", FNAME, pRequest->toString().c_str());

            std::string      sResponse;
            {
                Lock lock;  // Obtain exclusive access.
                result = _send_request_over_socket( s_nfd, *(pRequest.get()),
                                                    sResponse );
            }

            /* Parse the response for player-info. */

            hoxRequestType   type = hoxREQUEST_UNKNOWN;
            hoxParameters    parameters;

            hoxUtil::parse_network_message( sResponse,
                                            type,
                                            parameters );
            if ( type != pRequest->getType() )
            {
                hoxLog(LOG_ERROR, "%s: Wrong returned Message-Type [%s].", FNAME,
                    hoxUtil::requestTypeToString(type).c_str());
                continue;  // *** Still allow to continue
            }

            const std::string sCode    = parameters["code"];
            //const std::string sContent = parameters["content"];

            if ( sCode != "0" )
            {
                hoxLog(LOG_ERROR, "%s: Received an Error-code [%s].", FNAME, sCode.c_str());
                continue;  // *** Still allow to continue
            }

            st_sleep( ST_UTIME_NO_WAIT ); // yield so that others can run.
        }

        /* Cleanup WRITE connection. */

        s_writeThread = NULL;
        hoxLog(LOG_INFO, "%s: Closing DB WRITE connection.", FNAME);

        return NULL;
    }

} /* END of private namespace */

/* -----------------------------------------------------------------------
 *
 *     hoxDbClient namespace
 *
 * ----------------------------------------------------------------------- */

hoxResult
hoxDbClient::initialize( const char* szHost,
                         int         nPort )
{
    const char* FNAME = "hoxDbClient::initialize";
    hoxResult result = hoxRC_UNKNOWN;

    hoxLog(LOG_INFO, "%s: ENTER. [%s:%d]", FNAME, szHost, nPort);

    if ( s_bInitialized )
    {
        hoxLog(LOG_DEBUG, "%s: The module has already initialized.", FNAME);
        return hoxRC_OK;
    }

    s_mutex = st_mutex_new();  // ... to provide exclusive assess

    /* Open a "shared" client socket. */
    s_nfd = _open_client_socket( szHost, nPort );
    if ( s_nfd == NULL )
    {
        hoxLog(LOG_ERROR, "%s: Failed to open a client socket.", FNAME);
        return hoxRC_OK;
    }

    s_bInitialized = true;

    /* Send an "HELLO" request to make sure that the DB Agent is OK. */
    result = send_HELLO();
    if ( result != hoxRC_OK )
    {
        hoxLog(LOG_ERROR, "%s: Failed to send HELLO request.", FNAME);
        return hoxRC_ERR;
    }

    /* Start a "write" thread to help avoiding blocking
     * handlers of client requests.
     */

    s_writeCond = st_cond_new();

    s_writeThread = st_thread_create( _handle_db_write,
                                      ( void * ) NULL,
                                      1 /* joinable */,
                                      0 /* stack-size */ );
    if ( s_writeThread == NULL )
    {
        hoxLog(LOG_ERROR, "%s: Failed to create write DB thread.", FNAME);
        return hoxRC_ERR;
    }

    hoxLog(LOG_DEBUG, "%s: END. (OK)", FNAME);
    return hoxRC_OK;
}

hoxResult
hoxDbClient::deinitialize()
{
    const char* FNAME = "hoxDbClient::deinitialize";

    hoxLog(LOG_DEBUG, "%s: ENTER.", FNAME);

    if ( ! s_bInitialized )
    {
        hoxLog(LOG_WARN, "%s: The module has NOT YET initialized.", FNAME);
        return hoxRC_ERR;
    }

    /* Close the "shared" client socket. */
    st_netfd_close( s_nfd );
    s_nfd = NULL;

    s_bInitialized = false;

    hoxLog(LOG_DEBUG, "%s: END. (OK)", FNAME);
    return hoxRC_OK;
}

hoxResult
hoxDbClient::send_HELLO()
{
    const char* FNAME = "hoxDbClient::send_HELLO";
    hoxResult   result = hoxRC_UNKNOWN;

    hoxLog(LOG_DEBUG, "%s: ENTER.", FNAME);

    Lock lock;  // Obtain exclusive access.

    const hoxRequest request( hoxREQUEST_HELLO );
    std::string      sResponse;

    result = _send_request_over_socket( s_nfd, request,
                                        sResponse );
    return result;
}

hoxResult
hoxDbClient::put_player_info( hoxPlayer_SPtr     player,
                              const std::string& sEmail )
{
    const char* FNAME = "hoxDbClient::put_player_info";
    hoxResult         result = hoxRC_UNKNOWN;
    const std::string sPlayerId = player->getId();

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, sPlayerId.c_str());

    Lock lock;  // Obtain exclusive access.

    hoxRequest request( hoxREQUEST_DB_PLAYER_PUT );
    request.setParam("pid", sPlayerId);
    request.setParam("password", player->getHPassword());
    request.setParam("score", hoxUtil::intToString(player->getScore()));
    request.setParam("email", sEmail);

    std::string      sResponse;

    result = _send_request_over_socket( s_nfd, request,
                                        sResponse );

    /* Parse the response for player-info. */

    hoxRequestType   type = hoxREQUEST_UNKNOWN;
    hoxParameters    parameters;

    hoxUtil::parse_network_message( sResponse,
                                    type,
                                    parameters );
    if ( type != request.getType() )
    {
        hoxLog(LOG_ERROR, "%s: Wrong returned Message-Type [%s].", FNAME,
            hoxUtil::requestTypeToString(type).c_str());
        return hoxRC_ERR;
    }

    const std::string sCode    = parameters["code"];

    if ( sCode != "0" )
    {
        hoxLog(LOG_ERROR, "%s: Received an Error-code [%s].", FNAME, sCode.c_str());
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

hoxResult
hoxDbClient::get_player_info( hoxPlayer_SPtr player )
{
    hoxResult         result = hoxRC_UNKNOWN;
    const std::string sPlayerId = player->getId();

    Lock lock;  // Obtain exclusive access.

    hoxRequest request( hoxREQUEST_DB_PLAYER_GET );
    request.setParam("pid", sPlayerId);

    std::string sResponse;
    result = _send_request_over_socket( s_nfd, request,
                                        sResponse );

    /* Parse the response for player-info. */

    hoxRequestType   type = hoxREQUEST_UNKNOWN;
    hoxParameters    parameters;

    hoxUtil::parse_network_message( sResponse,
                                    type,
                                    parameters );
    if ( type != request.getType() )
    {
        hoxLog(LOG_ERROR, "%s: Wrong returned Message-Type [%s].", __FUNCTION__,
            hoxUtil::requestTypeToString(type).c_str());
        return hoxRC_ERR;
    }

    const std::string sCode    = parameters["code"];
    const std::string sContent = parameters["content"];

    const int nCode = hoxUtil::convertTo<int>( sCode );
    if ( nCode == hoxRC_NOT_FOUND )
    {
        hoxLog(LOG_INFO, "%s: Player [%s] not found in DB.", __FUNCTION__, sPlayerId.c_str());
        return hoxRC_NOT_FOUND;
    }
    else if ( nCode != hoxRC_OK )
    {
        hoxLog(LOG_ERROR, "%s: Received an Error-code [%d].", __FUNCTION__, hoxRC_OK);
        return hoxRC_ERR;
    }

    Player_t  playerInfo;

    result = _parse_str_player_info( sContent, playerInfo );
    hoxCHECK_MSG(result == hoxRC_OK, hoxRC_ERR, "Failed to parse Content");
    hoxCHECK_MSG(playerInfo.id == sPlayerId, hoxRC_ERR, "Player Ids not matched");

    player->setScore( playerInfo.score );
    player->setWins( playerInfo.wins );
    player->setDraws( playerInfo.draws );
    player->setLosses( playerInfo.losses );
    player->setHPassword( playerInfo.hpw );

    return hoxRC_OK;
}

void
hoxDbClient::set_player_info( const hoxPlayer_SPtr player,
                              const std::string&   sGameResult )
{
    const std::string sPlayerId = player->getId();

    hoxRequest_SPtr pRequest( new hoxRequest( hoxREQUEST_DB_PLAYER_SET ) );
    pRequest->setParam("pid", sPlayerId);
    pRequest->setParam("score", hoxUtil::intToString(player->getScore()));
    pRequest->setParam("result", sGameResult);

    s_requestList.push_back( pRequest );
    st_cond_signal( s_writeCond );
}

hoxResult
hoxDbClient::set_player_password( const hoxPlayer_SPtr player )
{
    const char* FNAME = "hoxDbClient::set_player_password";
    hoxResult         result = hoxRC_UNKNOWN;
    const std::string sPlayerId = player->getId();

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, sPlayerId.c_str());

    Lock lock;  // Obtain exclusive access.

    hoxRequest request( hoxREQUEST_DB_PASSWORD_SET );
    request.setParam("pid", sPlayerId);
    request.setParam("password", player->getHPassword());

    std::string      sResponse;

    result = _send_request_over_socket( s_nfd, request,
                                        sResponse );

    /* Parse the response for player-info. */

    hoxRequestType   type = hoxREQUEST_UNKNOWN;
    hoxParameters    parameters;

    hoxUtil::parse_network_message( sResponse,
                                    type,
                                    parameters );
    if ( type != request.getType() )
    {
        hoxLog(LOG_ERROR, "%s: Wrong returned Message-Type [%s].", FNAME,
            hoxUtil::requestTypeToString(type).c_str());
        return hoxRC_ERR;
    }

    const std::string sCode    = parameters["code"];
    //const std::string sContent = parameters["content"];

    if ( sCode != "0" )
    {
        hoxLog(LOG_ERROR, "%s: Received an Error-code [%s].", FNAME, sCode.c_str());
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

hoxResult
hoxDbClient::WWW_authenticate( const std::string& sPlayerId,
                               const std::string& sHPassword )
{
    const char* FNAME = "hoxDbClient::WWW_authenticate";
    hoxResult result = hoxRC_ERR;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, sPlayerId.c_str());

    /* Open the socket connect to the server. */

    const char* szHost = WWW_HOST;
    const int   nPort  = WWW_PORT;
    st_netfd_t  nfd    = NULL;

    nfd = _open_client_socket( szHost, nPort );
    if ( nfd == NULL )
    {
        hoxLog(LOG_ERROR, "%s: Failed to open a client socket to [%s:%d].", FNAME, szHost, nPort);
        return hoxRC_ERR;
    }

    /* Send the request. */

    std::string sRequest;

    sRequest = std::string("GET /blog/hoxchess-login.php")
                + "?pid="      + sPlayerId 
                + "&password=" + sHPassword
                + " HTTP/1.0\r\n"
             + "Host: " + szHost + "\r\n"
             + "Content-Length: 0\r\n"
             + "\r\n";

    const int nToSend = sRequest.size();
    ssize_t   nSent = 0;

    hoxLog(LOG_DEBUG, "%s: Sending (%d bytes): [\n%s]...", FNAME, sRequest.size(), sRequest.c_str());
    nSent = st_write( nfd, 
                      sRequest.c_str(), 
                      nToSend, 
                      ST_UTIME_NO_TIMEOUT );
    if ( nSent < nToSend )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to write to socket", FNAME);
        st_netfd_close( nfd );
        return hoxRC_OK;
    }

    /* Read the response back. */

    hoxLog(LOG_DEBUG, "%s: Reading response...", FNAME);
    std::string sResponse;
    ssize_t     nRead = 0;
    const int   MAX_TO_READ = 512;  // *** Hard-coded max-buffer-size.
    char        szBuffer[MAX_TO_READ];

    for (;;)
    {
        memset( szBuffer, 0, MAX_TO_READ );  // zero-out.
        nRead = st_read( nfd, szBuffer, MAX_TO_READ, ST_UTIME_NO_TIMEOUT );
        if ( nRead > 0 )
        {
            sResponse.append( szBuffer, nRead ); 
        }
        else if ( nRead != MAX_TO_READ )  // Connection closed?
        {
            break; // Done
        }
    }

    hoxLog(LOG_DEBUG, "%s: Received (%d bytes): [\n%s].", FNAME, sResponse.size(), sResponse.c_str());

    /* Check for return-code. */

    std::string::size_type npBody = sResponse.find("\r\n\r\n");

    if (   npBody != std::string::npos
        && sResponse.substr(npBody+4).find_first_of('0') == 0 )
    {
        result = hoxRC_OK;
    }

    /* Cleanup and return. */

    st_netfd_close( nfd );
    return result;
}

hoxResult
hoxDbClient::get_http_file( const std::string& sPath,
                            std::string&       sFileContent )
{
    const char* FNAME = "hoxDbClient::get_http_file";
    hoxResult result = hoxRC_UNKNOWN;

    hoxLog(LOG_DEBUG, "%s: ENTER. path = [%s].", FNAME, sPath.c_str());
    Lock lock;  // Obtain exclusive access.

    hoxRequest request( hoxREQUEST_HTTP_GET );
    request.setParam("path", sPath);

    std::string      sResponse;

    result = _send_request_over_socket( s_nfd, request,
                                        sResponse );

    /* Parse the response for player-info. */

    hoxRequestType   type = hoxREQUEST_UNKNOWN;
    hoxParameters    parameters;

    hoxUtil::parse_network_message( sResponse,
                                    type,
                                    parameters );
    if ( type != request.getType() )
    {
        hoxLog(LOG_ERROR, "%s: Wrong returned Message-Type [%s].", FNAME,
            hoxUtil::requestTypeToString(type).c_str());
        return hoxRC_ERR;
    }

    const std::string sCode    = parameters["code"];
    const std::string sContent = parameters["content"];

    if ( sCode != "0" )
    {
        hoxLog(LOG_ERROR, "%s: Received an Error-code [%s].", FNAME, sCode.c_str());
        return hoxRC_ERR;
    }

    const size_t nSize = ::atoi( sContent.c_str() );
    hoxLog(LOG_DEBUG, "%s: Path = [%s], Size = [%d].", FNAME, sPath.c_str(), nSize);

    result = hoxSocketAPI::read_nbytes( s_nfd, nSize, sFileContent );
    hoxCHECK_MSG(result == hoxRC_OK, hoxRC_ERR, "Failed to read N bytes");

    return hoxRC_OK;
}

hoxResult
hoxDbClient::log_msg( const std::string& sMsg )
{
    hoxResult   result = hoxRC_UNKNOWN;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !! Try to avoid logging here since this function  !!
    // !! is doing the "logging".                        !!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    Lock lock;  // Obtain exclusive access.

    hoxRequest request( hoxREQUEST_LOG );
    request.setParam("size", hoxUtil::intToString(sMsg.size()));

    std::string      sResponse;

    result = _send_request_over_socket( s_nfd, request,
                                        sResponse,
                                        sMsg /* Additional data */ );
    return result;
}

/******************* END OF FILE *********************************************/
