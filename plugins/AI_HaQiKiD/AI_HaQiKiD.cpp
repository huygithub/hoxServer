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

/*
 * External dependencies (defined in 'haqikidHOX.cpp')
 */

extern void        InitEngine();
extern void        InitGame();
extern const char* GenerateNextMove();
extern void        OnOpponentMove(const char *line);
extern void        DeInitEngine();
extern void        SetMaxDepth( int searchDepth );

/*
 * AI Engine Implementation
 */

class AIEngineImpl : public DefaultDelete<AIEngineLib>
{
public:
    AIEngineImpl(const char* engineName)
    {
        m_name = engineName ? engineName : "__UNKNOWN__";
    }

    ~AIEngineImpl()
    {
        DeInitEngine();
    }

    void destroy()
    {
        delete this;
    }

    void initEngine( int nAILevel = 0 )
    {
        setDifficultyLevel( nAILevel == 0 ? 5 : nAILevel );
        ::InitEngine();
    }

  	int initGame( const std::string& fen,
                  const MoveList&    moves )
    {
        //if ( ! fen.empty() ) return hoxAI_RC_NOT_SUPPORTED;

        ::InitGame();

        for ( MoveList::const_iterator it = moves.begin();
                                       it != moves.end(); ++it)
        {
            std::string stdMove = _hoxToMove( *it );
            ::OnOpponentMove( stdMove.c_str() );
        }

        return hoxAI_RC_OK;
    }

	std::string generateMove()
    {
        const char* szMove = ::GenerateNextMove();
        return _moveToHox( std::string( szMove ) );
    }

    void onHumanMove( const std::string& sMove )
    {
        std::string stdMove = _hoxToMove( sMove );
        ::OnOpponentMove( stdMove.c_str() );
    }

    int setDifficultyLevel( int nAILevel )
    {
        int searchDepth = 1;

        if      ( nAILevel > 10 ) searchDepth = 10;
        else if ( nAILevel < 1 )  searchDepth = 1;
        else                      searchDepth = nAILevel;

        ::SetMaxDepth( searchDepth );
        return hoxAI_RC_OK;
    }

    std::string getInfo()
    {
        return "H.G. Muller\n"
               "home.hccnet.nl/h.g.muller/XQhaqikid.html";
    }

private:
    std::string _hoxToMove( const std::string& sIn );
    std::string _moveToHox( const std::string& sIn );

private:
    std::string m_name;

}; /* class AIEngineImpl */

std::string
AIEngineImpl::_hoxToMove( const std::string& sIn )
{
    std::string stdMove;
    stdMove += (sIn[0] + 'a' - '0'); 
    stdMove += ('9' + '0' - sIn[1]); 
    stdMove += (sIn[2] + 'a' - '0'); 
    stdMove += ('9' + '0' - sIn[3]); 
    return stdMove;
}

std::string
AIEngineImpl::_moveToHox( const std::string& sIn )
{
    std::string sMove;
    sMove += sIn[0] - 'a' + '0';
    sMove += '9' + '0' - sIn[1];
    sMove += sIn[2] - 'a' + '0';
    sMove += '9' + '0' - sIn[3];
    return sMove;
}


//////////////////////////////////////////////////////////////
AIEngineLib* CreateAIEngineLib()
{
  return new AIEngineImpl("HaQiKi D Engine Lib");
}

/************************* END OF FILE ***************************************/
