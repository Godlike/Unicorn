# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)

project(Godlike)

option(UNICORN_BUILD_DOCUMENTATION "Build UnicornEngine documentation" OFF)
option(UNICORN_BUILD_DEMOS "Build UnicornEngine demo projects" ON)
option(UNICORN_BUILD_TESTS "Build UnicornEngine tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(UNICORN_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/UnicornEngine" CACHE STRING "Path to UnicornEngine root directory")

include(UnicornEngineConfig)

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
endif()

add_subdirectory(${UNICORN_ENGINE_ROOT})

if (${UNICORN_BUILD_DEMOS})
    add_subdirectory(demos)
endif()

if (${UNICORN_BUILD_TESTS})
    enable_testing()
    include(CTest)

    add_subdirectory(tests)
endif()
