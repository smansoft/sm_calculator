@echo off

rem
rem    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
rem                       Sergey Manoylo <info@smansoft.com>
rem

set CMAKE_GENERATOR="Visual Studio 16 2019"
set BUILD_TYPE="Release"
set BUILD_PLATFORM="x64"
set CMAKE_SRC_PATH="..\\.."

if not exist "_build" (
	md "_build"
)

pushd "_build"

cmake -G %CMAKE_GENERATOR% -A %BUILD_PLATFORM% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% %CMAKE_SRC_PATH%

popd

exit /B 0
