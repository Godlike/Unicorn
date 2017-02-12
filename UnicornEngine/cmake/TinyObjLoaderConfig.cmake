# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

set(TINYOBJLOADER_BUILD_TEST_LOADER OFF CACHE BOOL "Flag to override default TINYOBJLOADER_BUILD_TEST_LOADER value")
set(TINYOBJLOADER_COMPILATION_SHARED OFF CACHE BOOL "Flag to override default TINYOBJLOADER_COMPILATION_SHARED value")
set(TINYOBJLOADER_BUILD_OBJ_STICHER OFF CACHE BOOL "Flag to override default TINYOBJLOADER_BUILD_OBJ_STICHER value")

set(TINYOBJLOADER_ROOT_DIR "${UNICORN_ENGINE_ROOT}/external/tinyobjloader" CACHE STRING "Path to TinyObjLoader root directory")
set(TINYOBJLOADER_INCLUDE_DIR "${TINYOBJLOADER_ROOT_DIR}" CACHE STRING "Path to TinyObjLoader include directory")
set(TINYOBJLOADER_LIB "tinyobjloader" CACHE STRING "Name of TinyObjLoader library")
