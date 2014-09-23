//
// C++ Implementation: Player
//
// Description:  The generic Player.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created: 04/18/2009
//

#include "Player.h"
#include "hoxCommand.h"
#include "hoxSocketAPI.h"
#include "hoxLog.h"
#include "main.h"

#include <stdexcept>   // std::runtime_error

//-----------------------------------------------------------------------------
//
//                                  Player
//
//-----------------------------------------------------------------------------

Player::Player( const std::string& id,
                    const std::string& password )
        : m_id( id )
        , m_password( password )
        , m_nfd( NULL )
{
}

Player::~Player()
{
    this->disconnect();
}

void
Player::run()
{
    this->connect( g_config.serverIP, g_config.serverPort );
    this->login();

    this->eventLoop();

    this->logout();
    this->disconnect();
}

void
Player::connect( const std::string&       sHost,
                 const unsigned short int nPort )
{
    if ( hoxRC_OK != hoxSocketAPI::tcp_connect( sHost, nPort,
                                                m_nfd ) )
    {
        throw std::runtime_error("Failed to connect to server: " + sHost );
    }
}

void
Player::disconnect()
{
    if ( m_nfd != NULL )
    {
        st_netfd_close( m_nfd );
        m_nfd = NULL;
    }
}

void
Player::login()
{
    hoxCommand outCommand("LOGIN");
    outCommand["password"] = m_password;
    outCommand["version"] = CLIENT_VERSION;
    _SendCommand( outCommand );

    // The first incoming command from the server must confirm
    // that my login is OK.
    hoxCommand  inCommand;
    this->readIncomingCommand( inCommand );
    
    std::string playerId;
    int         nScore = 0;
    hoxCommand::Parse_InCommand_LOGIN( inCommand["content"],
                                       playerId, nScore );
    hoxLog(LOG_INFO, "%s: (%s) Received %s (%d).", __FUNCTION__, m_id.c_str(),
            playerId.c_str(), nScore);
    if ( playerId != m_id )
    {
        throw std::runtime_error("Failed to login to server for: " + m_id );
    }
}

void
Player::logout()
{
    hoxCommand outCommand("LOGOUT");
    _SendCommand( outCommand );

    // No need to confirm my logout (by reading incoming confirmation from the
    // the server) since the server should auto-disconnect me anyway.
}

void
Player::openNewTable( hoxTableInfo&      tableInfo,
                      const std::string& sRole /* = "Red" */ )
{
    hoxCommand outCommand("NEW");
    outCommand["itimes"] = "1500/300/20";
    outCommand["color"]  = sRole;
    _SendCommand( outCommand );

    /* Wait until reading the "right" response. */
    for (;;)
    {
        hoxCommand  inCommand;  // Incoming command from the server.
        this->readIncomingCommand( inCommand );
        if ( inCommand.m_type == "I_TABLE" )
        {
            hoxTableInfo::String_To_Table( inCommand["content"],
                                           tableInfo );
            if (    tableInfo.redId   == m_id
                 || tableInfo.blackId == m_id ) // Is this my Table?
            {
                break;
            }
        }
    }

    m_sTableId = tableInfo.id;  // Remember that this is my table.
}

void
Player::sendMove( const std::string& sMove )
{
    hoxCommand outCommand("MOVE");
    outCommand["tid"]     = m_sTableId;
    outCommand["move"]    = sMove;
    _SendCommand( outCommand );
}

void
Player::sendDraw( const std::string& tid )
{
    hoxCommand outCommand("DRAW");
    outCommand["tid"] = tid;
    _SendCommand( outCommand );
}

void
Player::joinTable( const std::string& sRole )
{
    hoxCommand outCommand("JOIN");
    outCommand["tid"] = m_sTableId;
    outCommand["color"] = sRole;
    _SendCommand( outCommand );
}

void
Player::leaveCurrentTable()
{
    if ( ! m_sTableId.empty() )
    {
        hoxCommand outCommand("LEAVE");
        outCommand["tid"] = m_sTableId;
        _SendCommand( outCommand );
    }
}

void
Player::inviteOther( const std::string& oid )
{
    hoxCommand outCommand("INVITE");
    outCommand["oid"] = oid; // Invitee
    outCommand["tid"] = m_sTableId;
    _SendCommand( outCommand );
}

void
Player::sendKeepAlive()
{
    hoxCommand outCommand("PING");
    _SendCommand( outCommand );

    hoxCommand  inCommand;  // Incoming command from the server.
    this->readIncomingCommand( inCommand );
}

void
Player::readIncomingCommand( hoxCommand& inCommand,
                             const int   timeout /* = -1 */ )
{
    inCommand.Clear();

    std::string sResponse;
    const int nRead = hoxSocketAPI::read_until_all( m_nfd, "\n\n",
                                                    sResponse, timeout );
    if ( nRead == HOX_ERR_SOCKET_TIMEOUT )
    {
        throw TimeoutException("Socket read timeout");
    }
    else if ( nRead <= 0 )
    {
        throw std::runtime_error("Failed to read response");
    }

    hoxCommand::String_To_Command( sResponse, inCommand );
    if ( inCommand["code"] != "0" )
    {
        hoxLog(LOG_INFO, "%s: Received error code [%s].", __FUNCTION__, sResponse.c_str()); 
        throw std::runtime_error("(String to Command) Received error code");
    }
}

void
Player::_SendCommand( hoxCommand& command )
{
    /* Make sure THIS player-ID is sent along. */
    command["pid"] = m_id;

    const std::string sCommand = command.ToString() + "\n";
    if ( hoxRC_OK != hoxSocketAPI::write_string( m_nfd, sCommand ) )
    {
        throw std::runtime_error("Failed to send command");
    }
}

/************************* END OF FILE ***************************************/
