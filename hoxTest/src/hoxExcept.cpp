//
// C++ Implementation: hoxExcept
//
// Description:   Containing exception and error definitions.
//
//
// Author: Huy Phan, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hoxExcept.h"
#include <cstdio>      // snprintf

// =========================================================================
//
//                        hoxError class
//
// =========================================================================

const std::string
hoxError::toString() const
{
    char szBuffer[32];
    snprintf(szBuffer, sizeof(szBuffer), "%d", _code);

    std::string result = std::string(szBuffer) + " " + this->what();

    return result;
}

// =========================================================================
//
//                        hoxTableError class
//
// =========================================================================

const std::string
hoxTableError::toString() const
{
    std::string result = hoxError::toString() 
                       + " (" + _tid + ")";
    return result;
}

/******************* END OF FILE *********************************************/
