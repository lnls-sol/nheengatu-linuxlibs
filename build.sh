#!/bin/sh
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/configure.sh
cmake --build $SCRIPTPATH/build -- $@
