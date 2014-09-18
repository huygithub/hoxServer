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
#include <boost/tokenizer.hpp>
#include <cstdlib>     // rand()
#include "hoxUtil.h"
#include "hoxLog.h"
#include "hoxSocketAPI.h"

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

        case hoxREQUEST_HELLO:       return "HELLO";
        case hoxREQUEST_REGISTER:    return "REGISTER";
        case hoxREQUEST_LOGIN:       return "LOGIN";
        case hoxREQUEST_LOGOUT:      return "LOGOUT";
        case hoxREQUEST_SHUTDOWN:    return "SHUTDOWN";
        case hoxREQUEST_POLL:        return "POLL";
        case hoxREQUEST_MOVE:        return "MOVE";
        case hoxREQUEST_LIST:        return "LIST";
        case hoxREQUEST_NEW:         return "NEW";
        case hoxREQUEST_JOIN:        return "JOIN";
        case hoxREQUEST_LEAVE:       return "LEAVE";
        case hoxREQUEST_UPDATE:       return "UPDATE";
        case hoxREQUEST_RESIGN:      return "RESIGN";
        case hoxREQUEST_DRAW:        return "DRAW";
        case hoxREQUEST_RESET:       return "RESET";
        case hoxREQUEST_E_JOIN:      return "E_JOIN";
        case hoxREQUEST_E_END:       return "E_END";
        case hoxREQUEST_E_SCORE:     return "E_SCORE";
        case hoxREQUEST_I_PLAYERS:   return "I_PLAYERS";
        case hoxREQUEST_I_TABLE:     return "I_TABLE";
        case hoxREQUEST_I_MOVES:     return "I_MOVES";
        case hoxREQUEST_INVITE:      return "INVITE";
        case hoxREQUEST_PLAYER_INFO:   return "PLAYER_INFO";
        case hoxREQUEST_PLAYER_STATUS: return "PLAYER_STATUS";
        case hoxREQUEST_MSG:         return "MSG";
        case hoxREQUEST_PING:        return "PING";
        
        case hoxREQUEST_DB_PLAYER_PUT:    return "DB_PLAYER_PUT";
        case hoxREQUEST_DB_PLAYER_GET:    return "DB_PLAYER_GET";
        case hoxREQUEST_DB_PLAYER_SET:    return "DB_PLAYER_SET";
        case hoxREQUEST_DB_PASSWORD_SET:  return "DB_PASSWORD_SET";

        case hoxREQUEST_HTTP_GET:    return "HTTP_GET";
        case hoxREQUEST_HTTP_POST:   return "HTTP_POST";
        case hoxREQUEST_LOG:         return "LOG";

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

    if ( input == "HELLO" )       return hoxREQUEST_HELLO;
    if ( input == "REGISTER" )    return hoxREQUEST_REGISTER;
    if ( input == "LOGIN" )       return hoxREQUEST_LOGIN;
    if ( input == "LOGOUT" )      return hoxREQUEST_LOGOUT;
    if ( input == "SHUTDOWN" )    return hoxREQUEST_SHUTDOWN;
    if ( input == "POLL" )        return hoxREQUEST_POLL;
    if ( input == "MOVE" )        return hoxREQUEST_MOVE;
    if ( input == "LIST" )        return hoxREQUEST_LIST;
    if ( input == "NEW" )         return hoxREQUEST_NEW;
    if ( input == "JOIN" )        return hoxREQUEST_JOIN;
    if ( input == "LEAVE" )       return hoxREQUEST_LEAVE;
    if ( input == "UPDATE" )      return hoxREQUEST_UPDATE;
    if ( input == "RESIGN" )      return hoxREQUEST_RESIGN;
    if ( input == "DRAW" )        return hoxREQUEST_DRAW;
    if ( input == "RESET" )       return hoxREQUEST_RESET;
    if ( input == "E_JOIN" )      return hoxREQUEST_E_JOIN;
    if ( input == "E_END" )       return hoxREQUEST_E_END;
    if ( input == "E_SCORE" )     return hoxREQUEST_E_SCORE;
    if ( input == "I_PLAYERS" )   return hoxREQUEST_I_PLAYERS;
    if ( input == "I_TABLE" )     return hoxREQUEST_I_TABLE;
    if ( input == "I_MOVES" )     return hoxREQUEST_I_MOVES;
    if ( input == "INVITE" )      return hoxREQUEST_INVITE;
    if ( input == "PLAYER_INFO" )   return hoxREQUEST_PLAYER_INFO;
    if ( input == "PLAYER_STATUS" ) return hoxREQUEST_PLAYER_STATUS;
    if ( input == "MSG" )         return hoxREQUEST_MSG;
    if ( input == "PING" )        return hoxREQUEST_PING;

    if ( input == "DB_PLAYER_PUT" )   return hoxREQUEST_DB_PLAYER_PUT;
    if ( input == "DB_PLAYER_GET" )   return hoxREQUEST_DB_PLAYER_GET;
    if ( input == "DB_PLAYER_SET" )   return hoxREQUEST_DB_PLAYER_SET;
    if ( input == "DB_PASSWORD_SET" ) return hoxREQUEST_DB_PASSWORD_SET;

    if ( input == "HTTP_GET" )  return hoxREQUEST_HTTP_GET;
    if ( input == "HTTP_POST" ) return hoxREQUEST_HTTP_POST;
    if ( input == "LOG" )       return hoxREQUEST_LOG;

    return hoxREQUEST_UNKNOWN;
}

const std::string
hoxUtil::colorToString( const hoxColor color )
{
    switch ( color )
    {
        case hoxCOLOR_UNKNOWN:   return "UNKNOWN";

        case hoxCOLOR_RED:       return "Red";
        case hoxCOLOR_BLACK:     return "Black";
        case hoxCOLOR_NONE:      return "None";

        default:                 return "UNKNOWN";
    }
}

hoxColor
hoxUtil::stringToColor( const std::string& input )
{
    if ( input == "UNKNOWN" ) return hoxCOLOR_UNKNOWN;

    if ( input == "Red" )     return hoxCOLOR_RED;
    if ( input == "Black" )   return hoxCOLOR_BLACK;
    if ( input == "None" )    return hoxCOLOR_NONE;

    return hoxCOLOR_UNKNOWN;
}

hoxTimeInfo
hoxUtil::stringToTimeInfo( const std::string& input )
{
    hoxTimeInfo timeInfo;

    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    typedef boost::char_separator<char> Separator;

    Separator sep("/");
    Tokenizer tok(input, sep);

    int i = 0;
    for ( Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        switch (i++)
        {
            case 0: timeInfo.nGame = ::atoi(it->c_str()); break;
            case 1: timeInfo.nMove = ::atoi(it->c_str()); break;
            case 2: timeInfo.nFree = ::atoi(it->c_str()); break;
            default: break; // Ignore the rest.
        }
    }

    return timeInfo;
}

const std::string
hoxUtil::timeInfoToString( const hoxTimeInfo timeInfo )
{
    std::ostringstream outStream;

    outStream << timeInfo.nGame << "/"
              << timeInfo.nMove << "/"
              << timeInfo.nFree;
    return outStream.str();
}

const std::string
hoxUtil::intToString( const int number )
{
    char szBuffer[32];
    snprintf(szBuffer, sizeof(szBuffer), "%d", number);
    return std::string(szBuffer);
}

int
hoxUtil::stringToInt( const std::string str )
{
    return ::atoi( str.c_str() );
}

template<typename T>
T hoxUtil::convertTo( const std::string& s,
                      bool               failIfLeftoverChars /* = true */ )
{
    /* CREDITS:
     * http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
     *
     * Other references:
     *   + Bullet Proof Integer Input Using strtol()
     *         http://home.att.net/~jackklein/c/code/strtol.html
     */

    std::istringstream i ( s );
    T    x;
    char c;
    if ( ! ( i >> x ) || ( failIfLeftoverChars && i.get (c) ) )
    {
        // printf("%s: Error converting %s", __FUNCTION__, s.c_str());
    }
    return x;
}
/* Explicit template instantiation for some C++ STL containers */
template int hoxUtil::convertTo( const std::string& s,
                                 bool              failIfLeftoverChars );

const std::string
hoxUtil::gameStatusToString( const hoxGameStatus gameStatus )
{
    switch( gameStatus )
    {
        case hoxGAME_STATUS_UNKNOWN:     return "UNKNOWN";

		case hoxGAME_STATUS_OPEN:        return "open";
		case hoxGAME_STATUS_READY:       return "ready";
		case hoxGAME_STATUS_IN_PROGRESS: return "in_progress";
		case hoxGAME_STATUS_RED_WIN:     return "red_win";
		case hoxGAME_STATUS_BLACK_WIN:   return "black_win";
		case hoxGAME_STATUS_DRAWN:       return "drawn";

		default:                         return "UNKNOWN";
    }
}

hoxGameStatus
hoxUtil::stringToGameStatus( const std::string& input )
{
    if ( input == "UNKNOWN" )     return hoxGAME_STATUS_UNKNOWN;

    if ( input == "open" )        return hoxGAME_STATUS_OPEN;
    if ( input == "ready" )       return hoxGAME_STATUS_READY;
    if ( input == "in_progress" ) return hoxGAME_STATUS_IN_PROGRESS;
    if ( input == "red_win" )     return hoxGAME_STATUS_RED_WIN;
    if ( input == "black_win" )   return hoxGAME_STATUS_BLACK_WIN;
    if ( input == "drawn" )       return hoxGAME_STATUS_DRAWN;

    return hoxGAME_STATUS_UNKNOWN;
}

void
hoxUtil::parse_network_message( const std::string& sNetworkMessage,
                                hoxRequestType&    type,
                                hoxParameters&     parameters )
{
    type = hoxREQUEST_UNKNOWN;

    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    typedef boost::escaped_list_separator<char> Separator;

    Separator sep('\\', '&', '\"');
    Tokenizer tok(sNetworkMessage, sep);
    std::string            name;
    std::string            value;
    std::string::size_type found_index;

    for ( Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        /* Parse the "name=value" pair. */
        found_index = it->find_first_of('=');
        if ( found_index == std::string::npos ) // not found?
        {
            name = (*it);
            value = "";
        }
        else
        {
            name = it->substr(0, found_index);
            value = it->substr(found_index+1);
        }

        /* Convert to request-type if this is 'OP' name. */
        if ( name == "op" )
        {
            type = hoxUtil::stringToRequestType( value );
        }
        else
        {
            parameters[name] = value;
        }
    }
}

int
hoxUtil::generateRandomNumber( const unsigned max_value )
{
    /*
     * CREDITS:
     *    http://www.jb.man.ac.uk/~slowe/cpp/srand.html
     */

    static bool s_bSeeded = false;  // *** Only generate seed once per process.
    if ( ! s_bSeeded )
    {
        srand( time(NULL) + getpid() );
        s_bSeeded = true;
    }

    const int randNum =
        1 + (int) ((double)max_value * (rand() / (RAND_MAX + 1.0)));

    return randNum;
}

void
hoxUtil::trimLast( std::string& sInput,
                   const char   token )
{
    /* Trim the token at the end of the line. */
    std::string::size_type loc = sInput.find_last_of( token );
    if (  loc != std::string::npos
       && loc == sInput.size() - 1 )
    {
        sInput.erase( sInput.size() - 1, 1 );
    }
}

std::string
hoxUtil::getHttpDate()
{
    static char str[32];
    static time_t lastt = 0;

    time_t currt = st_time();
    if ( currt == lastt )
        return str;

    struct tm* timeinfo = localtime( &currt );
    strftime(str, sizeof(str), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

    lastt = currt;
    return str;
}

/******************* END OF FILE *********************************************/
