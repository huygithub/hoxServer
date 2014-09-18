//
// C++ Interface: hoxUtil
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_UTIL_H_
#define __INCLUDED_HOX_UTIL_H_

#include <string>
#include "hoxTypes.h"


/**
 * Utility API.
 */
namespace hoxUtil
{
    /**
     * Convert a given request-type to a (human-readable) string.
     */
    const std::string
    requestTypeToString( const hoxRequestType requestType );

    /**
     * Convert a given (human-readable) string to a request-type.
     */
    hoxRequestType
    stringToRequestType( const std::string& input );

    /**
     * Convert a given Integer to a string.
     */
    const std::string
    intToString( const int number );

    /**
     * Read the entire content of a given "text" file.
     * Return: 0 if success.
     */
    int
    readFile( const std::string& sPath,
              std::string&       sContent );
  
}

#endif /* __INCLUDED_HOX_UTIL_H_ */
