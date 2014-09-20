#!/bin/bash
# ------------------------------------------------------------
#
# The startup script for DBAgent
#
# ------------------------------------------------------------

# ============================
# The global shared variables
# ============================

PROG=`basename $0`
APP=dbagent

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
elif [ -n "$1" ]; then
    echo "Unknown option: '$1'"
    echo "Try ./$PROG --help' for more information."
    exit 1;  # failure
fi

# Terminate the existing app.
kill_app

echo "Starting a new instance of [$APP]..."
#LD_LIBRARY_PATH=../lib ./dbagent  -l logs/ -b 192.168.206.141 -p 7001 &
#LD_LIBRARY_PATH=../lib ./dbagent  -l logs/ -p 7001 &
./$APP -l logs/ -p 7001 &

############# END OF FILE #####################################################

