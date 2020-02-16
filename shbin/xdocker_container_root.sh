#!/bin/bash

if [ $# -ne 1 ] ; then
	echo "USAGE : . xdocker_container_root.sh (container)"
	exit 1
fi

CONTAINER=$1

export XDOCKER_CONTAINER_ROOT=`xdocker -s container_root -c $CONTAINER`

