#!/bin/bash
# ------------------------------------------------------------
#
# The startup script for 'hoxtest'.
#
# See: 'server.cfg' for additional runtime parameters.
#
# ------------------------------------------------------------

# ============================
# The global shared variables
# ============================

PROG=`basename $0`
APP=hoxtest

# ----------------------------------------------------
# FUNCTION:
#  Print the usage of this script
# ----------------------------------------------------
print_usage()
{
    echo "Usage: ./$PROG [OPTION]"
    echo "Script to help control $APP"
    echo
    echo "The options are:"
    echo " --help   Display this help and exit"
    echo " --kill   Kill the currently running app"
    echo " --reload Reload the configuration and clear the file-Cache"
}

# ----------------------------------------------------
# FUNCTION:
#  Kill the currently running app.
# ----------------------------------------------------
kill_app()
{
    if [ -f "logs/pid" ]; then
        local exist_pid=`cat logs/pid`
        echo "Terminating pid $exist_pid..."
        kill $exist_pid
    fi
}

# ----------------------------------------------------
# FUNCTION:
#  Reload configuration and clear the file-Cache.
# ----------------------------------------------------
reload_app()
{
    if [ -f "logs/pid" ]; then
        local exist_pid=`cat logs/pid`
        echo "Reloading pid $exist_pid..."
        kill -SIGHUP $exist_pid
    fi
}

# ===============================================
# MAIN
# The main program starts here.
# ===============================================

# Arguments checking.
if [ "$1" == "--help" ]; then
    print_usage
    exit 0;  # success
elif [ "$1" == "--kill" ]; then
    kill_app
    exit 0;  # success
elif [ "$1" == "--reload" ]; then
    reload_app
    exit 0;  # success
elif [ -n "$1" ]; then
    echo "Unknown option: '$1'"
    echo "Try ./$PROG --help' for more information."
    exit 1;  # failure
fi

# Terminate the existing app.
kill_app

# Run the app.
echo "Starting a new instance of [$APP]..."
./$APP -l logs/ -p 1

############# END OF FILE #####################################################

