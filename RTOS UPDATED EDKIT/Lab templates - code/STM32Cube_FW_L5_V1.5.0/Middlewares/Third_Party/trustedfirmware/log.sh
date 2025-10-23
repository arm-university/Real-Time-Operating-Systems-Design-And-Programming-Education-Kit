#!/bin/bash
#-------------------------------------------------------------------------------
# Copyright (c) 2017, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if [ ! -e "c:/" ]
then
function cygpath {
	readlink -f "$2"
}
fi

file=`readlink -e ${@:$#:1}`
cmd=`which ${@:1:1}` || cmd=${@:1:1}
cmd=`cygpath -m "$cmd"`
cat <<EOM
{
  "directory": "`cygpath -m "$PWD"`",
  "command": "\"$cmd\" ${@:2:(( $#-1 ))}",
  "file": "`cygpath -m "$file"`"
},
EOM

