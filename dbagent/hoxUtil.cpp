//
// C++ Implementation: hoxUtil
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include "hoxUtil.h"

// =========================================================================
//
//                        hoxUtil API
//
// =========================================================================

/**
 * Convert a given request-type to a (human-readable) string.
 */
const std::string
hoxUtil::requestTypeToString( const hoxRequestType requestType )
{
    switch ( requestType )
    {
        case hoxREQUEST_UNKNOWN:     return "UNKNOWN";

        case hoxREQUEST_HELLO:            return "HELLO";
        case hoxREQUEST_DB_PLAYER_PUT:    return "DB_PLAYER_PUT";
        case hoxREQUEST_DB_PLAYER_GET:    return "DB_PLAYER_GET";
        case hoxREQUEST_DB_PLAYER_SET:    return "DB_PLAYER_SET";
        case hoxREQUEST_DB_PROFILE_SET:   return "DB_PROFILE_SET";
        case hoxREQUEST_DB_PASSWORD_SET:  return "DB_PASSWORD_SET";
        case hoxREQUEST_HTTP_GET:         return "HTTP_GET";
        case hoxREQUEST_LOG:              return "LOG";

        default:                     return "UNKNOWN";
    }
}

/**
 * Convert a given (human-readable) string to a request-type.
 */
hoxRequestType
hoxUtil::stringToRequestType( const std::string& input )
{
    if ( input == "UNKNOWN" )     return hoxREQUEST_UNKNOWN;

    if ( input == "HELLO" )            return hoxREQUEST_HELLO;
    if ( input == "DB_PLAYER_PUT" )    return hoxREQUEST_DB_PLAYER_PUT;
    if ( input == "DB_PLAYER_GET" )    return hoxREQUEST_DB_PLAYER_GET;
    if ( input == "DB_PLAYER_SET" )    return hoxREQUEST_DB_PLAYER_SET;
    if ( input == "DB_PROFILE_SET" )   return hoxREQUEST_DB_PROFILE_SET;
    if ( input == "DB_PASSWORD_SET" )  return hoxREQUEST_DB_PASSWORD_SET;
    if ( input == "HTTP_GET" )         return hoxREQUEST_HTTP_GET;
    if ( input == "LOG" )              return hoxREQUEST_LOG;

    return hoxREQUEST_UNKNOWN;
}

const std::string
hoxUtil::intToString( const int number )
{
    char szBuffer[32];
    snprintf(szBuffer, sizeof(szBuffer), "%d", number);
    return std::string(szBuffer);
}

int
hoxUtil::readFile( const std::string& sPath,
                   std::string&       sContent )
{
    sContent.clear();
    using namespace std;

    /* CREDITS:
     *   http://www.cplusplus.com/doc/tutorial/files.html
     *
     * NOTE: This function only works for text files.
     */

    std::ifstream theFile( sPath.c_str(), ios::in|ios::binary|ios::ate );
    if ( ! theFile.is_open())
    {
        return -1; // error
    }

    const ifstream::pos_type size = theFile.tellg();
    char* memblock = new char[size];
    theFile.seekg(0, ios::beg);
    theFile.read(memblock, size);
    theFile.close();

    sContent.assign( memblock, size );
    delete[] memblock;

    return 0; // success
}

/******************* END OF FILE *********************************************/
