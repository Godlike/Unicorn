# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED UNICORN_RENDER_NAME)
    set(UNICORN_RENDER_NAME "UnicornRender" CACHE STRING "Project name for Unicorn Render library")
endif()

project(${UNICORN_RENDER_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # TODO: assimp fail -Wall -Werror -pedantic
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /MP /W3") # TODO set WX (warning as error)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_definitions(-DVULKAN_HPP_NO_EXCEPTIONS -DVULKAN_HPP_TYPESAFE_CONVERSION -DVULKAN_HPP_NO_SMART_HANDLE)

## GLM
include(GlmConfig)
find_package(GLM REQUIRED)

if (NOT GLM_FOUND)
    add_subdirectory(${GLM_ROOT_DIR})
endif()

include(loguru)
include(stb)
include(utility)
include(system)
include(video)
include(WinkSignals)
include(assimp)
add_subdirectory(${ASSIMP_ROOT_DIR})

## Installation info
include(InstallInfo)

message(STATUS "Version: ${UNICORN_VERSION}")

install( FILES ${LOGURU_INCLUDE_DIR}/loguru.hpp
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_extra
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

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

if (UNIX)
    find_library(Vulkan REQUIRED)
endif()

include_directories(
    ${VULKAN_INCLUDE_DIR}
)

include_directories(
    ${UNICORN_RENDER_INCLUDE_DIRS}
)

include_directories( SYSTEM
    ${WINK_SIGNALS_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LOGURU_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${ASSIMP_INCLUDE_DIRS}
)

if (BUILD_SHARED_LIBS)
    add_definitions(-D${UNICORN_RENDER_LIB}_EXPORTS)
endif()

add_subdirectory(system)
add_subdirectory(utility)
add_subdirectory(video)

set(UNICORN_RENDER_HEADERS
    include/unicorn/UnicornRender.hpp
    include/unicorn/Settings.hpp)

set(UNICORN_RENDER_SOURCES
    source/UnicornRender.cpp
    source/Settings.cpp)

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

add_library(${UNICORN_RENDER_LIB}
    ${UNICORN_RENDER_HEADERS}
    ${UNICORN_RENDER_SOURCES}
    ${UNICORN_RENDER_EXTRA}

    $<TARGET_OBJECTS:${UNICORN_RENDER_UTILITY_LIB}>
    $<TARGET_OBJECTS:${UNICORN_RENDER_VIDEO_LIB}>
    $<TARGET_OBJECTS:${UNICORN_RENDER_SYSTEM_LIB}>
)

set_target_properties(
    ${UNICORN_RENDER_LIB}
    PROPERTIES
        VERSION ${UNICORN_VERSION}
        SOVERSION ${UNICORN_SOVERSION}
)

target_link_libraries(${UNICORN_RENDER_LIB}
    ${WINDOW_MANAGER_LIBS}
    ${VULKAN_LIBRARY}
    ${ASSIMP_LIB}
)

install( TARGETS ${UNICORN_RENDER_LIB}
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
            "${UNICORN_RENDER_ROOT}/data/textures"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${UNICORN_RENDER_ROOT}/data/models"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/models"
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(${UNICORN_RENDER_LIB} Shaders)

install( FILES ${SPIRV_BINARY_FILES}
    DESTINATION ${UNICORN_INSTALL_DATA_DIR}/data/shaders
    COMPONENT unicorn_data
)
