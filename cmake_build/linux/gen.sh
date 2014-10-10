#!/bin/bash
CMAKE_BUILD_TYPE=debug
if [ $# -gt 0 ] ; then
	CMAKE_BUILD_TYPE=$1
fi

cmake \
	-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
	`dirname $0`
