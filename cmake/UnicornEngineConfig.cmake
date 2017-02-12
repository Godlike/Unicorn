# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT) 
# (http://opensource.org/licenses/MIT)

set(UNICORN_ENGINE_NAME "UnicornEngine" CACHE STRING "Project name for UnicornEngine library.")

if (NOT DEFINED UNICORN_ENGINE_ROOT)
    set(UNICORN_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/UnicornEngine" CACHE STRING "Path to UnicornEngine root directory.")
endif()

set(UNICORN_ENGINE_INCLUDE_DIR "${UNICORN_ENGINE_ROOT}/include" CACHE STRING "Path to UnicornEngine include directory.")
set(UNICORN_ENGINE_LIB ${UNICORN_ENGINE_NAME} CACHE STRING "Name of UnicornEngine library.")
