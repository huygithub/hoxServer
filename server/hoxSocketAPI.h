//
// C++ Interface: hoxSocketAPI
//
// Description: Socket API
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/16/2009
//

#ifndef __INCLUDED_HOX_SOCKET_API_H__
#define __INCLUDED_HOX_SOCKET_API_H__

#include <string>
#include <st.h>
#include "hoxEnums.h"

namespace hoxSocketAPI
{

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
    hoxResult read_nbytes( const st_netfd_t  fd,
                           const size_t      nBytes,
                           std::string&      sResult );

    /**
     * Write data to a socket.
     *
     */
    hoxResult write_data( const st_netfd_t   fd,
                          const std::string& sOutData );

} /* namespace hoxSocketAPI */

#endif /* __INCLUDED_HOX_SOCKET_API_H__ */
