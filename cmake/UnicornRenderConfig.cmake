# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(UNICORN_RENDER_NAME "UnicornRender" CACHE STRING "Project name for Unicorn Render library")

if (NOT DEFINED UNICORN_RENDER_ROOT)
    set(UNICORN_RENDER_ROOT "${CMAKE_CURRENT_LIST_DIR}/../UnicornRender" CACHE STRING "Path to Unicorn Render root directory")
endif()

set(UNICORN_RENDER_INCLUDE_DIRS
    "${UNICORN_RENDER_ROOT}/include"
    "${UNICORN_RENDER_ROOT}/system/include"
    "${UNICORN_RENDER_ROOT}/video/include"
    "${UNICORN_RENDER_ROOT}/utility/include"
    CACHE LIST "List of Unicorn Render include directories")

list(APPEND CMAKE_MODULE_PATH "${UNICORN_RENDER_ROOT}/cmake")

include(WinkSignals)
include(GlmConfig)

find_package(GLM)

set(UNICORN_RENDER_EXTERNAL_INCLUDE_DEPENDENCIES
    ${WINK_SIGNALS_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    CACHE LIST "List of Unicorn Render external include dependencies")

set(UNICORN_RENDER_LIB ${UNICORN_RENDER_NAME} CACHE STRING "Name of Unicorn Render library")

if (BUILD_SHARED_LIBS)
    add_definitions(-DUNICORN_SHARED)
endif()
