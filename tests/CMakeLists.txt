#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

project(sm_calculator-tests)

cmake_minimum_required (VERSION 3.1)

message (STATUS "PROJECT_NAME     	= ${PROJECT_NAME}")

enable_language (C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set (FLEX_INPUT 	"${sm_calc_SOURCE_DIR}/sm_calc.l")
set (FLEX_OUTPUT_C 	"${sm_calc_SOURCE_DIR}/sm_calc_l.c")

set (BISON_INPUT 	"${sm_calc_SOURCE_DIR}/sm_calc.y")
set (BISON_OUTPUT_C	"${sm_calc_SOURCE_DIR}/sm_calc_y.c")
set (BISON_OUTPUT_H "${sm_calc_SOURCE_DIR}/sm_calc_y.h")

if(WIN32)
	add_definitions (/D_CONSOLE)

	add_custom_command(OUTPUT "${FLEX_OUTPUT_C}"
		COMMAND "cmd" ARGS "/C ${FLEX_CMD} ${FLEX_CMD_OPTIONS} -o ${FLEX_OUTPUT_C} ${FLEX_INPUT}")
	add_custom_command(OUTPUT "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}"
		COMMAND "cmd" ARGS "/C ${BISON_CMD} ${BISON_CMD_OPTIONS} -o ${BISON_OUTPUT_C} ${BISON_INPUT}")
endif(WIN32)

if(UNIX)
	add_definitions (-D_CONSOLE)

	add_custom_command(OUTPUT "${FLEX_OUTPUT_C}"
		COMMAND "${FLEX_CMD}" ARGS ${FLEX_CMD_OPTIONS} "-o" "${FLEX_OUTPUT_C}" "${FLEX_INPUT}")
	add_custom_command(OUTPUT ${BISON_OUTPUT_C} ${BISON_OUTPUT_H}
		COMMAND "${BISON_CMD}" ARGS ${BISON_CMD_OPTIONS} "-o" "${BISON_OUTPUT_C}" "${BISON_INPUT}")
endif(UNIX)

include_directories (${sm_calc_SOURCE_DIR})
include_directories (${sstring_lib_SOURCE_DIR})
include_directories (${getopt_lib_SOURCE_DIR})
include_directories (${log_lib_SOURCE_DIR})
include_directories (${cmocka_SOURCE_DIR}/include)

file(GLOB SM_CALC_HEADER_FILES 		${sm_calc_SOURCE_DIR}/*.h)
file(GLOB SM_CALC_SOURCE_FILES 		${sm_calc_SOURCE_DIR}/*.c ${sm_calc_SOURCE_DIR}/*.cpp)
file(GLOB SM_CALC_MANIFEST_FILES 	${sm_calc_SOURCE_DIR}/*.manifest)
file(GLOB SM_CALC_TEST_PCH_FILES 	framework.h pch.h )

set_source_files_properties(sm_calc_test.c sm_calc_test_files.c sm_calc_test_expr.c
			${SM_CALC_SOURCE_FILES} ${FLEX_OUTPUT_C} ${BISON_OUTPUT_C} ${BISON_OUTPUT_H}
			PROPERTIES COMPILE_DEFINITIONS UNIT_TESTING=1)

### sm_calc_test
if(WIN32)
	add_executable(sm_calc_test sm_calc_test.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}" 
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(WIN32)

if(UNIX)
	add_executable(sm_calc_test sm_calc_test.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}" 
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test pthread dl m getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(UNIX)

add_test(NAME sm_calc_test COMMAND sm_calc_test)

### sm_calc_test_files
if(WIN32)
	add_executable(sm_calc_test_files sm_calc_test_files.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_files getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(WIN32)

if(UNIX)
	add_executable(sm_calc_test_files sm_calc_test_files.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_files pthread dl m getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(UNIX)

set(SM_CALC_TEST_FILES 
	"sm_calc_test.in.01.txt"
	"sm_calc_test.in.02.txt"
	"sm_calc_test.in.03.txt"
	"sm_calc_test.in.04.txt"
	"sm_calc_test.in.05.txt"
	"sm_calc_test.in.06.txt"
	"sm_calc_test.in.07.txt"
	"sm_calc_test.check.01.txt"
	"sm_calc_test.check.02.txt"
	"sm_calc_test.check.03.txt"
	"sm_calc_test.check.04.txt"
	"sm_calc_test.check.05.txt"
	"sm_calc_test.check.06.txt"
	"sm_calc_test.check.07.txt")

foreach(test_file ${SM_CALC_TEST_FILES})
	add_custom_command(
		TARGET sm_calc_test_files POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${test_file}" "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()

add_custom_command(
	TARGET sm_calc_test_files POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_test_files.check.bat" "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
	TARGET sm_calc_test_files POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_test_files.check.sh" "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
	TARGET sm_calc_test_files POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/sm_calc_test_files.check.py" "${CMAKE_CURRENT_BINARY_DIR}")

### sm_calc_test_files_01
add_test(NAME sm_calc_test_files_01 COMMAND sm_calc_test_files "./sm_calc_test.in.01.txt" "./sm_calc_test.out.01.txt")

### sm_calc_test_files_02
add_test(NAME sm_calc_test_files_02 COMMAND sm_calc_test_files "./sm_calc_test.in.02.txt" "./sm_calc_test.out.02.txt")

### sm_calc_test_files_03
add_test(NAME sm_calc_test_files_03 COMMAND sm_calc_test_files "./sm_calc_test.in.03.txt" "./sm_calc_test.out.03.txt")

### sm_calc_test_files_04
add_test(NAME sm_calc_test_files_04 COMMAND sm_calc_test_files "./sm_calc_test.in.04.txt" "./sm_calc_test.out.04.txt")

### sm_calc_test_files_05
add_test(NAME sm_calc_test_files_05 COMMAND sm_calc_test_files "./sm_calc_test.in.05.txt" "./sm_calc_test.out.05.txt")

### sm_calc_test_files_06
add_test(NAME sm_calc_test_files_06 COMMAND sm_calc_test_files "./sm_calc_test.in.06.txt" "./sm_calc_test.out.06.txt")

### sm_calc_test_files_07
add_test(NAME sm_calc_test_files_07 COMMAND sm_calc_test_files "./sm_calc_test.in.07.txt" "./sm_calc_test.out.07.txt")

### sm_calc_test_expr
if(WIN32)
	add_executable(sm_calc_test_expr sm_calc_test_expr.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_expr getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(WIN32)

if(UNIX)
	add_executable(sm_calc_test_expr sm_calc_test_expr.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_expr pthread dl m getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(UNIX)

add_test(NAME sm_calc_test_expr COMMAND sm_calc_test_expr)

### sm_calc_test_log
if(WIN32)
	add_executable(sm_calc_test_log sm_calc_test_log.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_log getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(WIN32)

if(UNIX)
	add_executable(sm_calc_test_log sm_calc_test_log.c "${SM_CALC_TEST_PCH_FILES}" "${SM_CALC_SOURCE_FILES}" "${SM_CALC_HEADER_FILES}"
				"${FLEX_OUTPUT_C}" "${BISON_OUTPUT_C}" "${BISON_OUTPUT_H}" 
				"${SM_CALC_MANIFEST_FILES}")
	target_link_libraries(sm_calc_test_log pthread dl m getopt_lib log_lib sstring_lib ${CMOCKA_STATIC_LIBRARY})
endif(UNIX)

add_test(NAME sm_calc_test_log COMMAND sm_calc_test_log)

### properties

set_tests_properties(sm_calc_test sm_calc_test_expr sm_calc_test_log
						sm_calc_test_files_01 sm_calc_test_files_02 
						sm_calc_test_files_03 sm_calc_test_files_04
						sm_calc_test_files_05 sm_calc_test_files_06
						PROPERTIES WILL_FAIL "0")
set_tests_properties(sm_calc_test_files_07 
						PROPERTIES WILL_FAIL "1") 

if(CMAKE_GENERATOR MATCHES "Visual Studio*")
	set_tests_properties(sm_calc_test sm_calc_test_expr
							sm_calc_test_files_01 sm_calc_test_files_02 
							sm_calc_test_files_03 sm_calc_test_files_04
							sm_calc_test_files_05 sm_calc_test_files_06
							sm_calc_test_files_07
							PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:getopt_lib>;$ENV{PATH}")
else()
	set_tests_properties(sm_calc_test sm_calc_test_expr sm_calc_test_log
							sm_calc_test_files_01 sm_calc_test_files_02 
							sm_calc_test_files_03 sm_calc_test_files_04
							sm_calc_test_files_05 sm_calc_test_files_06
							sm_calc_test_files_07
							PROPERTIES ENVIRONMENT "PATH=${getopt_lib_BINARY_DIR};$ENV{PATH}")
endif()
