# Copyright (C) 2017 by Grapefruit Tech
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED VORPAL_ENGINE_NAME)
    set(VORPAL_ENGINE_NAME "VorpalEngine" CACHE STRING "Project name for VorpalEngine library")
endif ()

if (NOT DEFINED VORPAL_ENGINE_LIB)
    set(VORPAL_ENGINE_LIB ${VORPAL_ENGINE_NAME} CACHE STRING "Name of VorpalEngine library")
endif ()

project(${VORPAL_ENGINE_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()

include(TinyObjLoaderConfig)
add_subdirectory(${TINYOBJLOADER_ROOT_DIR})

## GLFW
# GLFW shall always be statically linked
include(GlfwConfig)

# Force BUILD_SHARED_LIBS to be OFF while saving initial value to temp variable
set(TMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force GLFW to build static library" FORCE)

add_subdirectory(${GLFW_ROOT_DIR})

# Restore initial BUILD_SHARED_LIBS value
set(BUILD_SHARED_LIBS ${TMP_BUILD_SHARED_LIBS} CACHE BOOL "Restored initial BUILD_SHARED_LIBS value" FORCE)

## GLM
include(GlmConfig)
add_subdirectory(${GLM_ROOT_DIR})

## Loguru
include(loguru)

## STB
include(stb)

if (UNIX)
    find_library(Vulkan REQUIRED)
endif ()

include_directories( ${VORPAL_ENGINE_INCLUDE_DIR} )

include_directories( SYSTEM
    ${TINYOBJLOADER_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LOGURU_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${VULKAN_INCLUDE_DIR}
)

if (BUILD_SHARED_LIBS)
    add_definitions(-DVORPAL_SHARED)
endif()

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wall -Werror")
endif()

add_library(${VORPAL_ENGINE_LIB}
    include/vorpal/system/Settings.hpp
    include/vorpal/system/Timer.hpp
    include/vorpal/utility/asset/Content.hpp
    include/vorpal/utility/asset/FileReader.hpp
    include/vorpal/utility/asset/Handler.hpp
    include/vorpal/utility/asset/SimpleStorage.hpp
    include/vorpal/utility/asset/Storage.hpp
    include/vorpal/utility/concurrent/UnorderedMap.hpp
    include/vorpal/utility/Logger.hpp
    include/vorpal/utility/SharedMacros.hpp
    include/vorpal/utility/templates/Singleton.hpp
    include/vorpal/utility/templates/Singleton.imp
    include/vorpal/graphics/Graphics.hpp
    include/vorpal/graphics/VulkanRenderer.hpp
    include/vorpal/graphics/VulkanUtil.hpp
    include/vorpal/graphics/Texture.hpp
    include/vorpal/graphics/Vertex.hpp
    include/vorpal/graphics/Scene.hpp
    include/vorpal/VorpalEngine.hpp

    sources/system/Settings.cpp
    sources/system/Timer.cpp
    sources/utility/asset/Content.cpp
    sources/utility/asset/FileReader.cpp
    sources/utility/asset/Handler.cpp
    sources/utility/asset/SimpleStorage.cpp
    sources/utility/asset/Storage.cpp
    sources/utility/Logger.cpp
    sources/graphics/Graphics.cpp
    sources/graphics/VulkanRenderer.cpp
    sources/graphics/Texture.cpp
    sources/graphics/Vertex.cpp
    sources/graphics/Scene.cpp
    sources/VorpalEngine.cpp
)

target_link_libraries(${VORPAL_ENGINE_LIB} ${GLFW_LIB} ${VULKAN_LIBRARY})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Workaround for a bug regarding __builtin_cpu_supports("sse2") call on GCC 5+
    # Call is made by STB but is not directly a bug in STB
    target_link_libraries(${PROJECT_NAME} -lgcc_s -lgcc)
endif()
