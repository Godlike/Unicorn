# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(UnicornRender)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Unicorn::Render ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

add_definitions(-DVULKAN_HPP_NO_EXCEPTIONS
    -DGLM_ENABLE_EXPERIMENTAL
    -DVULKAN_HPP_TYPESAFE_CONVERSION
    -DVULKAN_HPP_NO_SMART_HANDLE
)

include(GlmConfig)
include(Utils)
include(loguru)

if (NOT MULE_FOUND)
    include(MuleConfig)
    add_subdirectory("${MULE_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/Mule")
endif()

include(stb)
include(utility)
include(system)
include(video)
include(WinkSignals)
include(assimp)

# Force BUILD_SHARED_LIBS to be OFF while saving initial value to temp variable
set(TMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force assimp to build static library" FORCE)

add_subdirectory(${ASSIMP_ROOT_DIR})

# Restore initial BUILD_SHARED_LIBS value
set(BUILD_SHARED_LIBS ${TMP_BUILD_SHARED_LIBS} CACHE BOOL "Restored initial BUILD_SHARED_LIBS value" FORCE)

if(WIN32)
    set_target_properties (
        assimp
        IrrXML
        uninstall
        zlib
        zlibstatic
        UpdateAssimpLibsDebugSymbolsAndDLLs
        PROPERTIES FOLDER Assimp
    )
endif()

## Installation info
include(InstallInfo)

message(STATUS "Version: ${UNICORN_VERSION}")

install( DIRECTORY ${WINK_SIGNALS_INCLUDE_DIR}/wink
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_extra
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.h"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install( DIRECTORY ${GLM_INCLUDE_DIR}/glm
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_extra
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.h"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.inl"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install( DIRECTORY ${ASSIMP_INCLUDE_DIR}/assimp
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_extra
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.h"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.inl"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

if (UNIX)
    find_library(Vulkan REQUIRED)
endif()

include_directories(
    ${VULKAN_INCLUDE_DIR}
)

if (BUILD_SHARED_LIBS)
    add_definitions(-D${UNICORN_RENDER_LIB}_EXPORTS)
endif()

if (UNIX)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-command-line-argument")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W3") # remove loguru and put spdlog -> /WX
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(system)
add_subdirectory(utility)
add_subdirectory(video)

set(UNICORN_RENDER_HEADERS
    include/unicorn/UnicornRender.hpp
    include/unicorn/Loggers.hpp
)

set(UNICORN_RENDER_SOURCES
    source/Loggers.cpp
    source/UnicornRender.cpp
)

set(UNICORN_RENDER_EXTRA)

if (MSVC)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/UnicornVersionInfo.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/UnicornVersionInfo.rc

        @ONLY
    )

    set(UNICORN_RENDER_EXTRA
        ${UNICORN_RENDER_EXTRA}
        ${CMAKE_CURRENT_BINARY_DIR}/UnicornVersionInfo.rc
    )
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        ${UNICORN_RENDER_HEADERS}
        ${UNICORN_RENDER_SOURCES}
        ${UNICORN_RENDER_EXTRA}
)



target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${UNICORN_RENDER_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${WINK_SIGNALS_INCLUDE_DIR}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${UNICORN_VERSION}
        SOVERSION ${UNICORN_SOVERSION}
)

target_link_libraries(${PROJECT_NAME}
    Mule::Utilities
    Unicorn::Render::Video
    Unicorn::Render::System
    Unicorn::Render::Utility

    ${VULKAN_LIBRARY}
    ${ASSIMP_LIB}
)

install( TARGETS ${PROJECT_NAME}
    COMPONENT unicorn_libraries
    RUNTIME DESTINATION ${UNICORN_INSTALL_LIBRARY_DIR}
    LIBRARY DESTINATION ${UNICORN_INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${UNICORN_INSTALL_LIBRARY_DIR}
)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_headers
    FILES_MATCHING PATTERN "*.hpp"
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/UnicornRenderConfigVersion.cmake
    VERSION ${UNICORN_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/UnicornRenderConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/UnicornRenderConfig.cmake

    @ONLY
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/UnicornRenderConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/UnicornRenderConfigVersion.cmake
    DESTINATION ${UNICORN_INSTALL_LIBRARY_DIR}
    COMPONENT unicorn_headers
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Workaround for a bug regarding __builtin_cpu_supports("sse2") call on GCC 5+
    # Call is made by STB but is not directly a bug in STB
    target_link_libraries(${UNICORN_RENDER_LIB} -lgcc_s -lgcc)
endif()

if (UNICORN_BUILD_DOCUMENTATION)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(DOXYGEN_INPUT_DIRS
            ${UNICORN_RENDER_INCLUDE_DIRS}
        )
        set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
        string(REPLACE ";" " " DOXYGEN_INPUT "${DOXYGEN_INPUT_DIRS}")

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        add_custom_target( docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
            COMMENT "Generating Unicorn Render API documentation"
        )
    else()
        message(FATAL_ERROR "Doxygen is required to build documentation")
    endif()
endif()

if(UNIX)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
elseif(WIN32)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
    else()
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
    endif()
endif()

set(UNICORN_TEXTURE_DIRECTORY ${UNICORN_RENDER_ROOT}/data/textures)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/data/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/data/shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shaders/"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/textures/"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/models/"
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${UNICORN_TEXTURE_DIRECTORY}"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${UNICORN_RENDER_ROOT}/data/models"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/models"
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

file(GLOB_RECURSE TEXTURES_FILES
    "${UNICORN_RENDER_ROOT}/data/textures/*.png"
    "${UNICORN_RENDER_ROOT}/data/textures/*.jpg"
)

file(GLOB_RECURSE MODELS_FILES
    "${UNICORN_RENDER_ROOT}/data/models/*.png"
    "${UNICORN_RENDER_ROOT}/data/models/*.jpg"
    "${UNICORN_RENDER_ROOT}/data/models/*.bin"
    "${UNICORN_RENDER_ROOT}/data/models/*.gltf"
    "${UNICORN_RENDER_ROOT}/data/models/*.glb"
    "${UNICORN_RENDER_ROOT}/data/models/*.obj"
    "${UNICORN_RENDER_ROOT}/data/models/*.fbx"
)

add_custom_target(
    CompileEngineData
    DEPENDS
    ${SPIRV_BINARY_FILES}
    SOURCES
    ${GLSL_SOURCE_FILES}
    ${TEXTURES_FILES}
    ${MODELS_FILES}
)

add_dependencies(${UNICORN_RENDER_LIB} CompileEngineData)

source_group("Shaders" FILES ${GLSL_SOURCE_FILES})
source_group("Textures" FILES ${TEXTURES_FILES})
source_group("Models" FILES ${MODELS_FILES})

if(WIN32)
    set_target_properties (
        assimp
        IrrXML
        uninstall
        zlib
        zlibstatic
        UpdateAssimpLibsDebugSymbolsAndDLLs
        PROPERTIES FOLDER Assimp
    )
    set_target_properties (
        CompileEngineData
        UnicornRender
        system
        utility
        video
        PROPERTIES FOLDER Unicorn
)
endif()

install( FILES ${SPIRV_BINARY_FILES}
    DESTINATION ${UNICORN_INSTALL_DATA_DIR}/data/shaders
    COMPONENT unicorn_data
)

install( DIRECTORY ${UNICORN_TEXTURE_DIRECTORY}/
    DESTINATION ${UNICORN_INSTALL_DATA_DIR}/data/textures
    COMPONENT unicorn_data
)
