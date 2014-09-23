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

/**
 * DLL Interface classes for C++ Binary Compatibility article
 * article at http://aegisknight.org/cppinterface.html
 *
 * code author:     Ben Scott   (bscott@iastate.edu)
 * article author:  Chad Austin (aegis@aegisknight.org)
 */


#include <AIEngineLib.h>
#include <DefaultDelete.h>
#include "XQWLight.h"

class AIEngineImpl : public DefaultDelete<AIEngineLib>
{
public:
    AIEngineImpl( const char* engineName )
    {
        m_name = engineName ? engineName : "__UNKNOWN__";
    }

    ~AIEngineImpl()
    {
    }

    void destroy()
    {
        delete this;
    }

    void initEngine( int searchDepth = 0 )
    {
        if ( searchDepth > 0 )
        {
            XQWLight::init_engine( searchDepth );
        }
    }

  	int initGame( const std::string& fen,
                  const MoveList&    moves )
    {
        if ( fen.empty() )
        {
            XQWLight::init_game();
        }
        else
        {
            unsigned char board[10][9];
            char          side = 'w';
            if ( ! _convertFENtoBoard( fen, board, side ) )
            {
                return hoxAI_RC_ERR;
            }
            XQWLight::init_game( board, side );
        }
        return hoxAI_RC_OK;
    }

	std::string generateMove()
    {
        return XQWLight::generate_move();
    }

    void onHumanMove( const std::string& sMove )
    {
        XQWLight::on_human_move( sMove );
    }

private:
    bool _convertFENtoBoard( const std::string& fen,
                             unsigned char      board[10][9],
                             char&              side ) const;

private:
    std::string m_name;

}; /* class AIEngineImpl */

bool
AIEngineImpl::_convertFENtoBoard( const std::string& fen,
                                  unsigned char      board[10][9],
                                  char&              side ) const
{
    int r  = 0; // Row ... or the index of horizontal lines.
    int c  = 0; // Column ... or the index of vertical lines.

    for ( r = 0; r < 10; ++r )
    {
        for ( c = 0; c < 9; ++c )
        {
            board[r][c] = 0;
        }
    }

    r = 0;
    c = 0;
    for ( std::string::const_iterator it = fen.begin();
                                      it != fen.end(); ++it )
    {
        if ( *it >= '1' && *it <= '9' )
        {
            c += *it - '0';
        }
        else if ( *it == '/' )
        {
            ++r;
            c = 0;
        }
        else if ( *it == ' ' )
        {
            if ( ++it != fen.end() )
            {
                side = *it;
            }
            break;
        }
        else
        {
            const int color = ( *it < 'a' ? 0x08 : 0x10 );
            int cType = *it;
            if ( cType >= 'a' )
            {
                cType -= 'a' - 'A';  // ... to uppercase.
            }

            int type = 0;
            switch ( cType )
            {
                case 'K': type = 0; break;
                case 'A': type = 1; break;
                case 'E': type = 2; break;
                case 'R': type = 4; break;
                case 'H': type = 3; break;
                case 'C': type = 5; break;
                case 'P': type = 6; break;
                default: return false; /* failure */
            }

            board[r][c] = color + type;
            ++c;
        }
    }

    return true; // success
}

////////////////// END OF AIEngineImpl ////////////////////////////////////////

AIEngineLib* CreateAIEngineLib()
{
  return new AIEngineImpl("XQWLight Engine Lib");
}

/************************* END OF FILE ***************************************/
