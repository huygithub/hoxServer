/***************************************************************************
 *  Copyright 2007-2009 Huy Phan                                           *
 *                                                                         * 
 *  This file is part of HOXChess.                                         *
 *                                                                         *
 *  HOXChess is free software: you can redistribute it and/or modify       *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  HOXChess is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with HOXChess.  If not, see <http://www.gnu.org/licenses/>.      *
 ***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Name:            hoxCommand.cpp
// Created:         10/04/2008
//
// Description:     Containing simple types commonly used through out 
//                  the project.
/////////////////////////////////////////////////////////////////////////////

#include "hoxCommand.h"
#include "hoxCommon.h"
#include <boost/algorithm/string.hpp>  // trim_right()

// ----------------------------------------------------------------------------
// hoxCommand
// ----------------------------------------------------------------------------

const std::string
hoxCommand::ToString() const
{
    std::string result;

    result += "op=" + m_type;

    for ( hoxParameters::const_iterator it = m_parameters.begin();
                                        it != m_parameters.end(); ++it )
    {
        result += "&" + it->first + "=" + it->second;
    }

    return result;
}

void
hoxCommand::Clear()
{
    m_type = "";
    m_parameters.clear();
}

/* static */ void
hoxCommand::String_To_Command( const std::string& sInput, 
                               hoxCommand&        command )
{
    command.Clear();

    hoxSeparator sep("&");
    hoxTokenizer tok(sInput, sep);

    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);

        size_t sepIndex = token.find_first_of( '=' );

        if ( sepIndex == std::string::npos )
            continue;  // NOTE: Ignore this 'error' token.

        std::string paramName  = token.substr( 0, sepIndex );
        std::string paramValue = token.substr( sepIndex + 1 );

        if ( paramName == "op" ) // Special case for "op" param.
        {
            command.m_type = paramValue;
        }
        else
        {
            boost::trim_right( paramValue );
            command.m_parameters[paramName] = paramValue;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_LOGIN( const std::string& sInput,
                                   std::string&       playerId,
                                   int&               nScore )
{
    playerId = "";
    nScore   = 0;

    hoxTokenizer tok( sInput, hoxSeparator(";") );

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch ( i++ )
        {
            case 0: playerId = token;  break;
            case 1: nScore = ::atoi( token.c_str() ); break;
            default: /* Ignore the rest. */ break;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_I_PLAYERS( const std::string& sInput,
                                       hoxStringList&     players )
{
    players.clear();

    hoxTokenizer tok( sInput, hoxSeparator("\n") );

    std::string playerId;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        // NOTE: Only extract player-ID for now (i.e., ignore score...)
        std::string::size_type foundIndex = token.find_first_of(';');
        if ( foundIndex != std::string::npos )
        {
            playerId = token.substr(0, foundIndex);
            players.push_back( playerId );
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_E_JOIN( const std::string& sInput,
                                    std::string&       tableId,
                                    std::string&       playerId,
                                    int&               nPlayerScore,
                                    hoxColor&          color )
{
    nPlayerScore = 0;
    color        = hoxCOLOR_NONE; // Default = observer.

    hoxSeparator sep(";");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch ( i++ )
        {
            case 0: tableId = token;  break;
            case 1: playerId = token;  break;
            case 2: nPlayerScore = ::atoi( token.c_str() ); break;
            case 3: color = hoxUtil::StringToColor( token ); break;
            default: /* Ignore the rest. */ break;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_INVITE( const std::string& sInput,
                                    std::string&       inviterId )
{
    inviterId = "";

    hoxSeparator sep(";");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::const_iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch ( i++ )
        {
            case 0: inviterId  = token;  break;
            case 1: /* nInviterScore = token */;  break;
            case 2: /*inviteeId  = token*/;  break;
            default: /* Ignore the rest. */ break;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_MOVE( const std::string& sInput,
                                  std::string&       tableId,
                                  std::string&       playerId,
                                  std::string&       sMove,
                                  hoxGameStatus&     gameStatus)
{
    tableId    = "";
    playerId   = "";
    sMove      = "";
    gameStatus = hoxGAME_STATUS_UNKNOWN;

    hoxSeparator sep(";");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch ( i++ )
        {
            case 0: tableId  = token;  break;
            case 1: playerId = token;  break;
            case 2: sMove    = token;  break;
            case 3: gameStatus = hoxUtil::StringToGameStatus(token);  break;
            default: /* Ignore the rest. */ break;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_E_END( const std::string& sInput,
                                   std::string&       tableId,
                                   hoxGameStatus&     gameStatus,
                                   std::string&       sReason )
{
    tableId    = "";
    gameStatus = hoxGAME_STATUS_UNKNOWN;
    sReason    = "";

    hoxSeparator sep(";");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch (i++)
        {
            case 0: tableId  = token;  break;
            case 1: gameStatus = hoxUtil::StringToGameStatus(token);  break;
            case 2: sReason  = token;  break;
            default: /* Ignore the rest. */ break;
        }
    }
}

/* static */ void
hoxCommand::Parse_InCommand_DRAW( const std::string& sInput,
                                  std::string&       tableId,
                                  std::string&       playerId )
{
    tableId    = "";
    playerId   = "";

    hoxSeparator sep(";");
    hoxTokenizer tok(sInput, sep);

    int i = 0;
    for ( hoxTokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        const std::string token = (*it);
        switch (i++)
		{
			case 0: tableId  = token;  break;
            case 1: playerId = token;  break;
			default: /* Ignore the rest. */ break;
		}
	}
}

/************************* END OF FILE ***************************************/
