#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

cmake_minimum_required (VERSION 3.1)

project (sm_calculator)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/cmocka_lib/cmake/Modules")

include (GNUInstallDirs)
include (AddCMockaTest)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

enable_testing()
include(CTest)

set (CPACK_PACKAGE_NAME 			"sm_calculator")

set (CPACK_PACKAGE_VENDOR 			"SManSoft")
set (CPACK_PACKAGE_CONTACT 			"info@smansoft.com")

set (CPACK_PACKAGE_VERSION_MAJOR 	"0")
set (CPACK_PACKAGE_VERSION_MINOR 	"0")
set (CPACK_PACKAGE_VERSION_PATCH 	"1")
set (CPACK_PACKAGE_BUILD 			"1")

set (CMAKE_COLOR_MAKEFILE ON)

set (CMAKE_RUNTIME_SHARED OFF)
set (CMAKE_RUNTIME_STATIC ON)

if(WIN32)
    set (UNICODE_DEFAULT OFF)
endif(WIN32)

if(UNIX)
    set (UNICODE_DEFAULT OFF)
	if(CMAKE_RUNTIME_STATIC)
    	set (CMAKE_EXE_LINKER_FLAGS "-static-libgcc")
	endif(CMAKE_RUNTIME_STATIC)
endif(UNIX)

set (SM_PACKAGE_NAME "SM Caclulator")

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 14)

if(WIN32)
	set (FLEX_CMD 			"win_flex.exe")
	set (FLEX_CMD_OPTIONS 	"-i -L --nounistd")

	set (BISON_CMD 			"win_bison.exe")
	set (BISON_CMD_OPTIONS 	"-l -d -v")

	add_definitions (/DSM_OS_WINDOWS)
	add_definitions (/DMBCS /D_MBCS)

	if("${CMAKE_PLATFORM}" STREQUAL "x86")
		add_definitions (/D_WIN32)
		add_definitions (/DWIN32)
  		add_definitions (/DSM_X32)
	elseif("${CMAKE_PLATFORM}" STREQUAL "x64")
		add_definitions (/D_WIN32)
		add_definitions (/DWIN32)
		add_definitions (/D_WIN64)
		add_definitions (/DWIN64)
  		add_definitions (/DSM_X64)
	endif()

	if(MSVC)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qspectre /EHsc")
		set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} /Qspectre /EHsc")

		set(CompilerFlags
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_MINSIZEREL
			CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_RELWITHDEBINFO)

		if(CMAKE_RUNTIME_SHARED)
			foreach(CompilerFlag ${CompilerFlags})
 				 string(REPLACE "/MT" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
			endforeach()
		elseif(CMAKE_RUNTIME_STATIC)
			foreach(CompilerFlag ${CompilerFlags})
 				 string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
			endforeach()
		endif(CMAKE_RUNTIME_SHARED)
	endif(MSVC)

endif(WIN32)

if(UNIX)
	set (FLEX_CMD 			"flex")
	set (FLEX_CMD_OPTIONS 	-i -L --nounistd)

	set (BISON_CMD 			"bison")
	set (BISON_CMD_OPTIONS 	-l -d -v)

	message (STATUS "CompilerFlag     	= ${CompilerFlag}")

	add_definitions (-DSM_OS_LINUX)

	if (UNICODE)
  		add_definitions (-DUNICODE -D_UNICODE -UMBCS -U_MBCS)
	else (UNICODE)
  		add_definitions (-UUNICODE -U_UNICODE -DMBCS -D_MBCS)
	endif (UNICODE)

	if("${CMAKE_PLATFORM}" STREQUAL "x86")
  		add_definitions (-DSM_X32)
	elseif("${CMAKE_PLATFORM}" STREQUAL "x64")
  		add_definitions (-DSM_X64)
	endif()

endif(UNIX)

set(CPACK_PACKAGE_VERSION 	${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.${CPACK_PACKAGE_BUILD})

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
	set(SM_SYSTEM_BITS x86)
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
	set(SM_SYSTEM_BITS x64)	
endif()

set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${SM_SYSTEM_BITS}")
set (CMAKE_INSTALL_PREFIX "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${SM_SYSTEM_BITS}")

set(CPACK_IGNORE_FILES 			"/CVS/;/.svn/;/.bzr/;/.hg/;/.git/;.swp\$;.#;/#;/.vs/")
set(CPACK_SOURCE_IGNORE_FILES 	"/CVS/;/.svn/;/.bzr/;/.hg/;/.git/;.swp\$;.#;/#;/.vs/;/_build/;/_debug/;/_release/;/x64/;/x86/;/Win32/")

if(WIN32)
	set(CPACK_GENERATOR 		"ZIP")
    set(CPACK_SOURCE_GENERATOR 	"ZIP")
endif(WIN32)

if(UNIX)
    set(CPACK_GENERATOR 		"TGZ")
    set(CPACK_SOURCE_GENERATOR 	"TGZ")
endif(UNIX)

add_subdirectory (deps)
add_subdirectory (log_lib)
add_subdirectory (sm_calc)

add_subdirectory (tests)

install(FILES
		"${CMAKE_SOURCE_DIR}/README"
		"${CMAKE_SOURCE_DIR}/LICENSE"
		"${CMAKE_SOURCE_DIR}/HELP"
	DESTINATION
		"${CMAKE_INSTALL_DATADIR}/smansoft/doc"
	)

include(CPack)
