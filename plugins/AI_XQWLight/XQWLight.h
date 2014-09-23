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
// Name:            XQWLight.h
// Created:         10/11/2008
//
// Description:     This is 'XQWLight' Engine to interface with HOXChess.
//                  XQWLight is an open-source (?) Xiangqi AI Engine
//                  written by Huang Chen at www.elephantbase.net
//
//  (Original Chinese URL)
//        http://www.elephantbase.net/computer/stepbystep1.htm
//
//  (Translated English URL using Goold Translate)
//       http://74.125.93.104/translate_c?hl=en&langpair=
//         zh-CN|en&u=http://www.elephantbase.net/computer/stepbystep1.htm&
//         usg=ALkJrhj7W0v3J1P-xmbufsWzYq7uKciL1w
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_XQWLIGHT_HOX_ENGINE_H__
#define __INCLUDED_XQWLIGHT_HOX_ENGINE_H__

#include <string>

namespace XQWLight
{
	/* PUBLIC API */

    void init_engine( int searchDepth );

	void init_game( unsigned char board[10][9] = NULL,
                    const char    side = 'w' );

	std::string generate_move();
    void        on_human_move( const std::string& sMove );

    void set_search_time( int nSeconds );
	    /* Only approximately... */


    /* PRIVATE API (declared here for documentation purpose) */

	unsigned int _hox2xqwlight( const std::string& sMove );
	std::string _xqwlight2hox( unsigned int move );

} // namespace XQWLight

#endif /* __INCLUDED_XQWLIGHT_HOX_ENGINE_H__ */
