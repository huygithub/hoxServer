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
// Name:            DefaultDelete.h
// Created:         02/17/2009
//
// Description:     The default deleter for an AI Engine Plugin.
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

#ifndef __INCLUDED_DEFAULT_DELETE_H__
#define __INCLUDED_DEFAULT_DELETE_H__

template<typename T>
class DefaultDelete : public T
{
public:
    void operator delete(void* p)
    {
        ::operator delete(p);
    }
};

#endif /* __INCLUDED_DEFAULT_DELETE_H__ */
