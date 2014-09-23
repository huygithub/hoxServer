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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <execinfo.h>
#include <cstring>

#include <st.h>
#include <libconfig.h++>
#include "main.h"
#include "hoxLog.h"
#include "hoxCommon.h"

/******************************************************************
 * Server configuration parameters
 */

#define  DEFAULT_SERVER_IP    "127.0.0.1"
#define  DEFAULT_SERVER_PORT  8001

/* Log files */
#define PID_FILE           "pid"
#define ERRORS_FILE        "errors.log"
#define SERVER_CFG_FILE    "./server.cfg"

/* Max number of listening sockets ("hardware virtual servers") */
#define MAX_BIND_ADDRS 16

/* Access log buffer flushing interval (in seconds) */
#define ACCLOG_FLUSH_INTERVAL 2 /* 30 */

/******************************************************************
 * Global data
 */

static int vp_count = 0;        /* Number of server processes (VPs)     */
static pid_t *vp_pids;          /* Array of VP pids                     */

static int my_index = -1;       /* Current process index */
static pid_t my_pid = -1;       /* Current process pid   */
static int next_thread_id = 0;

static st_netfd_t sig_pipe[2];  /* Signal pipe           */

/*
 * Configuration flags/parameters
 */
static int interactive_mode = 0;
static const char* s_logdir = NULL;
static char *username   = NULL;
/*static*/ int g_errfd        = STDERR_FILENO;

hoxGlobalConfig g_config;    /* The global configuration */

/******************************************************************
 * Forward declarations
 */

static void usage( const char *progname );
static void parse_arguments( int argc, char *argv[] );
static void start_daemon( void );
static void change_user( void );
static void open_log_files( void );
static void start_processes( void );
static void wdog_sighandler( int signo );
static void child_sighandler( int signo );
static void install_sighandlers( void );
static void create_AI_players_threads( void );
static void start_threads( void );
static void *process_signals( void *arg );
static void *flush_acclog_buffer( void *arg );
static void *handle_connections( void *arg );
static void dump_server_info( void );

static void Signal( int sig, void ( *handler )( int ) );
//static int cpu_count( void );
static const std::string get_actual_path( const char* name );

extern void handle_session( const int  thread_id );
extern void* AI_Player_thread( void* arg );

extern void load_configs( void );
extern void logbuf_open( void );
extern void logbuf_flush( void );
extern void logbuf_close( void );

/*
 * General server example: accept a client connection and do something.
 * This program just outputs a short HTML page, but can be easily adapted
 * to do other things.
 *
 * This server creates a constant number of processes ("virtual processors"
 * or VPs) and replaces them when they die. Each virtual processor manages
 * its own independent set of state threads (STs), the number of which varies
 * with load against the server. Each state thread listens to exactly one
 * listening socket. The initial process becomes the watchdog, waiting for
 * children (VPs) to die or for a signal requesting termination or restart.
 * Upon receiving a restart signal (SIGHUP), all VPs close and then reopen
 * log files and reload configuration. All currently active connections remain
 * active. It is assumed that new configuration affects only request
 * processing and not the general server parameters such as number of VPs,
 * thread limits, bind addresses, etc. Those are specified as command line
 * arguments, so the server has to be stopped and then started again in order
 * to change them.
 *
 * Each state thread loops processing connections from a single listening
 * socket. Only one ST runs on a VP at a time, and VPs do not share memory,
 * so no mutual exclusion locking is necessary on any data, and the entire
 * server is free to use all the static variables and non-reentrant library
 * functions it wants, greatly simplifying programming and debugging and
 * increasing performance (for example, it is safe to ++ and -- all global
 * counters or call inet_ntoa(3) without any mutexes). The current thread on
 * each VP maintains equilibrium on that VP, starting a new thread or
 * terminating itself if the number of spare threads exceeds the lower or
 * upper limit.
 *
 * All I/O operations on sockets must use the State Thread library's I/O
 * functions because only those functions prevent blocking of the entire VP
 * process and perform state thread scheduling.
 */
int
main( int argc, char *argv[] )
{
    /* Parse command-line options */
    parse_arguments( argc, argv );

    /* Allocate array of server pids */
    if (( vp_pids = ( pid_t* ) calloc( vp_count, sizeof( pid_t ) ) ) == NULL )
        err_sys_quit( g_errfd, "ERROR: calloc failed" );

    /* Start the daemon */
    if ( !interactive_mode )
        start_daemon();

    /* Initialize the ST library */
    if ( st_init() < 0 )
        err_sys_quit( g_errfd, "ERROR: initialization failed: st_init" );

    /* Change the user */
    if ( username )
        change_user();

    /* Open log files */
    open_log_files();

    /* Start server processes (VPs) */
    start_processes();

    hoxUtil::generateRandomSeed(); /* Generate the random seed */
    st_timecache_set( 1 );         /* Turn time caching on     */

    /* Install signal handlers */
    install_sighandlers();

    /* Load configuration from config files */
    load_configs();

    /* Start all threads */
    start_threads();

    /* Become a signal processing thread */
    process_signals( NULL );

    /* NOTREACHED */
    return 1;
}


/******************************************************************/

static void usage( const char *progname )
{
    fprintf( stderr, "Usage: %s -l <log_directory> [<options>]\n\n"
        "Possible options:\n\n"
        "\t-p <num_processes>    Create specified number of processes.\n"
        "\t-u <user>             Change server's user id to specified value.\n"
        "\t-i                    Run in interactive mode.\n"
        "\t-h                    Print this message.\n",
        progname );
    exit( 1 );
}


/******************************************************************/

static void parse_arguments( int argc, char *argv[] )
{
    extern char *optarg;
    int   opt = 0;

    while (( opt = getopt( argc, argv, "p:l:u:ih" ) ) != EOF )
    {
        switch ( opt )
        {
            case 'p':
                vp_count = atoi( optarg );
                if ( vp_count < 1 )
                    err_quit( g_errfd, "ERROR: invalid number of processes: %s", optarg );
                break;
            case 'l':
                s_logdir = optarg;
                break;
            case 'u':
                username = optarg;
                break;
            case 'i':
                interactive_mode = 1;
                break;
            case 'h':
            case '?':
                usage( argv[0] );
        }
    }

    if ( s_logdir == NULL && !interactive_mode )
    {
        err_report( g_errfd, "ERROR: logging directory is required\n" );
        usage( argv[0] );
    }

    if ( getuid() == 0 && username == NULL )
        err_report( g_errfd, "WARNING: running as super-user!" );

    if ( vp_count == 0 /* && ( vp_count = cpu_count() ) < 1 */ )
        vp_count = 1;
}


/******************************************************************/

static void start_daemon( void )
{
    pid_t pid;

    /* Start forking */
    if (( pid = fork() ) < 0 )
        err_sys_quit( g_errfd, "ERROR: fork" );
    if ( pid > 0 )
        exit( 0 );                /* parent */

    /* First child process */
    setsid();                   /* become session leader */

    if (( pid = fork() ) < 0 )
        err_sys_quit( g_errfd, "ERROR: fork" );
    if ( pid > 0 )              /* first child */
        exit( 0 );

    umask( 022 );

    //if ( chdir( s_logdir ) < 0 )
    //    err_sys_quit( g_errfd, "ERROR: can't change directory to %s: chdir", s_logdir );
}


/******************************************************************/

static void change_user( void )
{
    struct passwd *pw;

    if (( pw = getpwnam( username ) ) == NULL )
        err_quit( g_errfd, "ERROR: can't find user '%s': getpwnam failed", username );

    if ( setgid( pw->pw_gid ) < 0 )
        err_sys_quit( g_errfd, "ERROR: can't change group id: setgid" );
    if ( setuid( pw->pw_uid ) < 0 )
        err_sys_quit( g_errfd, "ERROR: can't change user id: setuid" );

    err_report( g_errfd, "INFO: changed process user id to '%s'", username );
}


/******************************************************************/

static void open_log_files( void )
{
    int fd;
    char str[32];

    if ( interactive_mode )
        return;

    /* Open access log */
    logbuf_open();

    /* Open and write pid to pid file */
    if (( fd = open( get_actual_path(PID_FILE).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644 ) ) < 0 )
        err_sys_quit( g_errfd, "ERROR: can't open pid file: open" );
    sprintf( str, "%d\n", ( int )getpid() );
    if ( write( fd, str, strlen( str ) ) != (int) strlen( str ) )
        err_sys_quit( g_errfd, "ERROR: can't write to pid file: write" );
    close( fd );

    /* Open error log file */
    if (( fd = open( get_actual_path(ERRORS_FILE).c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644 ) ) < 0 )
        err_sys_quit( g_errfd, "ERROR: can't open error log file: open" );
    g_errfd = fd;

    hoxLog(LOG_INFO, "Starting the server..." );
}


/******************************************************************/

static void start_processes( void )
{
    int i, status;
    pid_t pid;
    sigset_t mask, omask;

    if ( interactive_mode )
    {
        my_index = 0;
        my_pid = getpid();
        return;
    }

    for ( i = 0; i < vp_count; i++ )
    {
        if (( pid = fork() ) < 0 )
        {
            err_sys_report( g_errfd, "ERROR: can't create process: fork" );
            if ( i == 0 )
                exit( 1 );
            err_report( g_errfd, "WARN: started only %d processes out of %d", i, vp_count );
            vp_count = i;
            break;
        }
        if ( pid == 0 )
        {
            my_index = i;
            my_pid = getpid();
            /* Child returns to continue in main() */
            return;
        }
        vp_pids[i] = pid;
    }

    /*
     * Parent process becomes a "watchdog" and never returns to main().
     */

    /* Install signal handlers */
    Signal( SIGTERM, wdog_sighandler );  /* terminate */
    Signal( SIGHUP,  wdog_sighandler );  /* restart   */
    Signal( SIGUSR1, wdog_sighandler );  /* dump info */

    /* Now go to sleep waiting for a child termination or a signal */
    for ( ; ; )
    {
        if (( pid = wait( &status ) ) < 0 )
        {
            if ( errno == EINTR )
                continue;
            err_sys_quit( g_errfd, "ERROR: watchdog: wait" );
        }
        /* Find index of the exited child */
        for ( i = 0; i < vp_count; i++ )
        {
            if ( vp_pids[i] == pid )
                break;
        }

        /* Block signals while printing and forking */
        sigemptyset( &mask );
        sigaddset( &mask, SIGTERM );
        sigaddset( &mask, SIGHUP );
        sigaddset( &mask, SIGUSR1 );
        sigprocmask( SIG_BLOCK, &mask, &omask );

        if ( WIFEXITED( status ) )
            err_report( g_errfd, "WARN: watchdog: process %d (pid %d) exited"
                        " with status %d", i, pid, WEXITSTATUS( status ) );
        else if ( WIFSIGNALED( status ) )
            err_report( g_errfd, "WARN: watchdog: process %d (pid %d) terminated"
                        " by signal %d", i, pid, WTERMSIG( status ) );
        else if ( WIFSTOPPED( status ) )
            err_report( g_errfd, "WARN: watchdog: process %d (pid %d) stopped"
                        " by signal %d", i, pid, WSTOPSIG( status ) );
        else
            err_report( g_errfd, "WARN: watchdog: process %d (pid %d) terminated:"
                        " unknown termination reason", i, pid );

        /* Fork another VP */
        if (( pid = fork() ) < 0 )
        {
            err_sys_report( g_errfd, "ERROR: watchdog: can't create process: fork" );
        }
        else if ( pid == 0 )
        {
            my_index = i;
            my_pid = getpid();
            /* Child returns to continue in main() */
            return;
        }
        vp_pids[i] = pid;

        /* Restore the signal mask */
        sigprocmask( SIG_SETMASK, &omask, NULL );
    }
}


/******************************************************************/

static void wdog_sighandler( int signo )
{
    int i, err;

    /* Save errno */
    err = errno;
    /* Forward the signal to all children */
    for ( i = 0; i < vp_count; i++ )
    {
        if ( vp_pids[i] > 0 )
            kill( vp_pids[i], signo );
    }
    /*
     * It is safe to do pretty much everything here because process is
     * sleeping in wait() which is async-safe.
     */
    switch ( signo )
    {
        case SIGHUP:
            hoxLog(LOG_INFO, "watchdog: caught SIGHUP" );
            /* Reopen log files - needed for log rotation */
            logbuf_close();
            logbuf_open();
            close( g_errfd );
            if (( g_errfd = open( get_actual_path(ERRORS_FILE).c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644 ) ) < 0 )
                err_sys_quit( STDERR_FILENO, "ERROR: watchdog: open" );
            break;
        case SIGTERM:
        {
            /* Non-graceful termination */
            hoxLog(LOG_INFO, "watchdog: caught SIGTERM, terminating" );
            unlink( get_actual_path(PID_FILE).c_str() );
            exit( 0 );
        }
        case SIGUSR1:
            hoxLog(LOG_INFO, "watchdog: caught SIGUSR1" );
            break;
        default:
            hoxLog(LOG_INFO, "watchdog: caught signal %d", signo );
    }
    /* Restore errno */
    errno = err;
}


/******************************************************************/

static void install_sighandlers( void )
{
    sigset_t mask;
    int p[2];

    /* Create signal pipe */
    if ( pipe( p ) < 0 )
        err_sys_quit( g_errfd, "ERROR: process %d (pid %d): can't create"
                      " signal pipe: pipe", my_index, my_pid );
    if (( sig_pipe[0] = st_netfd_open( p[0] ) ) == NULL ||
            ( sig_pipe[1] = st_netfd_open( p[1] ) ) == NULL )
        err_sys_quit( g_errfd, "ERROR: process %d (pid %d): can't create"
                      " signal pipe: st_netfd_open", my_index, my_pid );

    /* Install signal handlers */
    Signal( SIGTERM, child_sighandler );  /* terminate */
    Signal( SIGHUP,  child_sighandler );  /* restart   */
    Signal( SIGUSR1, child_sighandler );  /* dump info */
    Signal( SIGSEGV, child_sighandler );
    Signal( SIGABRT, child_sighandler );

    /* Unblock signals */
    sigemptyset( &mask );
    sigaddset( &mask, SIGTERM );
    sigaddset( &mask, SIGHUP );
    sigaddset( &mask, SIGUSR1 );
    sigaddset( &mask, SIGSEGV );
    sigaddset( &mask, SIGABRT );
    sigprocmask( SIG_UNBLOCK, &mask, NULL );
}


/******************************************************************/

static void child_sighandler( int signo )
{
    int err, fd;

    //////////////
    switch (signo )
    {
        case SIGSEGV: /* fall through */
        case SIGABRT:
            {
                const int SIZE = 20;
                void* buffer[SIZE];
                const int nptrs = backtrace(buffer, SIZE);
                //backtrace_symbols_fd(buffer, nptrs, g_errfd);
                char** strings = backtrace_symbols(buffer, nptrs);
                if ( strings != NULL )
                {
                    for (int i = 0; i < nptrs; ++i) {
                        err_report(g_errfd, "%s", strings[i]);
                    }
                    free(strings);
                }
            }
            break;
        default:
            break;
    }
    //////////////

    err = errno;
    fd = st_netfd_fileno( sig_pipe[1] );

    /* write() is async-safe */
    if ( write( fd, &signo, sizeof( int ) ) != sizeof( int ) )
        err_sys_quit( g_errfd, "ERROR: process %d (pid %d): child's signal"
                      " handler: write", my_index, my_pid );
    errno = err;
}


/******************************************************************
 * The "main" function of the signal processing thread.
 */

/* ARGSUSED */
static void *process_signals( void *arg )
{
    int signo;

    for ( ; ; )
    {
        /* Read the next signal from the signal pipe */
        if ( st_read( sig_pipe[0], &signo, sizeof( int ),
                      ST_UTIME_NO_TIMEOUT ) != sizeof( int ) )
            err_sys_quit( g_errfd, "ERROR: process %d (pid %d): signal processor:"
                          " st_read", my_index, my_pid );

        switch ( signo )
        {
            case SIGHUP:
                err_report( g_errfd, "INFO: process %d (pid %d): caught SIGHUP,"
                            " reloading configuration", my_index, my_pid );
                if ( interactive_mode )
                {
                    load_configs();
                    break;
                }
                /* Reopen log files - needed for log rotation */
                logbuf_flush();
                logbuf_close();
                logbuf_open();
                close( g_errfd );
                if (( g_errfd = open( get_actual_path(ERRORS_FILE).c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644 ) ) < 0 )
                    err_sys_quit( STDERR_FILENO, "ERROR: process %d (pid %d): signal"
                                  " processor: open", my_index, my_pid );
                /* Reload configuration */
                load_configs();
                break;
            case SIGTERM:
                /*
                 * Terminate ungracefully since it is generally not known how long
                 * it will take to gracefully complete all client sessions.
                 */
                err_report( g_errfd, "INFO: process %d (pid %d): caught SIGTERM,"
                            " terminating", my_index, my_pid );
                logbuf_flush();
                //hoxDbClient::deinitialize();
                exit( 0 );
            case SIGUSR1:
                err_report( g_errfd, "INFO: process %d (pid %d): caught SIGUSR1",
                            my_index, my_pid );
                /* Print server info to stderr */
                dump_server_info();
                break;
            default:
                err_report( g_errfd, "INFO: process %d (pid %d): caught signal %d",
                            my_index, my_pid, signo );
        }
    }

    /* NOTREACHED */
    return NULL;
}


/******************************************************************
 * The "main" function of the access log flushing thread.
 */

/* ARGSUSED */
static void *flush_acclog_buffer( void *arg )
{
    for ( ; ; )
    {
        st_sleep( ACCLOG_FLUSH_INTERVAL );
        logbuf_flush();
    }

    /* NOTREACHED */
    return NULL;
}


/******************************************************************/

static void start_threads( void )
{
    /* Create access log flushing thread */
    if ( st_thread_create( flush_acclog_buffer, NULL, 0, 0 ) == NULL )
        err_sys_quit( g_errfd, "ERROR: process %d (pid %d): can't create log flushing thread",
                      my_index, my_pid );

    /* Create AI players. */
    create_AI_players_threads();

    /* Create Test players. */
    const int num_test_players = g_config.numTestPlayers;
    err_report( g_errfd, "INFO: process %d (pid %d): starting %d threads",
                my_index, my_pid, num_test_players );
    for ( int t = 0; t < num_test_players; ++t )
    {
        if ( st_thread_create( handle_connections, NULL, 0, 0 ) == NULL )
        {
            err_sys_report( g_errfd, "ERROR: process %d (pid %d): can't create thread [%d]",
                            my_index, my_pid, t );
        }
    }
}


/******************************************************************/

static void* handle_connections( void* arg_NOT_USED )
{
    const int my_thread_id = ++next_thread_id;
    handle_session( my_thread_id );

    return NULL;
}


/******************************************************************/

static void dump_server_info( void )
{
    char* buf = NULL;

    if (( buf = ( char* ) malloc( 512 ) ) == NULL )
    {
        err_sys_report( g_errfd, "ERROR: malloc failed" );
        return;
    }

    int len = sprintf( buf, "\n\nProcess #%d (pid %d):\n", my_index, ( int )my_pid );
    len += sprintf( buf + len, "\nProcess Info:\n"
                    "-------------------------\n"
                    "Test Player            %d\n",
                    g_config.numTestPlayers );

    write( STDERR_FILENO, buf, len );
    free( buf );
}


/******************************************************************
 * Stubs
 */

/*
 * Create the AI Player Threads.
 */
void create_AI_players_threads( void )
{
    using namespace libconfig;

    const char* szConfigFile = SERVER_CFG_FILE;
    err_report( g_errfd, "INFO: process %d (pid %d): load AI Players from [%s]...",
                my_index, my_pid, szConfigFile );
    Config cfg;

    try
    {
        cfg.readFile( szConfigFile ); // Load the configuration file.
        
        // Load the list of AI players.
        Setting& aiPlayers = cfg.lookup( "server.AIPlayers" );
        for ( int i = 0; i < aiPlayers.getLength(); ++i )
        {
            const std::string playerName = aiPlayers[i]; 
            err_report( g_errfd, "INFO: Loading AI player #%d) [%s]...", i+1, playerName.c_str() );
            // --- Load the AI Player's info.
            const std::string aiPrefixKey = "server." + playerName + ".";
            hoxAIConfig* pAIConfig = new hoxAIConfig;
            Setting& pids = cfg.lookup( aiPrefixKey + "pids" );
            for ( int p = 0; p < pids.getLength(); ++p )
            {
                pAIConfig->pids.push_back( pids[p] );
            }
            pAIConfig->password = (const char *) cfg.lookup( aiPrefixKey + "password" );
            pAIConfig->engine   = (const char *) cfg.lookup( aiPrefixKey + "aiEngine" );
            pAIConfig->role     = (const char *) cfg.lookup( aiPrefixKey + "aiRole" );
            pAIConfig->depth    =                cfg.lookup( aiPrefixKey + "aiDepth" );
            // Create AI Player thread.
            if ( st_thread_create( AI_Player_thread, pAIConfig, 0, 0 ) == NULL )
            {
                err_sys_quit( g_errfd, "ERROR: process %d (pid %d): can't create"
                      " AI_Player thread", my_index, my_pid );
                delete pAIConfig;
            }
        }

    }
    catch (...)
    {
        err_report( g_errfd, "ERROR: Failed to load configuration from [%s].", szConfigFile );
    }
}

/*
 * Configuration loading function stub.
 */
void load_configs( void )
{
    using namespace libconfig;

    const char* szConfigFile = SERVER_CFG_FILE;
    err_report( g_errfd, "INFO: process %d (pid %d): load configuration from [%s]...",
                my_index, my_pid, szConfigFile );
    Config cfg;
    int    val = 0;

    try
    {
        cfg.readFile( szConfigFile ); // Load the configuration file.

        if ( cfg.lookupValue( "server.logLevel", val ) )
        {
            err_report( g_errfd, "INFO: ... server.logLevel = [%d].", val );
            if ( val >= LOG_MIN && val <= LOG_MAX )
            {
                g_config.minLogLevel = (hoxLogLevel) val;
            }
        }

        if ( !cfg.lookupValue( "server.ip", g_config.serverIP ) ) {
            g_config.serverIP = DEFAULT_SERVER_IP;
        }
        if ( !cfg.lookupValue( "server.port", g_config.serverPort ) ) {
            g_config.serverPort = DEFAULT_SERVER_PORT;
        }
        err_report( g_errfd, "INFO: ... server = [%s:%d].", g_config.serverIP.c_str(), g_config.serverPort );

        if ( cfg.lookupValue( "server.timeBetweenMoves", val ) )
        {
            err_report( g_errfd, "INFO: ... server.timeBetweenMoves = [%d].", val );
            g_config.timeBetweenMoves = val;
        }
        if ( cfg.lookupValue( "server.numTestPlayers", val ) )
        {
            err_report( g_errfd, "INFO: ... server.numTestPlayers = [%d].", val );
            g_config.numTestPlayers = val;
        }
        // Load the list of predefined Test IDs.
        g_config.groupTestIds.clear();
        Setting& testIDs = cfg.lookup( "server.groupTestIds" );
        for ( int i = 0; i < testIDs.getLength(); ++i )
        {
            g_config.groupTestIds.push_back( testIDs[i] );
        }
        // Load the group's password.
        g_config.groupPassword = (const char *) cfg.lookup( "server.groupPassword" );
    }
    catch (...)
    {
        err_report( g_errfd, "ERROR: Failed to load configuration from [%s].", szConfigFile );
    }
}


/*
 * Buffered access logging methods.
 * Note that stdio functions (fopen(3), fprintf(3), fflush(3), etc.) cannot
 * be used if multiple VPs are created since these functions can flush buffer
 * at any point and thus write only partial log record to disk.
 * Also, it is completely safe for all threads of the same VP to write to
 * the same log buffer without any mutex protection (one buffer per VP, of
 * course).
 */
void logbuf_open( void )
{

}


void logbuf_flush( void )
{
    hoxFlushPendingLogMsgs();
}


void logbuf_close( void )
{

}


/******************************************************************
 * Small utility functions
 */

static void Signal( int sig, void ( *handler )( int ) )
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset( &sa.sa_mask );
    sa.sa_flags = 0;
    sigaction( sig, &sa, NULL );
}

/*
static int cpu_count( void )
{
    int n;

#if defined (_SC_NPROCESSORS_ONLN)
    n = ( int ) sysconf( _SC_NPROCESSORS_ONLN );
#elif defined (_SC_NPROC_ONLN)
    n = ( int ) sysconf( _SC_NPROC_ONLN );
#elif defined (HPUX)
#include <sys/mpctl.h>
    n = mpctl( MPC_GETNUMSPUS, 0, 0 );
#else
    n = -1;
    errno = ENOSYS;
#endif

    return n;
}
*/

static const std::string get_actual_path( const char* name )
{
    const std::string sActualPath = std::string(s_logdir) + name;
    return sActualPath;
}

/******************************************************************/
