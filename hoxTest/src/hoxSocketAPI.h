//
// C++ Interface: hoxSocketAPI
//
// Description: 
//
//
// Author: Huy Phan, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_SOCKET_API_H__
#define __INCLUDED_HOX_SOCKET_API_H__

#include <string>
#include <st.h>
#include "hoxCommon.h"

#define HOX_INVALID_SOCKET      (-1)    /* Invalid socket descriptor       */
#define HOX_ERR_SOCKET_OTHER    (-1)    /* Some generic socket error       */
#define HOX_ERR_SOCKET_CLOSED   (-2)    /* Socket closed error             */
#define HOX_ERR_SOCKET_TIMEOUT  (-3)    /* Socket timeout error            */
#define HOX_ERR_SOCKET_LIMIT    (-4)    /* Socket over-buffer-limit error  */

namespace hoxSocketAPI
{

    /**
     * Make a TCP socket connection.
     *
     * @param sHost [IN] The host-name
     * @param nPort [IN] The port nubmer
     * @param nfd [OUT] The returned network file descriptor.
     */
    hoxResult tcp_connect( const std::string&       sHost,
                           const unsigned short int nPort,
                           st_netfd_t&              nfd );

    /**
     * Read a complete line (terminated by an '\n').
     *
     * @param timeout Time-out in seconds.
     */
    hoxResult read_line( st_netfd_t   fd,
                         std::string& outLine,
                         const int    timeout );

    /**
     * Read N bytes from a socket.
     *
     */
    hoxResult read_nbytes( const st_netfd_t fd,
                           const size_t     nBytes,
                           std::string&     sResult );

    /**
     * Read from a socket until seeing all the "wanted" characters.
     *
     * @param timeout Time-out in seconds.
     *                (-1) if no timeout is specified.
     */
    int read_until_all( const st_netfd_t   fd,
                        const std::string& sWanted,
                        std::string&       sOutput,
                        const int          timeout = -1 );

    /**
     * Write a given string (any data) to a socket.
     *
     */
    hoxResult write_string( const st_netfd_t    nfd,
                            const std::string&  sData );


} /* namespace hoxSocketAPI */

#endif /* __INCLUDED_HOX_SOCKET_API_H__ */

