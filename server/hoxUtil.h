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

#ifndef __INCLUDED_HOX_UTIL_H__
#define __INCLUDED_HOX_UTIL_H__

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
     * Convert a given Color (Piece's Color or Role) to a (human-readable) string.
     */
    const std::string
    colorToString( const hoxColor color );

    /**
     * Convert a given (human-readable) string to a Color (Piece's Color or Role).
     */
    hoxColor
    stringToColor( const std::string& input );

    /**
     * Convert a given (human-readable) string to a Time-Info\ of
     * of the format "nGame/nMove/nFree".
     */
    hoxTimeInfo
    stringToTimeInfo( const std::string& input );

    /**
     * Convert a given Time-Info to a (human-readable) string.
     */
    const std::string
    timeInfoToString( const hoxTimeInfo timeInfo );

    /**
     * Convert a given Integer to a string.
     */
    const std::string
    intToString( const int number );

    /**
     * Convert a given string to an Integer.
     * @NOTE Obsoleted by convertTo (see below).
     */
    int
    stringToInt( const std::string str );

    /**
     * Convert a string to a number.
     *
     */
    template<typename T>
    T convertTo( const std::string& s,
                 bool               failIfLeftoverChars = true );

	/**
	 * Convert a given Game-Status to a (human-readable) string.
	 */
    const std::string
    gameStatusToString( const hoxGameStatus gameStatus );

    /**
     * Convert a given (human-readable) string to a Game-Status.
     */
    hoxGameStatus
    stringToGameStatus( const std::string& input );

    /**
     * Parse a Network Message.
     */
    void
    parse_network_message( const std::string& sNetworkMessage,
                           hoxRequestType&    type,
                           hoxParameters&     parameters );

    /**
     * Generate a random number of range [1, max_value].
     */
    int
    generateRandomNumber( const unsigned max_value );

    /**
     * Trim the token at the end of the line.
     */
    void
    trimLast( std::string& sInput,
              const char   token );

    /**
     * Get the current Date in HTTP format.
     */
    std::string getHttpDate();

}

#endif /* __INCLUDED_HOX_UTIL_H__ */
