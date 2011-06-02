#!/bin/bash
# Turns on lamp, if a new revision is available
#
# Author: Daniel Kaefer
# Dependencies:
#  bash subversion, usblamp
#
# Configuration:
# - Repository path as first parameter

path="."
# cli parameter
if [ "x" != "x$1" ]; then
	path="$1"
fi
new=`svn info -r HEAD $path |sed -e '/Rev:/!d' -e '/Rev:/s/.*: //'`
old=`cat ~/.svn_last_rev`
if [ -z $old ]; then
	old=0
fi

echo $new > ~/.svn_last_rev

if [[ $new -gt $old ]]; then
    # new revision available
    usblamp green
fi
