//
// C++ Implementation: hoxSocketAPI
//
// Description: 
//
//
// Author: Huy Phan, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hoxSocketAPI.h"
#include "hoxLog.h"
#include "hoxDebug.h"
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

hoxResult
hoxSocketAPI::tcp_connect( const std::string&       sHost,
                           const unsigned short int nPort,
                           st_netfd_t&              nfd )
{
    int                 iResult = 0;
    struct sockaddr_in  serverAddress;
    struct hostent*     hostInfo;
    int                 sock = -1;

    nfd = NULL;   // Default: invalid socket

    // gethostbyname() takes a host name or ip address in "numbers and
    // dots" notation, and returns a pointer to a hostent structure,
    // which we'll need later.  It's not important for us what this
    // structure is actually composed of.
    hostInfo = gethostbyname( sHost.c_str() );
    if (hostInfo == NULL)
    {
        hoxLog(LOG_SYS_WARN, "%s: problem interpreting host: %s", __FUNCTION__, sHost.c_str());
        return hoxRC_ERR;
    }

    // Create a socket.  "AF_INET" means it will use the IPv4 protocol.
    // "SOCK_STREAM" means it will be a reliable connection (i.e., TCP;
    // for UDP use SOCK_DGRAM), and I'm not sure what the 0 for the last
    // parameter means, but it seems to work.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        hoxLog(LOG_SYS_WARN, "%s: cannot create socket", __FUNCTION__);
        return hoxRC_ERR;
    }

    if ((nfd = st_netfd_open_socket(sock)) == NULL) {
        hoxLog(LOG_SYS_WARN, "%s: st_netfd_open_socket failed", __FUNCTION__);
        close(sock);
        return hoxRC_ERR;
    }

    // Connect to server.  First we have to set some fields in the
    // serverAddress structure.  The system will assign me an arbitrary
    // local port that is not in use.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
            hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons( nPort);

    iResult = st_connect( nfd,
                          (struct sockaddr *) &serverAddress,
                           sizeof(serverAddress),
                           ST_UTIME_NO_TIMEOUT );
    if ( iResult < 0 )
    {
        hoxLog(LOG_SYS_WARN, "%s: cannot connect", __FUNCTION__);
        st_netfd_close( nfd );
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

hoxResult
hoxSocketAPI::read_line( st_netfd_t   fd,
                         std::string& outLine,
                         const int    timeout )
{
    const char* FNAME = "hoxSocketAPI::read_line";
    hoxResult  result = hoxRC_ERR;
    char       c;
    ssize_t    nread;
    st_utime_t timeout_usecs;

    outLine.clear();

    timeout_usecs = SEC2USEC( timeout ); // convert seconds -> microseconds

    /* Read until one of the following conditions is met:
     *  (1) One while line is read.
     *  (2) Timeout occurs.
     *  (3) An error occurs.
     */

    for ( ;; )
    {
        /* Read one character at a time */
        nread = st_read(fd, &c, 1, timeout_usecs);

        /* CASE 1: Network connection is closed */
        if ( nread == 0 )
        {
            hoxLog(LOG_INFO, "%s: Network connection closed.", FNAME);
            result = hoxRC_OK;
            break;
        }

        /* CASE 2: Possible error */
        if ( nread < 0 )
        {
            if ( errno == EINTR )  // The current thread was interrupted
            {
                continue;
            }
            else if ( errno == ETIME	) // The timeout occurred and no data was read
            {
                hoxLog(LOG_INFO, "%s: Timeout [%d secs] occurred.", FNAME, timeout);
                result = hoxRC_TIMEOUT;
                break;
            }
            else
            {
                hoxLog(LOG_SYS_WARN, "%s: Socket error.", FNAME);
                result = hoxRC_ERR;
                break;
            }
        }

        /* CASE 3: Read data OK */
        if ( c == '\n' )
        {
            result = hoxRC_OK;
            break; // Success.
        }
        else
        {
            outLine += c;

            // Impose some limit.
            if ( outLine.size() >= hoxNETWORK_MAX_MSG_SIZE )
            {
                hoxLog(LOG_WARN, "%s: Maximum message's size [%d] reached.",
                    FNAME, hoxNETWORK_MAX_MSG_SIZE );
                hoxLog(LOG_WARN, "%s: Partial read message (64 bytes) = [%s ...].",
                    FNAME, outLine.substr( 0, 64 ).c_str() );
                result = hoxRC_ERR;
                break;
            }
        }
    }

    return result;
}

hoxResult
hoxSocketAPI::read_nbytes( const st_netfd_t  fd,
                           const size_t      nBytes,
                           std::string&      sResult )
{
    /* Read a line until seeing N bytes */

    char     c;
    ssize_t  nRead = 0;

    for (;;)
    {
        nRead = st_read( fd, &c, sizeof(c), ST_UTIME_NO_TIMEOUT );
        if ( nRead == 1 )
        {
            sResult += c;
            if ( sResult.size() == nBytes )
            {
                break;   // Done.
            }
        }
        else
        {
            hoxLog(LOG_SYS_WARN, "%s: Fail to read 1 byte from the network", __FUNCTION__);
            hoxLog(LOG_WARN, "%s: Result message accumulated so far = [%s].", __FUNCTION__, sResult.c_str());
            return hoxRC_ERR;
        }
    }

    return hoxRC_OK;
}

int
hoxSocketAPI::read_until_all( const st_netfd_t   fd,
                              const std::string& sWanted,
                              std::string&       sOutput,
                              const int          timeout /* = -1 */ )
{
    const unsigned int nMax = 10 * 1024;  // 10-K limit

    sOutput.clear();

    char         c;         // The byte just received.
    const size_t requiredSeen = sWanted.size();
    size_t       currentSeen  = 0;
    int          iResult      = 0;
    int          nTotal       = 0; // Total bytes received so far.

    const st_utime_t timeout_usecs = ( timeout == -1 ? ST_UTIME_NO_TIMEOUT
                                                     : SEC2USEC( timeout ) );

    hoxCHECK_MSG(sizeof(char) == 1, HOX_ERR_SOCKET_OTHER, "size of char != 1");

    /* Receive data until seeing all characters in the "wanted" string
    * or until the peer closes the connection
    */
    while ( currentSeen < requiredSeen )
    {
        iResult = st_read( fd, &c, 1, timeout_usecs );
        if ( iResult == 1 )
        {
            sOutput += c;
            if ( c == sWanted[currentSeen] ) // seen the next char?
            {
                ++currentSeen;
                continue;
            }

            currentSeen = 0;  // Reset "what we have seen".

            if ( sOutput.size() >= nMax )  // Impose some limit.
            {
                hoxLog(LOG_WARN, "%s: *WARN* Max message's size [%d] reached.",
                    __FUNCTION__, nMax);
                return HOX_ERR_SOCKET_LIMIT;
            }
        }
        else if ( iResult == 0 ) // Connection closed?
        {
            return HOX_ERR_SOCKET_CLOSED;
        }
        else // Some other socket error?
        {
            if ( errno == EINTR )  // The current thread was interrupted
            {
                continue;
            }
            else if ( errno == ETIME    ) // The timeout occurred and no data was read
            {
                hoxLog(LOG_DEBUG, "%s: Timeout [%d secs].", __FUNCTION__, timeout);
                return HOX_ERR_SOCKET_TIMEOUT;
            }
            else
            {
                hoxLog(LOG_SYS_WARN, "%s: st_read failed", __FUNCTION__);
                return HOX_ERR_SOCKET_OTHER;
            }
        }
    }

    /* Chop off 'want' string. */
    if ( currentSeen == requiredSeen && requiredSeen > 0 )
    {
        nTotal = sOutput.size();
        sOutput = sOutput.substr(0, nTotal - requiredSeen);
    }

    return nTotal; // Return the number of bytes received.
}

hoxResult
hoxSocketAPI::write_string( const st_netfd_t    nfd,
                            const std::string&  sData )
{
    const int nSize = sData.size();

    if ( nSize != st_write( nfd,
                            sData.c_str(), nSize,
                            ST_UTIME_NO_TIMEOUT ) )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to write using st_write", __FUNCTION__);
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

/******************* END OF FILE *********************************************/
