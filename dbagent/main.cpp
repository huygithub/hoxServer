//
// C++ Implementation: Main module (Entrance)
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "hoxLog.h"
#include "hoxTypes.h"
#include "hoxSocketAPI.h"
#include "hoxDBAPI.h"
#include "hoxExcept.h"
#include "hoxUtil.h"

/******************************************************************
 * Server configuration parameters
 */

/* Listening IP/port */
#define DBAGENT_DEFAULT_IP   "0.0.0.0"
#define DBAGENT_DEFAULT_PORT 7000

/* Log files */
#define PID_FILE    "pid"
#define ERRORS_FILE "errors.log"
#define SERVER_FILE "../server/logs/errors.log"

/**
 * Configuration flags/parameters
 */
static char*       s_logdir      = NULL;
static const char* s_serverIP    = DBAGENT_DEFAULT_IP;
static int         s_nListenPort = DBAGENT_DEFAULT_PORT;

int s_errfd    = STDERR_FILENO;
int s_serverfd = STDERR_FILENO;

/******************************************************************
 * Data structure
 */

class SocketInfo
{
public:
    int             nSocket;
    struct in_addr  iaFrom;
};
typedef std::auto_ptr<SocketInfo>  SocketInfo_APtr;

/**
 * Open error log file.
 */

/******************************************************************
 * Helper API
 */

/**
 * Open error log file.
 */
int
open_log_file( const std::string& sLogDir )
{
    int fd = -1;

    if (( fd = open( sLogDir.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644 ) ) < 0 )
    {
        err_sys_quit( s_errfd, "ERROR: can't open error log file: [%s]", sLogDir.c_str() );
    }

    return fd;
}

/**
 * Read an incoming request from a given socket.
 */
hoxResult
read_request( int                   nfd,
              const struct in_addr* from,
              hoxRequest_SPtr&      pRequest )
{
    const char* FNAME = __FUNCTION__;
    hoxResult   result;
    std::string requestStr;

    result = hoxSocketAPI::read_line( nfd,
                                      requestStr );
    if ( result != hoxRC_OK )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to read request from %s",
            FNAME, inet_ntoa( *from ));
        return result;
    }

    /* Check for socket-close condition. */
    if ( requestStr.empty() )
    {
        //hoxLog(LOG_INFO, "%s: Returned 'NULL' request due to socket-close event.", FNAME);
        return hoxRC_CLOSED;
    }

    /* Parse the request. */
    pRequest.reset( new hoxRequest( requestStr ) );

    if ( ! pRequest->isValid() )
    {
        hoxLog(LOG_INFO, "%s: Request [%s] is invalid.", FNAME, requestStr.c_str() );
        return hoxRC_NOT_VALID;
    }

    return hoxRC_OK;
}

/**
 * Write an outgoing response back to the client.
 */
hoxResult
write_response( int                     nfd,
                const hoxResponse_SPtr& pResponse )
{
    const char* FNAME = __FUNCTION__;
    const std::string resp         = pResponse->toString();
    ssize_t           nToBeWritten = resp.size();

    //hoxLog(LOG_DEBUG, "%s: Sending out (%d)...", FNAME, nToBeWritten);

    ssize_t nWritten = write( nfd,
                              resp.c_str(), nToBeWritten);
    if ( nWritten != nToBeWritten )
    {
        hoxLog(LOG_SYS_WARN, "Failed to write to socket", FNAME );
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

/**
 * Handle request HELLO.
 */
hoxResult
handle_HELLO( const hoxRequest_SPtr&  pRequest,
              hoxResponse_SPtr&       pResponse )
{
    /* Return:
     *         Some server's info.
     */

    pResponse = hoxResponse::create_result_HELLO();

    return hoxRC_OK;
}

/**
 * Handle request DB_PLAYER_PUT.
 */
hoxResult
handle_DB_PLAYER_PUT( const hoxRequest_SPtr&  pRequest,
                      hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;
    hoxResult          result;
    hoxDBAPI::Player_t playerInfo;
    std::string        sEmail;   // The optional Email.

    playerInfo.id    = pRequest->getParam("pid");
    playerInfo.hpw   = pRequest->getParam("password");
    playerInfo.score = ::atoi( pRequest->getParam("score").c_str() );
    sEmail           = pRequest->getParam("email");

    result = hoxDBAPI::put_player_info( playerInfo, sEmail );
    if ( result != hoxRC_OK ) 
    {
        throw hoxError(hoxRC_ERR, "Failed to put NEW player-info");
    } 

    hoxLog(LOG_DEBUG, "%s: Put new player-info OK. id = [%s], password = [***], score = [%d].", 
            FNAME, playerInfo.id.c_str(), playerInfo.score);

    /* Return:
     *         Player-Info
     */

    std::ostringstream  outStream;

    outStream << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request DB_PLAYER_GET.
 */
hoxResult
handle_DB_PLAYER_GET( const hoxRequest_SPtr&  pRequest,
                      hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;
    hoxResult          result;
    const std::string  pid = pRequest->getParam("pid");
    hoxDBAPI::Player_t playerInfo;

    result = hoxDBAPI::get_player_info( pid, 
                                        playerInfo );
    if ( result != hoxRC_OK ) 
    {
        throw hoxError(hoxRC_ERR, "Failed to get player-info");
    } 
   
    if ( playerInfo.score == -1 )
    {
        throw hoxError(hoxRC_NOT_FOUND, "Player not found");
    }

    hoxLog(LOG_DEBUG, "%s: Obtained score=[%d], [W%d D%d L%d], hpassword=[%s], email=[%s].", 
        FNAME, playerInfo.score,
        playerInfo.wins, playerInfo.draws, playerInfo.losses,
        playerInfo.hpw.c_str(), playerInfo.email.c_str());

    /* Return:
     *         Player-Info
     */

    std::ostringstream  outStream;

    outStream << playerInfo.id << ";"
              << playerInfo.score << ";"
              << playerInfo.wins << ";"
              << playerInfo.draws << ";"
              << playerInfo.losses << ";"
              << playerInfo.hpw << ";"
              << playerInfo.email
              << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request DB_PLAYER_SET.
 */
hoxResult
handle_DB_PLAYER_SET( const hoxRequest_SPtr&  pRequest,
                      hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;
    hoxResult          result;
    hoxDBAPI::Player_t playerInfo;
    std::string sGameResult;   /* (W)in / (D)raw / (L)oss */

    playerInfo.id = pRequest->getParam("pid");
    playerInfo.score = ::atoi( pRequest->getParam("score").c_str() );
    sGameResult = pRequest->getParam("result");

    result = hoxDBAPI::set_player_info( playerInfo, sGameResult );
    if ( result != hoxRC_OK ) 
    {
        throw hoxError(hoxRC_ERR, "Failed to set player-info");
    } 

    hoxLog(LOG_DEBUG, "%s: Set new player-info OK. id = [%s], score = [%d].", 
            FNAME, playerInfo.id.c_str(), playerInfo.score);

    /* Return:
     *       Nothing
     */

    std::ostringstream  outStream;

    outStream << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request DB_PROFILE_SET.
 */
hoxResult
handle_DB_PROFILE_SET( const hoxRequest_SPtr&  pRequest,
                       hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;

    std::string pid       = pRequest->getParam("pid");
    std::string sEmail    = pRequest->getParam("email");
    std::string sPassword = pRequest->getParam("password");

    hoxResult result = hoxDBAPI::set_profile_info( pid, sEmail, sPassword );
    if ( result != hoxRC_OK ) 
    {
        throw hoxError(hoxRC_ERR, "Failed to set new profile");
    } 

    hoxLog(LOG_DEBUG, "%s: Set new profile OK. id = [%s].", FNAME, pid.c_str());

    /* Return:
     *       Nothing
     */

    std::ostringstream  outStream;

    outStream << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request DB_PASSWORD_SET.
 */
hoxResult
handle_DB_PASSWORD_SET( const hoxRequest_SPtr&  pRequest,
                        hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;
    hoxResult          result;
    hoxDBAPI::Player_t playerInfo;

    playerInfo.id = pRequest->getParam("pid");
    playerInfo.hpw = pRequest->getParam("password");

    result = hoxDBAPI::set_player_password( playerInfo );
    if ( result != hoxRC_OK ) 
    {
        throw hoxError(hoxRC_ERR, "Failed to set new player-password");
    } 

    hoxLog(LOG_DEBUG, "%s: Set new player-password OK. id = [%s], password = [xxx].", 
            FNAME, playerInfo.id.c_str());

    /* Return:
     *       Nothing
     */

    std::ostringstream  outStream;

    outStream << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request HTTP_GET.
 */
hoxResult
handle_HTTP_GET( const hoxRequest_SPtr&  pRequest,
                 hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;

    const std::string sPath = pRequest->getParam("path");

    std::string sContent;
    if ( 0 != hoxUtil::readFile( sPath, sContent ) )
    {
        sContent = "File not found: " + sPath;
    }

    hoxLog(LOG_DEBUG, "%s: Get file [%s]. Returned size = [%d].", 
           FNAME, sPath.c_str(), sContent.size());

    /* Return:
     *       The file's content.
     */

    std::ostringstream  outStream;

    outStream << sContent.size()
              << "\n\n"
              << sContent;  // *** "attached data" ***

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle request LOG.
 */
hoxResult
handle_LOG( int                     nfd,
            const hoxRequest_SPtr&  pRequest,
            hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;

    const int nSize = ::atoi( pRequest->getParam("size").c_str() );
    //hoxLog(LOG_DEBUG, "%s: Expect to log a message of size [%d] to file.", FNAME, nSize);

    std::string sMsg;
    if ( hoxRC_OK != hoxSocketAPI::read_nbytes( nfd, nSize,
                                                sMsg ) )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to read [%d] bytes", FNAME, nSize);
        // Still allow to continue
    }

    write( s_serverfd, sMsg.data(), sMsg.size() );

    /* Return:
     *       The file's content.
     */

    std::ostringstream  outStream;

    outStream << "OK: " << nSize
              << "\n\n";

    pResponse.reset( new hoxResponse( pRequest->getType() ) );
    pResponse->setContent( outStream.str() );

    return hoxRC_OK;
}

/**
 * Handle a request.
 */
hoxResult
handle_request( int                     nfd,
                const hoxRequest_SPtr&  pRequest,
                hoxResponse_SPtr&       pResponse )
{
    const char* FNAME = __FUNCTION__;
    hoxResult result = hoxRC_UNKNOWN;

    const hoxRequestType requestType = pRequest->getType();

    try
    {
        switch( requestType )
        {
            case hoxREQUEST_HELLO:
                return handle_HELLO( pRequest, pResponse );

            case hoxREQUEST_DB_PLAYER_PUT:
                return handle_DB_PLAYER_PUT( pRequest, pResponse );

            case hoxREQUEST_DB_PLAYER_GET:
                return handle_DB_PLAYER_GET( pRequest, pResponse );

            case hoxREQUEST_DB_PLAYER_SET:
                return handle_DB_PLAYER_SET( pRequest, pResponse );

            case hoxREQUEST_DB_PROFILE_SET:
                return handle_DB_PROFILE_SET( pRequest, pResponse );

            case hoxREQUEST_DB_PASSWORD_SET:
                return handle_DB_PASSWORD_SET( pRequest, pResponse );

            case hoxREQUEST_HTTP_GET:
                return handle_HTTP_GET( pRequest, pResponse );

            case hoxREQUEST_LOG:
                return handle_LOG( nfd, pRequest, pResponse );

            default:
                throw hoxError(hoxRC_NOT_SUPPORTED, "Unsupported Request");
        }
    }
    catch( hoxError error )
    {
        result = error.code();
        pResponse.reset( new hoxResponse( requestType, result ) );
        pResponse->setContent( error.what() + std::string("\n\n") );
        hoxLog(LOG_WARN, "%s: Error caught [%s].", FNAME, error.toString().c_str());
    }

    return result;
}

/**
 * A thread that handles a client connection.
 */
void*
handle_client_thread( void* arg )
{
    const char* FNAME = __FUNCTION__;
    const SocketInfo_APtr pSocketInfo( (SocketInfo* ) arg );
    hoxResult result;

    hoxLog(LOG_INFO, "%s: ENTER. Client from = [%s].", FNAME, 
        inet_ntoa( pSocketInfo->iaFrom ));

    const int fd = pSocketInfo->nSocket;

    /* Set the socket's timeout on reading INPUT. */ 
    const int timeout = (365 * 24 * 3600 );  // forever =  1 year
    if ( hoxRC_OK != hoxSocketAPI::set_read_timeout( fd, timeout ) )
    {
        hoxLog(LOG_SYS_WARN, "%s: Fail to set socket's read timeout.", FNAME);
        // NOTE: *** Still allow to continue.
    }

    for (;;)
    {
        hoxRequest_SPtr  pRequest;
        hoxResponse_SPtr pResponse;

        /* Read the incoming request. */

        result = ::read_request( fd,
                                 &(pSocketInfo->iaFrom),
                                 pRequest );
        if ( result != hoxRC_OK )
        {
            if ( result != hoxRC_CLOSED ) // Not 'connection closed'?
            {
                hoxLog(LOG_INFO, "%s: Cannot read request.", FNAME);
            }
            break;
        }

        //hoxLog(LOG_DEBUG, "%s: Received [%s]", FNAME, pRequest->toString().c_str() );

        /* Handle the request. */

        result = ::handle_request( fd,
                                   pRequest,
                                   pResponse );
        if ( result != hoxRC_OK )
        {
            hoxLog(LOG_INFO, "%s: Failed to handle request.", FNAME);
            // TODO: Still allow to continue...
        }

        /* Write a response, if any. */

        if ( pResponse.get() != NULL )
        {
            result = write_response( fd, 
                                     pResponse );
            if ( result != hoxRC_OK )
            {
                hoxLog(LOG_WARN, "%s: Failed to write response", FNAME);
                break;
            }
        }

    } /* for(...) */
    
    /* Close connection. */
    close( fd );

    hoxLog(LOG_INFO, "%s: END.", FNAME);
    return NULL;
}

/******************************************************************/

static void usage( const char *progname )
{
    fprintf( stderr, "Usage: %s -l <log_directory> [<options>]\n\n"
             "Possible options:\n\n"
             "\t-b <listening_ip>       The listening IP address.\n"
             "\t-p <listening_port>     The listening port.\n"
             "\t-l <loggin_directory>   The logging directory.\n"
             "\t-h                      Print this message.\n",
             progname );
    exit( 1 );
}

/******************************************************************/

static void parse_arguments( int argc, char *argv[] )
{
    extern char *optarg;
    int opt;
    char* c = NULL;

    while (( opt = getopt( argc, argv, "b:p:l:h" ) ) != EOF )
    {
        switch ( opt )
        {
            case 'b':
                if (( c = strdup( optarg ) ) == NULL )
                    err_sys_quit( s_errfd, "ERROR: strdup" );
                s_serverIP = c;
                break;
            case 'p':
                s_nListenPort = atoi( optarg );
                if ( s_nListenPort < 1024 )
                    err_quit( s_errfd, "ERROR: invalid listening port: %s", optarg );
                break;
            case 'l':
                s_logdir = optarg;
                break;
            case 'h':
            case '?':
                usage( argv[0] );
        }
    }

    if ( s_logdir == NULL )
    {
        err_report( s_errfd, "ERROR: logging directory is required\n" );
        usage( argv[0] );
    }
}

/******************************************************************/

static void open_log_files( void )
{
    int         fd = -1;
    char        str[32];
    std::string sFilename;

    /* Open and write pid to pid file */
    sFilename = std::string(s_logdir) + PID_FILE;
    if (( fd = open( sFilename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644 ) ) < 0 )
        err_sys_quit( s_errfd, "ERROR: can't open pid file: [%s]", sFilename.c_str() );
    sprintf( str, "%d\n", ( int )getpid() );
    if ( write( fd, str, strlen( str ) ) != (int) strlen( str ) )
        err_sys_quit( s_errfd, "ERROR: can't write to pid file: write" );
    close( fd );

    /* Open error log file */
    sFilename = std::string(s_logdir) + ERRORS_FILE;
    s_errfd = ::open_log_file( sFilename );

    /* Open server log file */
    s_serverfd = ::open_log_file( SERVER_FILE );
}

/******************************************************************
 * Small utility functions
 */

static void Signal( int sig, void ( *handler )( int ) )
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset( &sa.sa_mask );
    sa.sa_flags = 0;
    sigaction( sig, &sa, NULL );
}

/******************************************************************/

static void wdog_sighandler( int signo )
{
    int err;

    /* Save errno */
    err = errno;

    /*
     * It is safe to do pretty much everything here because process is
     * sleeping in wait() which is async-safe.
     */
    switch ( signo )
    {
        case SIGTERM:
        {
            /* Non-graceful termination */
            hoxLog(LOG_INFO, "watchdog: caught SIGTERM, terminating" );
            const std::string sFilename = std::string(s_logdir) + PID_FILE;
            unlink( sFilename.c_str() );
            exit( 0 );
        }
        default:
            hoxLog(LOG_INFO, "watchdog: caught signal %d", signo );
    }
    /* Restore errno */
    errno = err;
}

/******************************************************************/

/**
 * Main function.
 */
int
main( int argc, char *argv[] )
{
    const char* FNAME = __FUNCTION__;
    int       listenSock;   // Server listening socket.
    int       cliSock;      // Client remote socket.
    socklen_t clientAddressLength;
    struct sockaddr_in clientAddress;
    struct sockaddr_in serverAddress;
    SocketInfo* pSocketInfo = NULL;
    pthread_t   serviceThread;

    /* Parse command-line options */
    parse_arguments( argc, argv );

    /* Change the current directory. */
    //if ( chdir( s_logdir ) < 0 )
    //    err_sys_quit( s_errfd, "ERROR: can't change directory to %s: chdir", s_logdir );

    /* Open log files */
    open_log_files();

    hoxLog(LOG_INFO, "%s: Starting [dbagent]...", FNAME);

    /* Install signal handlers */
    Signal( SIGTERM, wdog_sighandler );  /* terminate */

    /* Create socket for listening for client connection requests.  */
    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) 
    {
        hoxLog(LOG_SYS_ERROR, "%s: cannot create listen socket", FNAME);
        exit(1);
    }
  
    /* Bind listen socket to listen port. */
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(s_serverIP);
    serverAddress.sin_port = htons(s_nListenPort);

    if ( bind( listenSock,
               (struct sockaddr *) &serverAddress,
               sizeof(serverAddress) ) < 0 ) 
    {
        hoxLog(LOG_SYS_ERROR, "%s: cannot bind socket", FNAME);
        exit(1);
    }

    /* Wait for connections from clients.
     * This is a non-blocking call; i.e., it registers this program with
     * the system as expecting connections on this socket, and then
     * this thread of execution continues on.
     */
    listen(listenSock, 5);
  
    for (;;)
    {
        hoxLog(LOG_INFO, "%s: Waiting for connections on [%s:%d]...", 
            FNAME, s_serverIP, s_nListenPort);

        /* Accept a connection with a client that is requesting one.
         */
        clientAddressLength = sizeof(clientAddress);
        cliSock = accept( listenSock,
                          (struct sockaddr *) &clientAddress,
                          &clientAddressLength);
        if ( cliSock < 0 ) 
        {
            hoxLog(LOG_SYS_ERROR, "%s: cannot accept connection", FNAME);
            exit(1);
        }
    
        /* (1) Show the IP address of the client.
         *     inet_ntoa() converts an IP address from binary form to the
         *     standard "numbers and dots" notation.
         *
         * (2) Show the client's port number.
         *     ntohs() converts a short int from network byte order (which is
         *     Most Significant Byte first) to host byte order (which on x86,
         *     for example, is Least Significant Byte first).
         */
        hoxLog(LOG_DEBUG, "%s: connected to [%s:%d]", FNAME, 
            inet_ntoa(clientAddress.sin_addr),
            ntohs(clientAddress.sin_port));

        /* Create a separate thread to handle this client. */

        pSocketInfo = new SocketInfo;
        pSocketInfo->nSocket = cliSock;
        pSocketInfo->iaFrom = clientAddress.sin_addr;

        if ( 0 != pthread_create( &serviceThread,
                                  NULL,   /* Use Default Attributes */
                                  handle_client_thread, 
                                  (void*) pSocketInfo ) )
        {
            hoxLog(LOG_SYS_ERROR, "%s: Failed to create service Thread", FNAME);
            delete pSocketInfo;
            continue;  // NOTE: *** Still continue...
        }

    } /* for(...) */

    /* Close log files. */
    close( s_errfd );
    close( s_serverfd );

    return 0;
}

/******************* END OF FILE *********************************************/
