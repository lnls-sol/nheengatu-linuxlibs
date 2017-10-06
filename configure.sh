#!/bin/sh
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
mkdir -p $SCRIPTPATH/build
cd $SCRIPTPATH/build
cmake .. $@
