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
 * 'Folium' AI Engine Plugin.
 */


#include <AIEngineLib.h>
#include <DefaultDelete.h>
#include <memory>
#include "folHOXEngine.h"

class AIEngineImpl : public DefaultDelete<AIEngineLib>
{
public:
    AIEngineImpl( const char* engineName )
        : m_name( engineName ? engineName : "__UNKNOWN__" )
    {
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
    }

  	int initGame( const std::string& fen,
                  const MoveList&    moves )
    {
        //if ( ! fen.empty() ) return hoxAI_RC_NOT_SUPPORTED;

        m_engine.reset( new folHOXEngine( ""/*fen*/ ) );

        for ( MoveList::const_iterator it = moves.begin();
                                       it != moves.end(); ++it)
        {
            m_engine->OnHumanMove( *it );
        }

        return hoxAI_RC_OK;
    }

	std::string generateMove()
    {
        return m_engine->GenerateMove();
    }

    void onHumanMove( const std::string& sMove )
    {
        m_engine->OnHumanMove( sMove );
    }


private:
    std::string    m_name;

    typedef std::auto_ptr<folHOXEngine>  Engine_APtr;
    Engine_APtr    m_engine;

}; /* class AIEngineImpl */


//////////////////////////////////////////////////////////////
AIEngineLib* CreateAIEngineLib()
{
  return new AIEngineImpl("Folium Engine Lib");
}

/************************* END OF FILE ***************************************/
