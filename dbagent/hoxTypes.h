//
// C++ Interface: hoxTypes
//
// Description:     Containing simple types commonly used through out 
//                  the project.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_TYPES_H_
#define __INCLUDED_HOX_TYPES_H_

#include <string>
#include <list>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>
#include "hoxEnums.h"

/* Forward declarations */
class hoxResponse;

/**
 * Typedef(s)
 */

typedef boost::shared_ptr<hoxResponse> hoxResponse_SPtr;

/**
 * Container for parameters.
 */
typedef std::map<const std::string, std::string> hoxParameters;

/**
 * Request comming from the remote Players.
 */
class hoxRequest
{
public:
    hoxRequest();
    hoxRequest(const std::string& requestStr);

    bool isValid() const { return _type != hoxREQUEST_UNKNOWN; }

    const hoxRequestType getType() const { return _type; }
    const hoxParameters& getParameters() const { return _parameters; }
    const std::string getParam(const std::string& key) const
        { return const_cast<hoxRequest*>(this)->_parameters[key]; }

    const std::string toString() const;

private:
    hoxRequestType  _type;
    hoxParameters   _parameters;

};
typedef boost::shared_ptr<hoxRequest> hoxRequest_SPtr;

/**
 * Response being returned to the remote Players.
 */
class hoxResponse
{
public:
    explicit hoxResponse( hoxRequestType type,
                          hoxResult      code = hoxRC_OK );
    hoxResponse( const hoxResponse& other ); // Copy constructor.
    ~hoxResponse();

    void setCode(hoxResult code) { _code = code; }
    void setTid(const std::string& tid) { _tid = tid; } 
    void setContent(const std::string& content) { _content = content; }

    const std::string toString() const;

    /* ---------- */
    /* Static API */
    /* ---------- */
public:
    static hoxResponse_SPtr
    create_result_HELLO();

private:
    const hoxRequestType  _type;
    hoxResult             _code;
    std::string           _tid;   // Table-Id (if applicable).
    std::string           _content;
};


#endif /* __INCLUDED_HOX_TYPES_H_ */
