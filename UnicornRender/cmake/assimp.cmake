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

set(ASSIMP_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/external/assimp/include" CACHE STRING "Path to generated config file")
set(ASSIMP_INCLUDE_DIRS  ${ASSIMP_CONFIG_PATH} ${ASSIMP_INCLUDE_DIR})

set(ASSIMP_LIB "assimp" CACHE STRING "Name of Assimp library")

# if(WIN32)
#     if(MSVC12)
#         set(ASSIMP_MSVC_VERSION "vc120")
#     elseif(MSVC14)
#         set(ASSIMP_MSVC_VERSION "vc140")
#     else()
#         set(ASSIMP_MSVC_VERSION "vc141")
#     endif(MSVC12)

#     set(ASSIMP_LIBRARY_RELEASE "assimp-${ASSIMP_MSVC_VERSION}-mt.lib")
#     set(ASSIMP_LIBRARY_DEBUG "assimp-${ASSIMP_MSVC_VERSION}-mtd.lib")

#     if(CMAKE_BUILD_TYPE MATCHES RELEASE)
#         set(ASSIMP_LIB ${ASSIMP_LIBRARY_RELEASE})
#     else()
#         set(ASSIMP_LIB ${ASSIMP_LIBRARY_DEBUG})
#     endif()

#         # FUNCTION(ASSIMP_COPY_BINARIES TargetDirectory)
#         #     ADD_CUSTOM_TARGET(AssimpCopyBinaries
#         #         COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/bin${ASSIMP_ARCHITECTURE}/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll     ${TargetDirectory}/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll
#         #         COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/bin${ASSIMP_ARCHITECTURE}/assimp-${ASSIMP_MSVC_VERSION}-mt.dll      ${TargetDirectory}/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll
#         #     COMMENT "Copying Assimp binaries to '${TargetDirectory}'"
#         #     VERBATIM)
#         # ENDFUNCTION(ASSIMP_COPY_BINARIES)
# else()
# endif()
