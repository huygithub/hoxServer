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
// Name:            folHOXEngine.h
// Created:         10/09/2008
//
// Description:     This is 'folium' Engine to interface with HOXChess.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_FOL_HOX_ENGINE_H__
#define __INCLUDED_FOL_HOX_ENGINE_H__

#include <string>
#include <memory>   // auto_ptr

class folEngine;   // Forward declaration.

class folHOXEngine
{
public:
    folHOXEngine( const std::string& fen,
                  const int          searchDepth = 3 );
    ~folHOXEngine();

    std::string GenerateMove();
    void OnHumanMove( const std::string& sMove );

    void SetSearchDepth( int searchDepth ) { _searchDepth = searchDepth; }
    int  GetSearchDepth() const { return _searchDepth; }

private:
    unsigned int _hox2folium( const std::string& sMove ) const;
    std::string _folium2hox( unsigned int move ) const;

private:
    folEngine*       _engine;
        /* NOTE: I cannot use std::auto_ptr<...> here because
         *       it generates a compiler warning due to incomplete type.
         */

    int              _searchDepth;
};

#endif /* __INCLUDED_FOL_HOX_ENGINE_H__ */
