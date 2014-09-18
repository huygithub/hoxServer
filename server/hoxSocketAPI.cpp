//
// C++ Implementation: hoxSocketAPI
//
// Description: Socket API
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#include <cstring>
#include "hoxSocketAPI.h"
#include "hoxLog.h"


hoxResult
hoxSocketAPI::read_line( st_netfd_t   fd,
                         std::string& outLine,
                         const int    timeout )
{
    const char* FNAME = "hoxSocketAPI::read_line";
    hoxResult  result = hoxRC_ERR;
    char       c;
    ssize_t    nread;

    outLine.clear();

    const st_utime_t timeout_usecs = SEC2USEC( timeout ); // seconds -> microseconds

    /* Read until one of the following conditions is met:
     *  (1) One entire line is read.
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
                hoxLog(LOG_INFO, "%s: Interrupted by a singal.", FNAME);
                result = hoxRC_EINTR;
                break;
            }
            else if ( errno == ETIME ) // The timeout occurred and no data was read
            {
                hoxLog(LOG_INFO, "%s: Timeout [%d secs] occurred.", FNAME, timeout);
                result = hoxRC_TIMEOUT;
                break;
            }
            else
            {
                hoxLog(LOG_INFO, "%s: Socket error: [%s]", FNAME, ::strerror( errno ));
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

            if ( outLine.size() >= hoxNETWORK_MAX_MSG_SIZE ) // Impose limit.
            {
                hoxLog(LOG_WARN, "%s: Max-size [%d] reached: [%s ...]", FNAME,
                       hoxNETWORK_MAX_MSG_SIZE, outLine.substr( 0, 64 ).c_str());
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
    const char* FNAME = __FUNCTION__;

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
            hoxLog(LOG_SYS_WARN, "%s: Fail to read 1 byte from the network", FNAME);
            hoxLog(LOG_WARN, "%s: Result message accumulated so far = [%s].", FNAME, sResult.c_str());
            return hoxRC_ERR;
        }
    }

    return hoxRC_OK;
}

hoxResult
hoxSocketAPI::write_data( const st_netfd_t   fd,
                          const std::string& sOutData )
{
    const int nSize = sOutData.size();

    if ( nSize != st_write( fd, 
                            sOutData.data(), nSize,
                            ST_UTIME_NO_TIMEOUT ) )
    {
        hoxLog(LOG_SYS_WARN, "%s: Failed to write using st_write", __FUNCTION__);
        return hoxRC_ERR;
    }

    return hoxRC_OK;
}

/******************* END OF FILE *********************************************/
