set(ASSIMP_DOUBLE_PRECISION OFF CACHE BOOL "Flag to override default ASSIMP_DOUBLE_PRECISION value")
set(ASSIMP_OPT_BUILD_PACKAGES OFF CACHE BOOL "Flag to override default ASSIMP_OPT_BUILD_PACKAGES value")
set(ASSIMP_ANDROID_JNIIOSYSTEM OFF CACHE BOOL "Flag to override default ASSIMP_ANDROID_JNIIOSYSTEM value")
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "Flag to override default ASSIMP_NO_EXPORT value")

set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_ZLIB value")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_ASSIMP_TOOLS value")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_SAMPLES value")

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Flag to override default ASSIMP_BUILD_TESTS value")
set(ASSIMP_COVERALLS OFF CACHE BOOL "Flag to override default ASSIMP_COVERALLS value")

set(ASSIMP_ROOT_DIR "${VORPAL_ENGINE_ROOT}/external/assimp" CACHE STRING "Path to Assimp root directory")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT_DIR}/include" CACHE STRING "Path to Assimp include directory")
