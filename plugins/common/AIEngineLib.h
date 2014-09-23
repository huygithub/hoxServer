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
// Name:            AIEngineLib.h
// Created:         02/17/2009
//
// Description:     The interface of an AI Engine Plugin.
//
// Copyright:
//   This file is based on the following:
// ---------------------------------------------------------------------------
//   DLL Interface classes for C++ Binary Compatibility article
//   article at http://aegisknight.org/cppinterface.html
//
//   code author:     Ben Scott   (bscott@iastate.edu)
//   article author:  Chad Austin (aegis@aegisknight.org)
// ---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_AI_ENGINE_LIB_H__
#define __INCLUDED_AI_ENGINE_LIB_H__

#ifdef WIN32
  #ifdef EXPORTING
    #define CALL __declspec(dllexport)
  #else
    #define CALL __declspec(dllexport)
  #endif
#else
#define CALL
#endif

#include <string>
#include <list>

/**
 * Plugin error codes (or Return-Codes).
 */
#define hoxAI_RC_UNKNOWN       -1
#define hoxAI_RC_OK             0  /* A generic success       */
#define hoxAI_RC_ERR            1  /* A generic error         */
#define hoxAI_RC_NOT_FOUND      2  /* Something not found     */
#define hoxAI_RC_NOT_SUPPORTED  3  /* Something not supported */

/**
 * Typdefs
 */
typedef std::list<std::string> MoveList;

/**
 * AIEngineLib interface.
 */
class AIEngineLib
{
public:
    virtual void        destroy() = 0;
    virtual void        initEngine( int searchDepth = 0 ) = 0;

    // ------------
    // ............................................. fen - Forsyth-Edwards Notation (FEN)
	virtual int         initGame( const std::string& fen,
                                  const MoveList&    moves ) = 0;
	virtual std::string generateMove() = 0;
    virtual void        onHumanMove( const std::string& sMove ) = 0;
    // ------------

    void operator delete(void* p)
        {
            if (p)
            {
                AIEngineLib* engine = static_cast<AIEngineLib*>(p);
                engine->destroy();
            }
        }
};


extern "C" CALL AIEngineLib* CreateAIEngineLib();

typedef AIEngineLib* (*PICreateAIEngineLibFunc)();

#endif /* __INCLUDED_AI_ENGINE_LIB_H__ */
