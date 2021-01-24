#
#    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmocka_lib/cmake/Modules")

add_subdirectory (cmocka_lib)
add_subdirectory (sstring_lib)
add_subdirectory (getopt_lib)
