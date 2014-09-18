//
// C++ Interface: hoxFileMgr
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_FILE_MGR_H__
#define __INCLUDED_HOX_FILE_MGR_H__

#include <map>
#include "hoxTypes.h"

/**
 * Representing a single file on disk.
 */
class hoxFile
{
public:
    std::string   m_sPath;
    std::string   m_sContent;
    std::string   m_sType;   // File type ("text/css", "image/png", ...)

    hoxFile( const std::string& sPath ) : m_sPath( sPath ) {}
};

typedef boost::shared_ptr<hoxFile> hoxFile_SPtr;


/**
 * The Manager to manage local files on disk.
 * This class is implemented as a singleton.
 */
class hoxFileMgr
{
private:
    static hoxFileMgr* s_instance;  // The singleton instance.

    typedef std::map<std::string, hoxFile_SPtr> FileContainer;

public:
    static hoxFileMgr* getInstance();
    static bool        m_bCacheEnabled;
    static bool        m_bErrorIfNotInCache;

public:
    ~hoxFileMgr() {}

    void         setLocation( const std::string& s ) { _sLocation = s; }
    void         preloadFile( const std::string& sFile );
    hoxFile_SPtr getFile( const std::string& sPath );
    void         clearCache();

private:
    hoxFileMgr() {}

    /**
     * Load a given file from disk.
     */
    hoxFile_SPtr _loadFile( const std::string& sFile );

    /**
     * Determine the Content-Type based on the HTTP path.
     */
    const std::string _getHttpContentType( const std::string& sPath ) const;

private:
    std::string    _sLocation;
    FileContainer  _files;
};

#endif /* __INCLUDED_HOX_FILE_MGR_H__ */
