//
// C++ Implementation: hoxLog
//
// Description: The Log module to log message to disk.
//
// Author: Huy Phan,,,, (C) 2009
//
// Created:         04/12/2009
//

/*
 * Portions created by SGI are Copyright (C) 2000 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Silicon Graphics, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <list>
#include <st.h>

#include "hoxLog.h"
#include "main.h"
#include "hoxCommon.h"
//#include "hoxDbClient.h"

/* Defined in main.cpp */
extern hoxGlobalConfig g_config;
extern int g_errfd;

/*
 * Simple error reporting functions.
 * Suggested in W. Richard Stevens' "Advanced Programming in UNIX
 * Environment".
 */

#define MAXLINE 4096  /* max line length */

static hoxStringList s_logMessages;

static void err_doit( int, int, const char *, va_list );


/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void err_sys_report( int fd, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    err_doit( fd, 1, fmt, ap );
    va_end( ap );
}


/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys_quit( int fd, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    err_doit( fd, 1, fmt, ap );
    va_end( ap );
    exit( 1 );
}


/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void err_sys_dump( int fd, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    err_doit( fd, 1, fmt, ap );
    va_end( ap );
    abort();  /* dump core and terminate */
    exit( 1 );  /* shouldn't get here */
}


/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_report( int fd, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    err_doit( fd, 0, fmt, ap );
    va_end( ap );
}


/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit( int fd, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    err_doit( fd, 0, fmt, ap );
    va_end( ap );
    exit( 1 );
}


/*
 * Return a pointer to a string containing current time.
 */
char *err_tstamp( void )
{
    static const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                                  };
    static char str[32];
    static time_t lastt = 0;
    struct tm* tmp = NULL;
    time_t currt = st_time();

    if ( currt == lastt )
        return str;

    tmp = localtime( &currt );
    sprintf( str, "[%02d/%s/%d:%02d:%02d:%02d] ", tmp->tm_mday,
             months[tmp->tm_mon], 1900 + tmp->tm_year, tmp->tm_hour,
             tmp->tm_min, tmp->tm_sec );
    lastt = currt;

    return str;
}


/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit( int fd, int errnoflag, const char *fmt, va_list ap )
{
    int errno_save;
    char buf[MAXLINE];

    errno_save = errno;         /* value caller might want printed   */
    strcpy( buf, err_tstamp() );  /* prepend a message with time stamp */
    vsprintf( buf + strlen( buf ), fmt, ap );
    if ( errnoflag )
        sprintf( buf + strlen( buf ), ": %s\n", strerror( errno_save ) );
    else
        strcat( buf, "\n" );
    write( fd, buf, strlen( buf ) );
    errno = errno_save;
}

void
hoxLog(enum hoxLogLevel level, const char *fmt, ...)
{
    static const char* levels[] = { "FATAL", "SYS_FATAL",
                                    "ERROR", "SYS_ERROR",
                                    "WARN",  "SYS_WARN",
                                    "INFO",
                                    "DEBUG"
                                  };

    if ( level > g_config.minLogLevel )
        return;

    int errno_save;
    char buf[MAXLINE];
    const size_t nMax = sizeof(buf);
    std::string sOut;   // containing the entire line output.

    int errnoflag = (   level == LOG_SYS_FATAL
                     || level == LOG_SYS_ERROR
                     || level == LOG_SYS_WARN )
                    ? 1 : 0;

    va_list ap;
    va_start( ap, fmt );

    errno_save = errno;         /* value caller might want printed   */
    sOut.append(err_tstamp()).append(levels[level]).append(": ");

    int printed = vsnprintf( buf, nMax, fmt, ap );
    va_end( ap );
    int used = std::min(printed, (int)nMax);
    sOut.append(buf, used);

    if ( errnoflag )
    {
        sOut.append(": ").append( ::strerror( errno_save ) );
    }
    sOut.append("\n");

    // ----
#if 0
    s_logMessages.push_back( sOut );
#else
    int fd = g_errfd;     // TODO: Defined in main.cpp
    write( fd, sOut.data(), sOut.size() );
#endif
    errno = errno_save;
}

void
hoxFlushPendingLogMsgs()
{
    if ( s_logMessages.empty() ) return;

    std::string sOut;
    for ( hoxStringList::const_iterator it = s_logMessages.begin();
                                        it != s_logMessages.end(); ++it )
    {
        sOut.append( *it );
    }
    s_logMessages.clear();

    //(void) hoxDbClient::log_msg( sOut );
}

/******************* END OF FILE *********************************************/
