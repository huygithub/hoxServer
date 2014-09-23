//
// C++ Interface: hoxLog
//
// Description: The Log module to log message to disk.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

#ifndef __INCLUDED_HOX_LOG_H__
#define __INCLUDED_HOX_LOG_H__


/* Error reporting functions defined in the error.cpp file */
void err_sys_report( int fd, const char *fmt, ... );
void err_sys_quit( int fd, const char *fmt, ... );
void err_sys_dump( int fd, const char *fmt, ... );
void err_report( int fd, const char *fmt, ... );
void err_quit( int fd, const char *fmt, ... );


enum hoxLogLevel
{
    /* NOTE: Do not change the constants here as they are referred to
     *       by outside systems.
     */

    LOG_MIN        = 0,      // BEGIN ---

    LOG_FATAL      = LOG_MIN,
    LOG_SYS_FATAL  = 1,  // with error > 0
    LOG_ERROR      = 2,
    LOG_SYS_ERROR  = 3,  // with error > 0
    LOG_WARN       = 4,
    LOG_SYS_WARN   = 5,   // with error > 0
    LOG_INFO       = 6,
    LOG_DEBUG      = 7,

    LOG_MAX        = LOG_DEBUG // END ---
};


void
hoxLog(enum hoxLogLevel level, const char *fmt, ...);

void
hoxFlushPendingLogMsgs();


#endif /* __INCLUDED_HOX_LOG_H__ */
