//
// C++ Interface: hoxExcept
//
// Description:   Containing exception and error definitions.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_EXCEPT_H_
#define __INCLUDED_HOX_EXCEPT_H_

#include <stdexcept>
#include <string>
#include "hoxEnums.h"

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

#endif /* __INCLUDED_HOX_EXCEPT_H_ */
