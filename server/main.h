//
// C++ Interface: "main" interface
//
// Description: Containing global variables.
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Created: 04/14/2009
//

#ifndef __INCLUDED_MAIN_H__
#define __INCLUDED_MAIN_H__

#include "hoxLog.h"


class hoxGlobalConfig
{
public:
    hoxGlobalConfig() : minLogLevel( LOG_DEBUG )
        { /* empty */ }

    hoxLogLevel  minLogLevel;        /* Minimal log level      */
};

/* Defined in main.cpp */
extern hoxGlobalConfig g_config;

#endif /* __INCLUDED_MAIN_H__ */
