//
// C++ Implementation: hoxDBAPI
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "hoxDBAPI.h"
#include "hoxLog.h"
#include "hoxUtil.h"

#include <sqlite3.h>

/* ------------------------------------------------------------------------- *
 * Constants
 * ------------------------------------------------------------------------- */

#define DB_NAME  "../database/hoxserver.db"


/* ------------------------------------------------------------------------- *
 * Private API
 * ------------------------------------------------------------------------- */

static int
_callback_GET( void *pArg, 
               int  argc, 
               char **argv, 
               char **azColName )
{
    hoxDBAPI::Player_t* player = (hoxDBAPI::Player_t*) pArg;
    //hoxLog(LOG_DEBUG, "%s: argc=[%d]. Player-Id = [%s].", __FUNCTION__, argc, player->id.c_str());

    std::string sColumn;
    for ( int i = 0; i < argc; i++ )
    {
        sColumn = ( azColName[i] ? azColName[i] : "" );
        //hoxLog(LOG_DEBUG, "... %s = %s", sColumn.c_str(), argv[i] ? argv[i] : "NULL");

        if      ( sColumn == "score" )     player->score  = ::atoi( argv[i] );
        else if ( sColumn == "wins" )      player->wins   = ::atoi( argv[i] );
        else if ( sColumn == "draws" )     player->draws  = ::atoi( argv[i] );
        else if ( sColumn == "losses" )    player->losses = ::atoi( argv[i] );
        else if ( sColumn == "hpassword" ) player->hpw    = argv[i];
        else if ( sColumn == "email" )     player->email  = argv[i];
    }
    
    return 0;
}

/* ------------------------------------------------------------------------- *
 * Public API
 * ------------------------------------------------------------------------- */

hoxResult
hoxDBAPI::put_player_info( const Player_t&    playerInfo,
                           const std::string& sEmail )
{
    const char* FNAME = "hoxDBAPI::put_player_info";
    sqlite3*    db = NULL;
    int         rc = 0;
    char*       szErrMsg = NULL;
    std::string sSQL;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, playerInfo.id.c_str());

    rc = sqlite3_open( DB_NAME, &db );
    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: Can't open database: [%s].", FNAME, sqlite3_errmsg(db));
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sSQL = std::string("INSERT INTO players (pid, score, hpassword, email) VALUES ")
        + "('" + playerInfo.id + "'"
        + " ,'" + hoxUtil::intToString( playerInfo.score ) + "'"
        + " ,'" + playerInfo.hpw + "'"
        + " ,'" + sEmail + "'"
        + ")";

    rc = sqlite3_exec( db, 
                       sSQL.c_str(), 
                       NULL,  /* callback  */
                       NULL,  /* 1st argument to callback */
                       &szErrMsg );

    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: SQL error: [%s].", FNAME, szErrMsg);
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sqlite3_close( db );

    return hoxRC_OK;
}

hoxResult
hoxDBAPI::get_player_info( const std::string& pid,
                           Player_t&          playerInfo )
{
    const char* FNAME = "hoxDBAPI::get_player_info";
    sqlite3*    db = NULL;
    int         rc = 0;
    char*       szErrMsg = NULL;
    std::string sSQL;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, pid.c_str());

    playerInfo.id = pid;

    rc = sqlite3_open( DB_NAME, &db );
    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: Can't open database: [%s].", FNAME, sqlite3_errmsg(db));
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sSQL = "SELECT * FROM players WHERE pid = '" + pid + "'" + "LIMIT 1";

    rc = sqlite3_exec( db, 
                       sSQL.c_str(), 
                       _callback_GET, 
                       &playerInfo, /* 1st argument to callback */
                       &szErrMsg );

    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: SQL error: [%s].", FNAME, szErrMsg);
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sqlite3_close( db );

    return hoxRC_OK;
}

hoxResult
hoxDBAPI::set_player_info( const Player_t&    playerInfo,
                           const std::string& sGameResult )
{
    const char* FNAME = "hoxDBAPI::set_player_info";
    sqlite3*    db = NULL;
    int         rc = 0;
    char*       szErrMsg = NULL;
    std::string sSQL;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, playerInfo.id.c_str());

    rc = sqlite3_open( DB_NAME, &db );
    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: Can't open database: [%s].", FNAME, sqlite3_errmsg(db));
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sSQL = "UPDATE players SET score = '" 
        + hoxUtil::intToString(playerInfo.score) + "'";
    if      ( sGameResult == "W" ) sSQL += ", wins = wins+1";
    else if ( sGameResult == "D" ) sSQL += ", draws = draws+1";
    else if ( sGameResult == "L" ) sSQL += ", losses = losses+1";
    sSQL += " WHERE pid = '" + playerInfo.id + "'";

    rc = sqlite3_exec( db, 
                       sSQL.c_str(), 
                       NULL,
                       NULL, /* 1st argument to callback */
                       &szErrMsg );

    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: SQL error: [%s].", FNAME, szErrMsg);
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sqlite3_close( db );

    return hoxRC_OK;
}

hoxResult
hoxDBAPI::set_profile_info( const std::string& pid,
                            const std::string& sEmail,
                            const std::string& sPassword )
{
    const char* FNAME = "hoxDBAPI::set_profile_info";
    sqlite3*    db = NULL;
    int         rc = 0;
    char*       szErrMsg = NULL;
    std::string sSQL;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, pid.c_str());

    rc = sqlite3_open( DB_NAME, &db );
    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: Can't open database: [%s].", FNAME, sqlite3_errmsg(db));
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sSQL = "UPDATE players SET email = '" + sEmail + "'";
    if ( ! sPassword.empty() ) sSQL += ", hpassword = '" + sPassword + "'";
    sSQL += " WHERE pid = '" + pid + "'";

    rc = sqlite3_exec( db, 
                       sSQL.c_str(), 
                       NULL,
                       NULL, /* 1st argument to callback */
                       &szErrMsg );

    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: SQL error: [%s].", FNAME, szErrMsg);
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sqlite3_close( db );

    return hoxRC_OK;
}

hoxResult
hoxDBAPI::set_player_password( const Player_t& playerInfo )
{
    const char* FNAME = "hoxDBAPI::set_player_password";
    sqlite3*    db = NULL;
    int         rc = 0;
    char*       szErrMsg = NULL;
    std::string sSQL;

    hoxLog(LOG_DEBUG, "%s: ENTER. pid = [%s].", FNAME, playerInfo.id.c_str());

    rc = sqlite3_open( DB_NAME, &db );
    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: Can't open database: [%s].", FNAME, sqlite3_errmsg(db));
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sSQL = "UPDATE players SET hpassword = '" + playerInfo.hpw + "'"
        + " WHERE pid = '" + playerInfo.id + "'";

    rc = sqlite3_exec( db, 
                       sSQL.c_str(), 
                       NULL,
                       NULL, /* 1st argument to callback */
                       &szErrMsg );

    if( rc != SQLITE_OK )
    {
        hoxLog(LOG_ERROR, "%s: SQL error: [%s].", FNAME, szErrMsg);
        sqlite3_close( db );
        return hoxRC_ERR;
    }

    sqlite3_close( db );

    return hoxRC_OK;
}

/******************* END OF FILE *********************************************/
