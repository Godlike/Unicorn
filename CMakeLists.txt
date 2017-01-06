cmake_minimum_required(VERSION 3.0)

project(73s-studio)

option(VORPAL_BUILD_DEMOS "Build VorpalEngine demo projects" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(BUILD_SHARED_LIBS ON CACHE BOOL "Flag indicating if we want to build shared libraries")

set(VORPAL_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/VorpalEngine" CACHE STRING "Path to VorpalEngine root directory")

include(VorpalEngineConfig)

add_subdirectory(${VORPAL_ENGINE_ROOT})

if (${VORPAL_BUILD_DEMOS})
	add_subdirectory(demo)
endif()
