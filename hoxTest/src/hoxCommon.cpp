//
// C++ Implementation: "common" interface
//
// Description:  Containing basic Constants and Types commonly used
//               through out the project.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:      04/12/2009
//

#include "hoxCommon.h"


/* static */ void
hoxTableInfo::String_To_Table( const std::string& sInput, 
                               hoxTableInfo&      tableInfo )
{
    tableInfo.Clear();

    hoxSeparator sep(";", 0,
                     boost::keep_empty_tokens);
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch (i++)
        {
            case 0: /* Id */ tableInfo.id = token; break;

            case 1: /* Group */
                tableInfo.group = ( token == "0" ? hoxGAME_GROUP_PUBLIC
                                                    : hoxGAME_GROUP_PRIVATE );
                break;

            case 2: /* Type */
                tableInfo.gameType = ( token == "0" ? hoxGAME_TYPE_RATED
                                                    : hoxGAME_TYPE_NONRATED );
                break;

            case 3: /* Initial-Time */
                tableInfo.initialTime = hoxUtil::StringToTimeInfo( token );
                break;

            case 4: /* RED-Time */
                tableInfo.redTime = hoxUtil::StringToTimeInfo( token );
                break;

            case 5: /* BLACK-Time */
                tableInfo.blackTime = hoxUtil::StringToTimeInfo( token );
                break;

            case 6: /* RED-Id */      tableInfo.redId      = token; break;
            case 7: /* RED-Score */   tableInfo.redScore   = token; break;
            case 8: /* BLACK-Id */    tableInfo.blackId    = token; break;
            case 9: /* BLACK-Score */ tableInfo.blackScore = token; break;

            default: break; // Ignore the rest
        }
    }
}

hoxTimeInfo 
hoxUtil::StringToTimeInfo( const std::string& sInput )
{
    hoxTimeInfo timeInfo;

    hoxSeparator sep("/");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch (i++)
        {
            case 0: /* Game-Time */ timeInfo.nGame = ::atoi(token.c_str()); break;
            case 1: /* Move-Time */ timeInfo.nMove = ::atoi(token.c_str()); break;
            case 2: /* Free-Time */ timeInfo.nFree = ::atoi(token.c_str());	break;

            default: break; // Ignore the rest.
        }
    }

    return timeInfo;
}

const std::string 
hoxUtil::ColorToString( const hoxColor color )
{
    switch( color )
    {
        case hoxCOLOR_UNKNOWN:   return "UNKNOWN";

        case hoxCOLOR_RED:       return "Red";
        case hoxCOLOR_BLACK:     return "Black";
        case hoxCOLOR_NONE:      return "None";

        default:                 return "UNKNOWN";
    }
}

hoxColor 
hoxUtil::StringToColor( const std::string& sInput )
{
    if ( sInput == "UNKNOWN" ) return hoxCOLOR_UNKNOWN;

    if ( sInput == "Red" )     return hoxCOLOR_RED;
    if ( sInput == "Black" )   return hoxCOLOR_BLACK;
    if ( sInput == "None" )    return hoxCOLOR_NONE;

    return hoxCOLOR_UNKNOWN;
}

const std::string
hoxUtil::GameStatusToString( const hoxGameStatus gameStatus )
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
hoxUtil::StringToGameStatus( const std::string& sInput )
{
    if ( sInput == "UNKNOWN" )     return hoxGAME_STATUS_UNKNOWN;

    if ( sInput == "open" )        return hoxGAME_STATUS_OPEN;
    if ( sInput == "ready" )       return hoxGAME_STATUS_READY;
    if ( sInput == "in_progress" ) return hoxGAME_STATUS_IN_PROGRESS;
    if ( sInput == "red_win" )     return hoxGAME_STATUS_RED_WIN;
    if ( sInput == "black_win" )   return hoxGAME_STATUS_BLACK_WIN;
    if ( sInput == "drawn" )       return hoxGAME_STATUS_DRAWN;

    return hoxGAME_STATUS_UNKNOWN;
}

/*
 * Random numbers functions.
 *
 * CREDITS:
 *    http://www.jb.man.ac.uk/~slowe/cpp/srand.html
 */

void
hoxUtil::generateRandomSeed()
{    
    srand( time(NULL) + getpid() );
}

unsigned
hoxUtil::generateRandomInRange( unsigned min_value, unsigned max_value )
{
    const unsigned randNum = min_value
        + (unsigned) ((double)(max_value-min_value+1) * (rand() / (RAND_MAX + 1.0)));
    return randNum;
}

unsigned
hoxUtil::generateRandomNumber( const unsigned max_value )
{
    return hoxUtil::generateRandomInRange(1, max_value);
}

template <typename T>
std::string 
hoxUtil::joinElements(const T& l, char sep /* = ',' */)
{
    std::string sResult;
    typename T::const_iterator it;
    for ( it = l.begin(); it != l.end(); ++it )
    {
        if ( !sResult.empty() ) sResult += sep;
        sResult += (*it);
    }
    return sResult;
}

/* Explicit template instantiation for some C++ STL containers */
template
std::string hoxUtil::joinElements(const hoxStringVector& l, char sep /* = ',' */);

/************************* END OF FILE ***************************************/
