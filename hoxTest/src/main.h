//
// C++ Interface: "main" interface
//
// Description:  Containing global variables.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/13/2009
//

#ifndef __INCLUDED_MAIN_H__
#define __INCLUDED_MAIN_H__

#include "hoxLog.h"
#include "hoxCommon.h"
#include <string>

/**
 * The global configuration for the App.
 */
class hoxGlobalConfig
{
public:
    hoxGlobalConfig() : minLogLevel( LOG_DEBUG )
                      , timeBetweenMoves( 10 )
                      , numTestPlayers( 2 )
        { /* empty */ }

    hoxLogLevel    minLogLevel;        /* Minimal log level            */

    std::string    serverIP;           /* Server's IP or Name          */
    int            serverPort;         /* Server's Port                */

    int            timeBetweenMoves;   /* ... in seconds               */

    int            numTestPlayers;     /* The number of Test players   */
    hoxStringList  groupTestIds;       /* Predefined Test IDs          */
    std::string    groupPassword;      /* Predefined password          */ 
};

/**
 * The AI Player's configuration.
 */
class hoxAIConfig
{
public:
    hoxAIConfig() : depth( 0 )
        { /* empty */ }

    hoxStringVector pids;   // List of PIDs.
    std::string     password;
    std::string     engine;
    std::string     role;
    int             depth;
};

/**
 *   Globally shared variables.
 */

/* Defined in main.cpp */
extern hoxGlobalConfig g_config;

#endif /* __INCLUDED_MAIN_H__ */
