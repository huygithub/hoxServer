//
// C++ Implementation: hoxFileMgr
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hoxFileMgr.h"
#include "hoxDbClient.h"
#include "hoxExcept.h"
#include "hoxLog.h"

// =========================================================================
//
//                        hoxFileMgr
//
// =========================================================================

/* Define the static singleton instance. */
hoxFileMgr* hoxFileMgr::s_instance = NULL;
bool        hoxFileMgr::m_bCacheEnabled = false;
bool        hoxFileMgr::m_bErrorIfNotInCache = false;

/*static*/
hoxFileMgr*
hoxFileMgr::getInstance()
{
    if ( hoxFileMgr::s_instance == NULL )
    {
        hoxFileMgr::s_instance = new hoxFileMgr();
    }

    return hoxFileMgr::s_instance;
}

void
hoxFileMgr::preloadFile( const std::string& sFile )
{
    const char* FNAME = "hoxFileMgr::preloadFile";
    hoxLog(LOG_DEBUG, "%s: ENTER [%s].", FNAME, sFile.c_str());

    try
    {
        const std::string sFullPath = _sLocation + sFile;
        const hoxFile_SPtr pFile = _loadFile( sFullPath );
        _files[sFile] = pFile;
    }
    catch( hoxError ex )
    {
        hoxLog(LOG_WARN, "%s: Error caught [%d %s].", FNAME, ex.code(), ex.toString().c_str());
    }
}

hoxFile_SPtr
hoxFileMgr::getFile( const std::string& sPath )
{
    const char* FNAME = "hoxFileMgr::getFile";
    hoxFile_SPtr pFile;

    /* Lookup the file's content in the Cache first.
     * If found, then simply return the "cached".
     */
    if ( hoxFileMgr::m_bCacheEnabled )
    {
        pFile = _files[sPath];
        if ( pFile )
        {
            hoxLog(LOG_DEBUG, "%s: Found in cache file [%s].", FNAME, sPath.c_str());
            return pFile;
        }
        if ( hoxFileMgr::m_bErrorIfNotInCache )
        {
            pFile.reset( new hoxFile( sPath ) );
            pFile->m_sContent = "File not found";
            pFile->m_sType    = "text/html";
            hoxLog(LOG_WARN, "%s: File [%s] not in cache.", FNAME, sPath.c_str());
            return pFile;
        }
    }

    /* If not found, then load the file from disk. */

    try
    {
        pFile = _loadFile( sPath );

        if ( hoxFileMgr::m_bCacheEnabled )
        {
            _files[sPath] = pFile;
        }
    }
    catch( hoxError ex )
    {
        pFile->m_sContent = "Error caught :" + ex.toString();
        pFile->m_sType    = "text/html";
        hoxLog(LOG_WARN, "%s: Error caught [%d %s].", FNAME, ex.code(), ex.toString().c_str());
        pFile.reset(); // Signal "error".
    }

    return pFile;
}

void
hoxFileMgr::clearCache()
{
    const char* FNAME = "hoxFileMgr::clearCache";
    hoxLog(LOG_INFO, "%s: ENTER. Cache-size = [%d]", FNAME, _files.size());
    _files.clear();
}

hoxFile_SPtr
hoxFileMgr::_loadFile( const std::string& sFile )
{
    const char* FNAME = "hoxFileMgr::_loadFile";
    hoxFile_SPtr pFile( new hoxFile( sFile ) );

    try
    {
        std::string  sFileContent;
        if ( hoxRC_OK != hoxDbClient::get_http_file( sFile, sFileContent ) )
        {
            throw hoxError(hoxRC_NOT_FOUND, "Failed to get HTTP file: " + sFile);
        }
        pFile->m_sContent = sFileContent;
        pFile->m_sType    = _getHttpContentType( sFile );
    }
    catch( hoxError ex )
    {
        hoxLog(LOG_WARN, "%s: Error caught [%d %s].", FNAME, ex.code(), ex.toString().c_str());
        pFile.reset(); // Signal "error".
        throw;
    }

    return pFile;
}

const std::string
hoxFileMgr::_getHttpContentType( const std::string& sPath ) const
{
    std::string sExtension;    // The file's extension.
    std::string::size_type loc = sPath.find_last_of( '.' );
    if (  loc != std::string::npos )
    {
        sExtension = sPath.substr( loc+1 );
    }

    if      ( sExtension == "js" )  return "application/javascript";
    else if ( sExtension == "png" ) return "image/png";
    else if ( sExtension == "css" ) return "text/css";
    /* else */                      return "text/html";
}

/******************* END OF FILE *********************************************/
