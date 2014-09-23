//
// C++ Interface: hoxExcept
//
// Description:   Containing exception and error definitions.
//
// Author: Huy Phan, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_EXCEPT_H__
#define __INCLUDED_HOX_EXCEPT_H__

#include <stdexcept>
#include <string>
#include "hoxCommon.h"

/**
 * Generic Error.
 */
class hoxError : public std::runtime_error
{
public:
    explicit hoxError( const hoxResult    code, 
                       const std::string& what )
                : std::runtime_error( what )
                , _code (code ) {}
    
    virtual ~hoxError() throw() {}

    hoxResult code() const throw() { return _code; }

    virtual const std::string toString() const;

private:
    const hoxResult _code;
};

/**
 * Table Error.
 */
class hoxTableError : public hoxError
{
public:
    explicit hoxTableError( const hoxResult    code,
                            const std::string& tid,
                            const std::string& what )
                : hoxError( code, what )
                , _tid( tid ) {}
    
    virtual ~hoxTableError() throw() {}

    virtual const std::string toString() const;

    std::string tid() const throw() { return _tid;  }

private:
    const std::string  _tid;   // Table-Id (if applicable).
};

#endif /* __INCLUDED_HOX_EXCEPT_H__ */
