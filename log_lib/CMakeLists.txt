#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

project (log_lib)

cmake_minimum_required (VERSION 3.1)

message (STATUS "PROJECT_NAME     	= ${PROJECT_NAME}")

enable_language (C)

if(WIN32)
	add_definitions (/D_LIB)
endif(WIN32)

if(UNIX)
	add_definitions (-D_LIB)
endif(UNIX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (${sstring_lib_SOURCE_DIR})
include_directories (${${PROJECT_NAME}_SOURCE_DIR})

file(GLOB HEADER_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.h)
file(GLOB SOURCE_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.c ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)

set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE C)

set (BUILD_SHARED_LIBS false)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} sstring_lib)

set(targets_export_name "${PROJECT_NAME}Targets")

set_target_properties(${PROJECT_NAME} PROPERTIES
	PUBLIC_HEADER  	"${HEADER_FILES}"
	OUTPUT_NAME 	"${PROJECT_NAME}")
