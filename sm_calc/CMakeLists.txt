#
#    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

project(sm_calc)

cmake_minimum_required(VERSION 3.1)

message (STATUS "PROJECT_NAME     	= ${PROJECT_NAME}")

enable_language (C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (FLEX_INPUT 	"${CMAKE_CURRENT_SOURCE_DIR}/sm_calc.l")
set (FLEX_OUTPUT_C 	"${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_l.c")
set (FLEX_OUTPUT_H 	"${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_l.h")

set (BISON_INPUT 	"${CMAKE_CURRENT_SOURCE_DIR}/sm_calc.y")
set (BISON_OUTPUT_C	"${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_y.c")
set (BISON_OUTPUT_H     "${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_y.h")

if(WIN32)
	add_custom_command(OUTPUT "${FLEX_OUTPUT_C}" "${FLEX_OUTPUT_H}"
		COMMAND "cmd" ARGS "/C ${FLEX_CMD} ${FLEX_CMD_OPTIONS} --header=${FLEX_OUTPUT_H} -o ${FLEX_OUTPUT_C} ${FLEX_INPUT}")
	add_custom_command(OUTPUT "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}"
		COMMAND "cmd" ARGS "/C ${BISON_CMD} ${BISON_CMD_OPTIONS} --defines=${BISON_OUTPUT_H} -o ${BISON_OUTPUT_C} ${BISON_INPUT}")
endif(WIN32)

if(UNIX)
	add_custom_command(OUTPUT "${FLEX_OUTPUT_C}" "${FLEX_OUTPUT_H}"
		COMMAND "${FLEX_CMD}" ARGS ${FLEX_CMD_OPTIONS} "--header=${FLEX_OUTPUT_H}" "-o" "${FLEX_OUTPUT_C}" "${FLEX_INPUT}")
	add_custom_command(OUTPUT "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}"
		COMMAND "${BISON_CMD}" ARGS ${BISON_CMD_OPTIONS} "--defines=${BISON_OUTPUT_H}" "-o" "${BISON_OUTPUT_C}" "${BISON_INPUT}")
endif(UNIX)

include_directories (${sstring_lib_SOURCE_DIR})
include_directories (${getopt_lib_SOURCE_DIR})
include_directories (${log_lib_SOURCE_DIR})
include_directories (${${PROJECT_NAME}_SOURCE_DIR})

file(GLOB HEADER_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.h)
file(GLOB SOURCE_FILES 		${${PROJECT_NAME}_SOURCE_DIR}/*.c ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)
file(GLOB MANIFEST_FILES 	${${PROJECT_NAME}_SOURCE_DIR}/*.manifest)

set_source_files_properties(${SOURCE_FILES} ${FLEX_OUTPUT_C} ${${FLEX_OUTPUT_C}} PROPERTIES LANGUAGE C)

if(WIN32)
	add_definitions (/D_CONSOLE)

	add_executable(${PROJECT_NAME} "${SOURCE_FILES}" "${HEADER_FILES}" 
		"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${FLEX_OUTPUT_H}" "${BISON_OUTPUT_H}" 
		"${MANIFEST_FILES}")

	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")

	target_link_libraries(${PROJECT_NAME} getopt_lib log_lib sstring_lib)
endif(WIN32)

if(UNIX)
	add_definitions (-D_CONSOLE)

	set(CMAKE_INSTALL_RPATH $ORIGIN/../${CMAKE_INSTALL_LIBDIR})

	add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES}
		"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${FLEX_OUTPUT_H}" "${BISON_OUTPUT_H}")

	target_link_libraries(${PROJECT_NAME} pthread dl m getopt_lib log_lib sstring_lib)
endif(UNIX)

install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}Targets"
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
