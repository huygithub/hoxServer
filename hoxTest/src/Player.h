//
// C++ Interface: Player
//
// Description:  The generic Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created: 04/18/2009
//

#ifndef __INCLUDED_PLAYER_H__
#define __INCLUDED_PLAYER_H__

#include <string>
#include <st.h>
#include "hoxCommand.h"

/**
 * The Player
 */
class Player
{
public:
    Player( const std::string& id,
            const std::string& password );
    virtual ~Player();

    const std::string getId() const { return m_id; }

    /* Interface (main) API */

    virtual void run();

    /* Public API */

    void connect( const std::string&       sHost,
                  const unsigned short int nPort );
    void disconnect();
    void login();
    void logout();
    void openNewTable( hoxTableInfo&      tableInfo,
                       const std::string& sRole = "Red" );
    void joinTable( const std::string& sRole );
    void leaveCurrentTable();
    void inviteOther( const std::string& oid );
    void sendMove( const std::string& sMove );
    void sendDraw( const std::string& tid );
    void sendKeepAlive();

    /**
     * Read incoming event/command from the connection.
     *
     * @param timeout Time-out in seconds.
     *                (-1) if no timeout is specified.
     */
    void readIncomingCommand( hoxCommand& inCommand,
                              const int   timeout = -1 );

protected:
    virtual void eventLoop() {}

    void setMyTable( const std::string& tid ) { m_sTableId = tid; }
    bool isMyTable( const std::string& tid ) const { return m_sTableId == tid; }

private:
    void _SendCommand( hoxCommand& command );

protected:
    const std::string   m_id;
    const std::string   m_password;
    st_netfd_t          m_nfd;     // socket's descriptor.

    std::string         m_sTableId; // THE table this Player is playing.
};

#endif /* __INCLUDED_PLAYER_H__ */

