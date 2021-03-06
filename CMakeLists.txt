# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.2)

project(Unicorn)

option(UNICORN_BUILD_DOCUMENTATION "Build UnicornRender documentation" OFF)
option(UNICORN_BUILD_DEMOS "Build UnicornRender demo projects" ON)
option(UNICORN_BUILD_TESTS "Build UnicornRender tests" ON)
option(BUILD_SHARED_LIBS "Build shared libs" ON)

message(STATUS "${PROJECT_NAME} ${CMAKE_BUILD_TYPE} configuration:")
message(STATUS "-- UNICORN_BUILD_DOCUMENTATION: ${UNICORN_BUILD_DOCUMENTATION}")
message(STATUS "-- UNICORN_BUILD_DEMOS: ${UNICORN_BUILD_DEMOS}")
message(STATUS "-- UNICORN_BUILD_TESTS: ${UNICORN_BUILD_TESTS}")
message(STATUS "-- BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(UnicornRenderConfig)

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
endif()

add_subdirectory(${UNICORN_RENDER_ROOT})

if (${UNICORN_BUILD_DEMOS})
    add_subdirectory(demos)
endif()

if (${UNICORN_BUILD_TESTS})
    enable_testing()
    include(CTest)

    add_subdirectory(tests)
endif()

get_directory_property(UNICORN_HAS_PARENT PARENT_DIRECTORY)

if (UNICORN_HAS_PARENT)
    set(UNICORN_RENDER_LIB ${UNICORN_RENDER_LIB} PARENT_SCOPE)
endif()
