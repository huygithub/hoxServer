//
// C++ Interface: hoxLog
//
// Description: 
//
//
// Author: Huy Phan  <hphan@hphan-hp>, (C) 2008-2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INCLUDED_HOX_LOG_H_
#define __INCLUDED_HOX_LOG_H_


/* Error reporting functions defined in the error.cpp file */
void err_sys_report( int fd, const char *fmt, ... );
void err_sys_quit( int fd, const char *fmt, ... );
void err_sys_dump( int fd, const char *fmt, ... );
void err_report( int fd, const char *fmt, ... );
void err_quit( int fd, const char *fmt, ... );


enum hoxLogLevel
{
    LOG_FATAL,
    LOG_SYS_FATAL,  // with error > 0
    LOG_ERROR,
    LOG_SYS_ERROR,  // with error > 0
    LOG_WARN,
    LOG_SYS_WARN,   // with error > 0
    LOG_INFO,
    LOG_DEBUG
};

/* Defined in main.cpp */
extern int s_errfd;

void
hoxLog(enum hoxLogLevel level, const char *fmt, ...);


#endif /* __INCLUDED_HOX_LOG_H_ */
