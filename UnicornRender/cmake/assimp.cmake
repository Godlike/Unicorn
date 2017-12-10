# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(ASSIMP_DOUBLE_PRECISION OFF CACHE BOOL "Flag to override default ASSIMP_DOUBLE_PRECISION value")
set(ASSIMP_OPT_BUILD_PACKAGES OFF CACHE BOOL "Flag to override default ASSIMP_OPT_BUILD_PACKAGES value")
set(ASSIMP_ANDROID_JNIIOSYSTEM OFF CACHE BOOL "Flag to override default ASSIMP_ANDROID_JNIIOSYSTEM value")
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "Flag to override default ASSIMP_NO_EXPORT value")

set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_ZLIB value")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_ASSIMP_TOOLS value")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_SAMPLES value")

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_TESTS value")
set(ASSIMP_COVERALLS OFF CACHE BOOL "Flag to override default ASSIMP_COVERALLS value")

set(ASSIMP_ROOT_DIR "${UNICORN_RENDER_ROOT}/external/assimp" CACHE STRING "Path to Assimp root directory")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT_DIR}/include" CACHE STRING "Path to Assimp include directory")
set(ASSIMP_LIB "assimp" CACHE STRING "Name of ASSIMP library")

set(ASSIMP_INCLUDE_DIRS "lib/assimp/include" "lib/assimp/build/include" ${ASSIMP_INCLUDE_DIR})
