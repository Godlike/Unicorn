# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED UNICORN_ENGINE_NAME)
    set(UNICORN_ENGINE_NAME "UnicornEngine" CACHE STRING "Project name for UnicornEngine library")
endif ()

if (NOT DEFINED UNICORN_ENGINE_LIB)
    set(UNICORN_ENGINE_LIB ${UNICORN_ENGINE_NAME} CACHE STRING "Name of UnicornEngine library")
endif ()

project(${UNICORN_ENGINE_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /MP /W3") # TODO set WX (warning as error)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_definitions(-DVULKAN_HPP_NO_EXCEPTIONS -DVULKAN_HPP_TYPESAFE_CONVERSION)

## Should be uncommented when TinyObjLoader library is used
# include(TinyObjLoaderConfig)
# add_subdirectory(${TINYOBJLOADER_ROOT_DIR})

## GLM
include(GlmConfig)
find_package(GLM)

if (NOT GLM_FOUND)
    add_subdirectory(${GLM_ROOT_DIR})
endif()

## Loguru
include(loguru)

## STB
include(stb)

## utility
include(utility)

## system
include(system)

## video
include(video)

## Wink Signals
include(WinkSignals)

if (UNIX)
    find_library(Vulkan REQUIRED)
endif()

include_directories(
    ${VULKAN_INCLUDE_DIR}
)

include_directories(
    ${UNICORN_ENGINE_INCLUDE_DIRS}
)

include_directories( SYSTEM
    ${WINK_SIGNALS_INCLUDE_DIR}
    ${TINYOBJLOADER_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LOGURU_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

if (BUILD_SHARED_LIBS)
    add_definitions(-D${UNICORN_ENGINE_LIB}_EXPORTS)
endif()

add_subdirectory(system)
add_subdirectory(utility)
add_subdirectory(video)

set(UNICORN_ENGINE_HEADERS
    include/unicorn/UnicornEngine.hpp
    include/unicorn/Settings.hpp)

set(UNICORN_ENGINE_SOURCES
    source/UnicornEngine.cpp
    source/Settings.cpp)

add_library(${UNICORN_ENGINE_LIB}
    ${UNICORN_ENGINE_HEADERS}
    ${UNICORN_ENGINE_SOURCES}

    $<TARGET_OBJECTS:${UNICORN_ENGINE_UTILITY_LIB}>
    $<TARGET_OBJECTS:${UNICORN_ENGINE_VIDEO_LIB}>
    $<TARGET_OBJECTS:${UNICORN_ENGINE_SYSTEM_LIB}>
)

target_link_libraries(${UNICORN_ENGINE_LIB}
    ${WINDOW_MANAGER_LIBS}
    ${VULKAN_LIBRARY}
)

install( TARGETS ${UNICORN_ENGINE_LIB}
    COMPONENT godlike_libraries
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/godlike
    COMPONENT godlike_headers
    FILES_MATCHING PATTERN "*.hpp"
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Workaround for a bug regarding __builtin_cpu_supports("sse2") call on GCC 5+
    # Call is made by STB but is not directly a bug in STB
    target_link_libraries(${PROJECT_NAME} -lgcc_s -lgcc)
endif()

if (UNICORN_BUILD_DOCUMENTATION)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(DOXYGEN_INPUT_DIRS
            ${UNICORN_ENGINE_INCLUDE_DIRS}
        )
        set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
        string(REPLACE ";" " " DOXYGEN_INPUT "${DOXYGEN_INPUT_DIRS}")

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        add_custom_target( docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
            COMMENT "Generating UnicornEngine API documentation"
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
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(${UNICORN_ENGINE_LIB} Shaders)
