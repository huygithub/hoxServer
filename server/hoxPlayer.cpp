//
// C++ Implementation: hoxPlayer
//
// Description: The Player
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/15/2009
//

#include "hoxPlayer.h"
#include "hoxTable.h"
#include "hoxSession.h"
#include "hoxDebug.h"
#include "hoxLog.h"
#include "hoxExcept.h"
#include "hoxUtil.h"


hoxPlayer::hoxPlayer(const std::string&  id,
                     const hoxPlayerType type /* = hoxPLAYER_TYPE_NORMAL */ )
        : _id( id )
        , _type( type )
        , _score( 0 )
        , _wins( 0 )
        , _draws( 0 )
        , _losses( 0 )
{
    const char* FNAME = "hoxPlayer::hoxPlayer";
    hoxLog(LOG_DEBUG, "%s: (%s) ENTER.", FNAME, _id.c_str());
}

hoxPlayer::~hoxPlayer()
{
    const char* FNAME = "hoxPlayer::~hoxPlayer";
    hoxLog(LOG_DEBUG, "%s: (%s) ENTER.", FNAME, _id.c_str());
}

void
hoxPlayer::onNewEvent( const hoxResponse_SPtr& event )
{
    if ( _session )
    {
        _session->addResponse( event );
    }
}

void
hoxPlayer::joinTableAs( const hoxTable_SPtr& pTable,
                        const hoxColor       requestColor )
{
    hoxCHECK_RET(pTable, "Table is NULL");

    if ( hoxRC_OK != pTable->assignPlayerAs( shared_from_this(),
                                             requestColor ) )
    {
        throw hoxError(hoxRC_ERR, "Failed to join table");
    }

    _tables.insert( pTable );  // Make a copy.
}

void
hoxPlayer::leaveTable( const hoxTable_SPtr& pTable )
{
    const char* FNAME = "hoxPlayer::leaveTable";

    hoxCHECK_RET(pTable, "Table is NULL");

    hoxLog(LOG_DEBUG, "%s: Player [%s] is leaving table [%s]...",
        FNAME, this->getId().c_str(), pTable->getId().c_str() );

    pTable->onLeave_FromPlayer( shared_from_this() );
    _tables.erase( pTable );
}

void
hoxPlayer::leaveAllTables()
{
    const char* FNAME = "hoxPlayer::leaveAllTables";

    hoxLog(LOG_DEBUG, "%s: (%s) ENTER.", FNAME, _id.c_str());

    hoxTable_SPtr pTable;
    while ( ! _tables.empty() )
    {
        pTable = *( _tables.begin() );
        this->leaveTable( pTable ); // NOTE: The table is also removed from the list.
    }
}

bool
hoxPlayer::isAtTable( const hoxTable_SPtr& pTable ) const
{
    return ( _tables.find( pTable ) != _tables.end() );
}

void
hoxPlayer::doMove( const std::string&  tableId,
                   const std::string&  sMove )
{
    const char* FNAME = "hoxPlayer::doMove";

    hoxTable_SPtr pTable = _findTable(tableId);

    if ( hoxRC_OK != pTable->acceptMove( shared_from_this(), sMove ) )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] did not accept Move.",
            FNAME, _id.c_str(), tableId.c_str());
        throw hoxError(hoxRC_ERR, "Move not OK");
    }
}

void
hoxPlayer::offerResign( const std::string&  tableId )
{
    const char* FNAME = "hoxPlayer::offerResign";

    hoxTable_SPtr pTable = _findTable(tableId);

    if ( hoxRC_OK != pTable->handleResignRequest( shared_from_this() ) )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] failed to handle Resign-Request.",
            FNAME, _id.c_str(), tableId.c_str());
        throw hoxTableError( hoxRC_NOT_VALID, pTable->getId(),
                             "Table failed to handle RESIGN" );
    }
}

void
hoxPlayer::offerDraw( const std::string&  tableId )
{
    const char* FNAME = "hoxPlayer::offerDraw";

    hoxTable_SPtr pTable = _findTable(tableId);

    if ( hoxRC_OK != pTable->handleDrawRequest( shared_from_this() ) )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] failed to handle Draw-Request.",
            FNAME, _id.c_str(), tableId.c_str());
        throw hoxTableError( hoxRC_NOT_VALID, pTable->getId(),
                             "Table failed to handle DRAW" );
    }
}

void
hoxPlayer::resetTable( const std::string&  tableId )
{
    const char* FNAME = "hoxPlayer::resetTable";

    hoxTable_SPtr pTable = _findTable(tableId);

    if ( hoxRC_OK != pTable->handleResetRequest( shared_from_this() ) )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] failed to handle Reset-Request.",
            FNAME, _id.c_str(), tableId.c_str());
        throw hoxTableError( hoxRC_NOT_VALID, pTable->getId(),
                             "Table failed to handle RESET" );
    }
}

hoxResult
hoxPlayer::updateTable( const std::string& tableId,
                        const bool         bRatedGame,
                        const hoxTimeInfo& newInitialTime )
{
    const char* FNAME = "hoxPlayer::updateTable";

    hoxTable_SPtr pTable = _findTable(tableId);

    if ( hoxRC_OK != pTable->handleUpdateRequest( shared_from_this(),
                                                  bRatedGame, newInitialTime ) )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] failed to handle Update-Request.",
            FNAME, _id.c_str(), tableId.c_str());
        throw hoxTableError( hoxRC_NOT_VALID,
                             pTable->getId(),
                             "Table failed to handle UPDATE" );
    }

    return hoxRC_OK;
}

void
hoxPlayer::resumePlayingIfNeeded()
{
    for ( hoxTableSet::const_iterator it = _tables.begin();
                                      it != _tables.end(); ++it )
    {
        (*it)->sendInfoToPlayer( shared_from_this() );
    }
}

hoxTable_SPtr
hoxPlayer::_findTable( const std::string& tableId,
                       bool bThrowErrorIfNotFound /* = true */ ) const
{
    hoxTable_SPtr pTable;
    for ( hoxTableSet::const_iterator it = _tables.begin();
                                      it != _tables.end(); ++it )
    {
        if ( (*it)->getId() == tableId )
        {
            pTable = (*it);
            return pTable;
        }
    }

    if ( bThrowErrorIfNotFound )
    {
        hoxLog(LOG_INFO, "%s: (%s) Table [%s] not found.", __FUNCTION__,
            _id.c_str(), tableId.c_str());
        throw hoxError(hoxRC_NOT_FOUND, "Table not found");
    }
    return pTable;
}

/******************* END OF FILE *********************************************/
