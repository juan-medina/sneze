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

cmake_minimum_required(VERSION 3.4)

#configure variables
set(LIB_NAME "${PROJECT_NAME}")

#configure directories
set(LIBRARY_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib")
set(LIBRARY_SRC_PATH "${LIBRARY_MODULE_PATH}/src")
set(LIBRARY_INCLUDE_PATH "${LIBRARY_MODULE_PATH}/include")

#set sources
file(GLOB_RECURSE LIB_HEADER_FILES "${LIBRARY_INCLUDE_PATH}/*.hpp")
file(GLOB_RECURSE LIB_SOURCE_FILES "${LIBRARY_SRC_PATH}/*.cpp")

#create embedded resources
set(EMBEDDED_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/embedded-src/include/sneze/embedded")

#create embedded directory
file(MAKE_DIRECTORY "${EMBEDDED_OUTPUT_DIR}")

set_property(GLOBAL PROPERTY EMBEDDED_FILES)

function(add_embedded_resource INPUT_FILE OUTPUT_FILE)
    set(INPUT_EMBEDDED_FILE "${PROJECT_SOURCE_DIR}/lib/embedded/${INPUT_FILE}")
    set(OUTPUT_EMBEDDED_FILE "${EMBEDDED_OUTPUT_DIR}/${OUTPUT_FILE}.hpp")

    get_property(local_EMBEDDED_FILES GLOBAL PROPERTY EMBEDDED_FILES)

    add_custom_command(
            OUTPUT "${OUTPUT_EMBEDDED_FILE}"
            COMMAND "dump" "${INPUT_EMBEDDED_FILE}" "${OUTPUT_EMBEDDED_FILE}"
            DEPENDS "${INPUT_EMBEDDED_FILE}"
    )

    list(APPEND local_EMBEDDED_FILES "${OUTPUT_EMBEDDED_FILE}")
    set_property(GLOBAL PROPERTY EMBEDDED_FILES "${local_EMBEDDED_FILES}")
    message(
            "add embedded resource: ${INPUT_EMBEDDED_FILE} -> ${OUTPUT_EMBEDDED_FILE}"
    )
endfunction()

add_embedded_resource("sprites/sneze.png" "sneze_logo_data")
add_embedded_resource("fonts/fira_mono.fnt" "mono_font_data")
add_embedded_resource("fonts/fira_mono_0.png" "mono_font_texture_data")
add_embedded_resource("fonts/tilt_warp.fnt" "regular_font_data")
add_embedded_resource("fonts/tilt_warp_0.png" "regular_font_texture_data")

get_property(EMBEDDED_FILES GLOBAL PROPERTY EMBEDDED_FILES)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/embedded-src/include")


# enable FetchContent
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# set default flags for dependencies
set(CMAKE_CXX_FLAGS ${SAVED_CMAKE_CXX_FLAGS})

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

## add EnTT
FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.11.1
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(EnTT)

## add SDL2
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

## add SDL2_image
FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.6.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(SDL2_image)

## add rapidjson
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "")
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "")
FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG v1.1.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(rapidjson)

set(RapidJSON_DIR ${rapidjson_SOURCE_DIR})
find_package(RapidJSON REQUIRED QUIET)
set(rapidjson_INCLUDE_DIR ${rapidjson_SOURCE_DIR}/include)

include_directories(SYSTEM ${rapidjson_INCLUDE_DIR})

# restore flags
set(CMAKE_CXX_FLAGS ${SNEZE_CXX_FLAGS})

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
endif ()


#set library
add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES} ${EMBEDDED_FILES})

add_dependencies(${LIB_NAME} dump)

#add dependencies
target_link_libraries(${LIB_NAME}
        PUBLIC fmt
        PUBLIC spdlog
        PUBLIC sago::platform_folders
        PUBLIC Boxer
        PUBLIC EnTT::EnTT
        PUBLIC SDL2::SDL2-static
        PUBLIC SDL2_image::SDL2_image-static
        )

#set includes
target_include_directories(${LIB_NAME} PUBLIC ${LIBRARY_INCLUDE_PATH})
