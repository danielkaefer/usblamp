#!/bin/bash
# Sets lamp color depending on the load of the given server.
# The server needs to have munin-node running.
# Author: Christian Weiske <cweiske@cweiske.de>
# Dependencies: bash (for "source" command), bc (math), netcat, usblamp
usblamp=`which usblamp`
host=FIXME
port=4949
if [ -x 'serverload-config.sh' ]; then
    source 'serverload-config.sh'
fi

load=`echo "fetch load" |nc -i 1 $host $port|grep load.value|sed 's/load.value //'`
#load=0.6

if [ `echo "$load > 1.2"|bc` -eq 1 ]; then
    #load is high
    $usblamp red
elif [ `echo "$load > 0.5"|bc` -eq 1 ]; then
    #load is medium orange
    $usblamp '#FF1100'
else
    #load is fine
    $usblamp off
fi
