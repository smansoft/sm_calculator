#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

project (sstring_lib)

cmake_minimum_required (VERSION 3.1)

message (STATUS "PROJECT_NAME     	= ${PROJECT_NAME}")

enable_language (C)

if(WIN32)
	add_definitions (/D_LIB)
	add_definitions (/DSTDC_HEADERS)
endif(WIN32)

if(UNIX)
	add_definitions (-D_LIB)
	add_definitions (-DSTDC_HEADERS)
endif(UNIX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (${sm_common.rt_SOURCE_DIR})
include_directories (${sm_version.rt_SOURCE_DIR})
include_directories (${${PROJECT_NAME}_SOURCE_DIR})

file(GLOB HEADER_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.h)
file(GLOB SOURCE_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.c ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)

set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE C)

set (BUILD_SHARED_LIBS false)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

set(targets_export_name "${PROJECT_NAME}Targets")

message (STATUS "CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message (STATUS "CMAKE_INSTALL_LIBDIR     = ${CMAKE_INSTALL_LIBDIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES
	PUBLIC_HEADER  	"${HEADER_FILES}"
	OUTPUT_NAME 	"${PROJECT_NAME}")

