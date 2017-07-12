# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(UNICORN_ENGINE_NAME "UnicornEngine" CACHE STRING "Project name for UnicornEngine library")

if (NOT DEFINED UNICORN_ENGINE_ROOT)
    set(UNICORN_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/UnicornEngine" CACHE STRING "Path to UnicornEngine root directory")
endif()

set(UNICORN_ENGINE_INCLUDE_DIRS
    "${UNICORN_ENGINE_ROOT}/include"
    "${UNICORN_ENGINE_ROOT}/system/include"
    "${UNICORN_ENGINE_ROOT}/video/include"
    "${UNICORN_ENGINE_ROOT}/utility/include"
    CACHE LIST "List of UnicornEngine include directories")

include(${UNICORN_ENGINE_ROOT}/cmake/WinkSignals.cmake)
include(${UNICORN_ENGINE_ROOT}/cmake/GlmConfig.cmake)

set(UNICORN_ENGINE_EXTERNAL_INCLUDE_DEPENDENCIES
    ${WINK_SIGNALS_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
	${STB_INCLUDE_DIR}
    CACHE LIST "List of UnicornEngine external include dependencies")

set(UNICORN_ENGINE_LIB ${UNICORN_ENGINE_NAME} CACHE STRING "Name of UnicornEngine library")

if (BUILD_SHARED_LIBS)
    add_definitions(-DUNICORN_SHARED)
endif()
