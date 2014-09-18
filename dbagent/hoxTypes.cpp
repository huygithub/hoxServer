//
// C++ Implementation: hoxTypes
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <string>
#include <sstream>
#include <iostream>
#include <boost/tokenizer.hpp>
#include "hoxTypes.h"
#include "hoxUtil.h"


// =========================================================================
//
//                        hoxRequest class
//
// =========================================================================

hoxRequest::hoxRequest()
        : _type( hoxREQUEST_UNKNOWN )
{
}

hoxRequest::hoxRequest( const std::string& requestStr )
        : _type( hoxREQUEST_UNKNOWN )
{
    typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
    typedef boost::escaped_list_separator<char> Separator;

    Separator sep('\\', '&', '\"');
    Tokenizer tok(requestStr, sep);
    std::string name;
    std::string value;
    std::string::size_type found_index;

    for ( Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it )
    {
        /* Parse the "name=value" pair. */
        found_index = it->find_first_of('=');
        if ( found_index == std::string::npos ) // not found?
        {
            name = (*it);
            value = "";
        }
        else
        {
            name = it->substr(0, found_index);
            value = it->substr(found_index+1);
        }

        /* Convert to request-type if this is 'OP' name. */
        if ( name == "op" )
        {
            _type = hoxUtil::stringToRequestType( value );
        }
        else
        {
            _parameters[name] = value;
        }
    }

}

const std::string
hoxRequest::toString() const
{
    std::string result;

    result = "op=" + hoxUtil::requestTypeToString( _type );

    for ( hoxParameters::const_iterator it = _parameters.begin();
                                        it != _parameters.end(); ++it )
    {
        result += "&" + it->first + "=" + it->second;
    }

    return result;
}

// =========================================================================
//
//                        hoxResponse class
//
// =========================================================================

hoxResponse::hoxResponse( hoxRequestType type,
                          hoxResult      code /* = hoxRC_OK */ )
        : _type( type )
        , _code( code )
{
}

hoxResponse::hoxResponse( const hoxResponse& other )
        : _type( other._type )
        , _code( other._code )
{
    _content = other._content;
}

hoxResponse::~hoxResponse()
{
}

const std::string
hoxResponse::toString() const
{
    std::ostringstream outStream;

    outStream << "op=" << hoxUtil::requestTypeToString( _type )
              << "&code=" << _code;
    
    if ( ! _tid.empty() )
    {
        outStream << "&tid=" << _tid;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // NOTE: Make sure that the output is terminated by only
    //       (and only) TWO end-of-line characters. 
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    outStream << "&content=" << _content;

    return outStream.str();
}

/*static*/
hoxResponse_SPtr
hoxResponse::create_result_HELLO()
{
    std::ostringstream  outStream;

    outStream << "I_got_it"
              << "\n\n";

    hoxResponse_SPtr pResponse( new hoxResponse( hoxREQUEST_HELLO ) );
    pResponse->setContent( outStream.str() );

    return pResponse;
}

/******************* END OF FILE *********************************************/
