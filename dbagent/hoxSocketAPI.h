//
// C++ Interface: hoxSocketAPI
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_SOCKET_API_H_
#define __INCLUDED_HOX_SOCKET_API_H_

#include <string>
#include "hoxEnums.h"

/*
 *  "No-timeout" constant.
 */
#define HOX_SOCKET_API_NO_TIMEOUT  (-1)

namespace hoxSocketAPI
{

    /**
     * Set the socket's timeout on reading.
     *
     * @param timeout Time-out in seconds.
     */
    hoxResult
    set_read_timeout( int fd,
                      int timeout);

    /**
     * Read a complete line (terminated by an '\n').
     *
     */
    hoxResult
    read_line( int          fd,
               std::string& outLine );

    /**
     * Read a number of bytes.
     *
     * @param nBytes The number of bytes to be read.
     */
    hoxResult
    read_nbytes( int          fd,
                 const size_t nBytes,
                 std::string& sResult );

} /* namespace hoxSocketAPI */

#endif /* __INCLUDED_HOX_SOCKET_API_H_ */
