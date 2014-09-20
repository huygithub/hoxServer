//
// C++ Implementation: hoxSocketAPI
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hoxSocketAPI.h"
#include "hoxLog.h"
#include <sys/socket.h>
#include <unistd.h>  // for read() API
#include <cerrno>

hoxResult
hoxSocketAPI::set_read_timeout( int fd,
                                int timeout)
{
    const char* FNAME = "hoxSocketAPI::set_read_timeout";

    struct timeval tvTimeout;
    tvTimeout.tv_sec = timeout;
    tvTimeout.tv_usec = 0;

    if ( -1 == setsockopt( fd, 
                           SOL_SOCKET,
                           SO_RCVTIMEO, 
                           &tvTimeout, 
                           sizeof(tvTimeout)) )
    {
        hoxLog(LOG_SYS_WARN, "%s: setsockopt SO_RCVTIMEO failed", FNAME);
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

hoxResult
hoxSocketAPI::read_line( int          fd,
                         std::string& outLine )
{
    const char* FNAME = "hoxSocketAPI::read_line";
    hoxResult  result = hoxRC_ERR;
    char       c;
    ssize_t    nread;

    outLine.clear();

    /* Read until one of the following conditions is met:
     *  (1) One while line is read.
     *  (2) Timeout occurs.
     *  (3) An error occurs.
     */

    for ( ;; )
    {
        /* Read one character at a time */
        nread = read(fd, &c, 1);

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
                hoxLog(LOG_INFO, "%s: Timeout occurred.", FNAME);
                result = hoxRC_TIMEOUT;
                break;
            }
            else
            {
                hoxLog(LOG_SYS_WARN, "%s: Socket error. Data read so far [%s].", 
                    FNAME, outLine.c_str());
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
                hoxLog(LOG_WARN, "%s: Maximum message's size [%d] reached. "
                    "Likely to be an error.",
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
hoxSocketAPI::read_nbytes( int          fd,
                           const size_t nBytes,
                           std::string& sResult )
{
    const char* FNAME = "hoxSocketAPI::read_nbytes";

    sResult.clear();

    /* Read a line until seeing N bytes */

    char     c;
    ssize_t  nRead = 0;

    while ( sResult.size() < nBytes )
    {
        nRead = read( fd, &c, 1 );
        if ( nRead == 1 )
        {
            sResult += c;
        }
        else
        {
            hoxLog(LOG_SYS_WARN, "%s: Fail to read 1 byte from the network", FNAME);
            hoxLog(LOG_WARN, "%s: Result message accumulated so far = [%s].", FNAME, sResult.c_str());
            return hoxRC_ERR;
        }
    }

    return hoxRC_OK;
}

/******************* END OF FILE *********************************************/
