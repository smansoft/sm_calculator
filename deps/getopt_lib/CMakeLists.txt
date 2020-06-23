#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

project (getopt_lib)

cmake_minimum_required (VERSION 3.1)

message (STATUS "PROJECT_NAME     	= ${PROJECT_NAME}")

enable_language (C)

if(WIN32)
	add_definitions (/D_LIB)
	add_definitions (/DEXPORTS_GETOPT)
	add_definitions (/D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

if(UNIX)
	add_definitions (-D_LIB)
	add_definitions (-DEXPORTS_GETOPT)
endif(UNIX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (${sm_common.rt_SOURCE_DIR})
include_directories (${sm_version.rt_SOURCE_DIR})
include_directories (${${PROJECT_NAME}_SOURCE_DIR})

file(GLOB HEADER_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.h)
file(GLOB SOURCE_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.c ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)

set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE C)

set (BUILD_SHARED_LIBS true)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	PUBLIC_HEADER  	"${HEADER_FILES}"
	OUTPUT_NAME 	"${PROJECT_NAME}")

if(WIN32)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif(WIN32)

if(UNIX)
	install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(UNIX)
