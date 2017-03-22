# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT) 
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)

project(Godlike)

option(UNICORN_BUILD_DOCUMENTATION "Build UnicornEngine documentation" OFF)
option(UNICORN_BUILD_DEMOS "Build UnicornEngine demo projects" ON)
option(UNICORN_BUILD_TESTS "Build UnicornEngine tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(BUILD_SHARED_LIBS ON CACHE BOOL "Flag indicating if we want to build shared libraries")

set(UNICORN_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/UnicornEngine" CACHE STRING "Path to UnicornEngine root directory")

include(UnicornEngineConfig)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

add_subdirectory(${UNICORN_ENGINE_ROOT})

if (${UNICORN_BUILD_DEMOS})
    add_subdirectory(demos)
endif()

if (${UNICORN_BUILD_TESTS})
    enable_testing()
    include(CTest)

    add_subdirectory(tests)
endif()
