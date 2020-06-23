#!/bin/bash

#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

CMAKE_GENERATOR="Unix Makefiles";
BUILD_TYPE="Release";
#BUILD_TYPE="Debug";
CMAKE_SRC_PATH="../..";

export CXX="gcc";

if [ ! -d "./_build" ]; then
	mkdir "./_build";
fi;

pushd "./_build";

cmake -G "$CMAKE_GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" "$CMAKE_SRC_PATH";

make clean;
make all test;

cpack -G "TGZ";
cpack -G "TGZ" --config CPackSourceConfig.cmake;

popd;

exit;
