# Required cmake version
cmake_minimum_required(VERSION 3.5.0)
cmake_policy(SET CMP0048 NEW)

# Specify search path for CMake modules to be loaded by include() 
# and find_package()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

### Add defaults for cmake
# These defaults need to be included before the project() call.
include(DefineCMakeDefaults)

# This will provide -DCMAKE_BUILD_TYPE=Profiling
# and -DCMAKE_BUILD_TYPE=AddressSanitizer
include(DefineCompilerFlags)
include(DefinePlatformDefaults)

project(cmocka VERSION 1.1.5 LANGUAGES C)

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
	set(SM_SYSTEM_BITS x86)
elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
       set(SM_SYSTEM_BITS x64)	
endif()

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})

# SOVERSION scheme: CURRENT.AGE.REVISION
#   If there was an incompatible interface change:
#     Increment CURRENT. Set AGE and REVISION to 0
#   If there was a compatible interface change:
#     Increment AGE. Set REVISION to 0
#   If the source code was changed, but there were no interface changes:
#     Increment REVISION.
set(LIBRARY_VERSION "1.1.5")
set(LIBRARY_SOVERSION "0")

set(BUILD_STATIC_LIB ON)
set(WITH_STATIC_LIB ON)

set(UNIT_TESTING OFF)
set(WITH_CMOCKERY_SUPPORT ON)
set(WITH_EXAMPLES OFF)
set(UNIT_TESTING OFF)
set(PICKY_DEVELOPER OFF)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Unit testing framework for C with mock objects")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VENDOR "Andreas Schneider")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

set(CPACK_PACKAGE_VERSION 	${LIBRARY_VERSION})

set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${LIBRARY_VERSION}-${CMAKE_SYSTEM_NAME}-${SM_SYSTEM_BITS}")
set (CMAKE_INSTALL_PREFIX "${CPACK_PACKAGE_NAME}-${LIBRARY_VERSION}-${CMAKE_SYSTEM_NAME}-${SM_SYSTEM_BITS}")

set(CPACK_IGNORE_FILES 			"/CVS/;/.svn/;/.bzr/;/.hg/;/.git/;.swp\$;.#;/#;/.vs/")
set(CPACK_SOURCE_IGNORE_FILES 	"/CVS/;/.svn/;/.bzr/;/.hg/;/.git/;.swp\$;.#;/#;/.vs/;/_build/;/_debug/;/_release/;/x64/;/x86/;/Win32/;_cmake")

if(WIN32)
	set(CPACK_GENERATOR 		"ZIP")
    set(CPACK_SOURCE_GENERATOR 	"ZIP")
endif(WIN32)

if(UNIX)
    set(CPACK_GENERATOR 		"TGZ")
    set(CPACK_SOURCE_GENERATOR 	"TGZ")
endif(UNIX)

# include cmake files
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CompilerChecks.cmake)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

# config.h checks
include(ConfigureChecks.cmake)

# config.h checks
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# MinGW DLL Naming Workaround
if (MINGW)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif (MINGW)

if("$ENV{CMAKE_INSTALL_PROJECT_FILES}" STREQUAL "ON")
	set(CMAKE_INSTALL_PROJECT_FILES ON)
else()
	set(CMAKE_INSTALL_PROJECT_FILES OFF)
endif()

# check subdirectories
add_subdirectory(include)
add_subdirectory(src)

include(AddCMockaTest)

# cmake config files
if (WIN32)
    set(CMOCKA_LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(CMOCKA_LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE INTERNAL "")
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR} CACHE INTERNAL "")
configure_package_config_file(${PROJECT_NAME}-config.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
                              PATH_VARS
                                  INCLUDE_INSTALL_DIR LIB_INSTALL_DIR
                              INSTALL_DESTINATION
                                  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
                                 COMPATIBILITY
                                     AnyNewerVersion)

if(CMAKE_INSTALL_PROJECT_FILES)
	install(
    	FILES
        	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
	        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    	DESTINATION
	        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    	COMPONENT
        	devel
	)
endif(CMAKE_INSTALL_PROJECT_FILES)

# Add 'make dist' target which makes sure to invoke cmake before
add_custom_target(dist
                  COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

include(CPack)
