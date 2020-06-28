@echo off

rem
rem    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
rem                       Sergey Manoylo <info@smansoft.com>
rem

set CMAKE_GENERATOR="NMake Makefiles"
rem set BUILD_TYPE="Debug"
set BUILD_TYPE="Release"
set CMAKE_SRC_PATH="..\\.."

if not exist "_build" (
	md "_build"
)

pushd "_build"

cmake -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %CMAKE_SRC_PATH%

nmake clean
nmake
nmake test

cpack -G "ZIP"
cpack -G "ZIP" --config CPackSourceConfig.cmake

popd

exit /B 0
