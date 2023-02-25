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

cmake_minimum_required(VERSION 3.0)

#configure variables
set(LIB_NAME "${PROJECT_NAME}")

#configure directories
set(LIBRARY_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib")
set(LIBRARY_SRC_PATH "${LIBRARY_MODULE_PATH}/src")
set(LIBRARY_INCLUDE_PATH "${LIBRARY_MODULE_PATH}/include")

#set sources
file(GLOB_RECURSE LIB_HEADER_FILES "${LIBRARY_INCLUDE_PATH}/*.hpp")
file(GLOB_RECURSE LIB_SOURCE_FILES "${LIBRARY_SRC_PATH}/*.cpp")

# enable FetchContent
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

## add fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 9.1.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmt)

## add spdlog
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "do not use std format, because libc++" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "do use lib fmt" FORCE)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.11.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(spdlog)

## add PlatformFolders
FetchContent_Declare(
        platform_folders
        GIT_REPOSITORY https://github.com/sago007/PlatformFolders
        GIT_TAG 4.2.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(platform_folders)

## add Boxer
FetchContent_Declare(
        Boxer
        GIT_REPOSITORY https://github.com/aaronmjacobs/Boxer
        GIT_TAG aed3fdcab88da5dd925ed5211c21e39bc6c0c147
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Boxer)

## add toml11
FetchContent_Declare(
        toml11
        GIT_REPOSITORY https://github.com/ToruNiina/toml11
        GIT_TAG v3.7.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(toml11)

FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.11.1
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(STB
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG 5736b15f7ea0ffb08dd38af21067c314d6a3aae9
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        )
FetchContent_Populate(STB)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
endif ()

#set library
add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES} ${STB_HEADER_FILES})

#add dependencies
target_link_libraries(${LIB_NAME}
        PUBLIC fmt
        PUBLIC spdlog
        PUBLIC sago::platform_folders
        PUBLIC Boxer
        PUBLIC toml11
        PUBLIC EnTT::EnTT
        PUBLIC SDL2::SDL2main PUBLIC SDL2::SDL2-static
        )

#set includes
target_include_directories(${LIB_NAME} PUBLIC ${LIBRARY_INCLUDE_PATH} PUBLIC "${stb_SOURCE_DIR}")
