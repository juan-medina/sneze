# MIT License
#
# Copyright (c) 2023 Juan Medina
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# CMake build : library

#configure variables
set(LIB_NAME "${PROJECT_NAME}")

#configure directories
set(LIBRARY_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib")
set(LIBRARY_SRC_PATH "${LIBRARY_MODULE_PATH}/src")
set(LIBRARY_INCLUDE_PATH "${LIBRARY_MODULE_PATH}/include")

#set sources
file(GLOB_RECURSE LIB_HEADER_FILES "${LIBRARY_INCLUDE_PATH}/*.h")
file(GLOB_RECURSE LIB_SOURCE_FILES "${LIBRARY_SRC_PATH}/*.cpp")

## add fmt
add_subdirectory(external/fmt)

## add raylib
add_subdirectory(external/raylib)

## add spdlog
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "do not use std format, because libc++" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "do use lib fmt" FORCE)
add_subdirectory(external/spdlog)

## add PlatformFolders
add_subdirectory(external/PlatformFolders)
find_package(platform_folders QUIET)

## add Boxer
add_subdirectory("external/Boxer")

## add toml11
add_subdirectory("external/toml11")

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
endif ()

#set library
add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})

#add dependencies
target_link_libraries(${LIB_NAME} PUBLIC fmt PUBLIC raylib PUBLIC spdlog PUBLIC sago::platform_folders PUBLIC Boxer PUBLIC toml11)

#set includes
target_include_directories(${LIB_NAME} PUBLIC ${LIBRARY_INCLUDE_PATH})
