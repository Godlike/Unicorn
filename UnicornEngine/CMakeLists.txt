# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED UNICORN_ENGINE_NAME)
	set(UNICORN_ENGINE_NAME "UnicornEngine" CACHE STRING "Project name for UnicornEngine library.")
endif ()

if (NOT DEFINED UNICORN_ENGINE_LIB)
	set(UNICORN_ENGINE_LIB ${UNICORN_ENGINE_NAME} CACHE STRING "Name of UnicornEngine library.")
endif ()

project(${UNICORN_ENGINE_NAME})

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
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force GLFW to build static library." FORCE)

add_subdirectory(${GLFW_ROOT_DIR})

# Restore initial BUILD_SHARED_LIBS value
set(BUILD_SHARED_LIBS ${TMP_BUILD_SHARED_LIBS} CACHE BOOL "Restored initial BUILD_SHARED_LIBS value." FORCE)

## GLM
include(GlmConfig)
add_subdirectory(${GLM_ROOT_DIR})

## Loguru
include(loguru)

## STB
include(stb)

find_library(Vulkan REQUIRED)
message(STATUS "Vulkan include dir: ${VULKAN_INCLUDE_DIR}")

include_directories( ${UNICORN_ENGINE_INCLUDE_DIR} )

include_directories( SYSTEM
	${TINYOBJLOADER_INCLUDE_DIR}
	${VULKAN_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${LOGURU_INCLUDE_DIR}
	${STB_INCLUDE_DIR}
)

if (BUILD_SHARED_LIBS)
	add_definitions(-DUNICORN_SHARED)
endif()

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wall -Werror")
endif()

add_library(${UNICORN_ENGINE_LIB}
	include/unicorn/system/Settings.hpp
	include/unicorn/system/Timer.hpp
	include/unicorn/system/Window.hpp
	include/unicorn/utility/asset/Content.hpp
	include/unicorn/utility/asset/FileReader.hpp
	include/unicorn/utility/asset/Handler.hpp
	include/unicorn/utility/asset/SimpleStorage.hpp
	include/unicorn/utility/asset/Storage.hpp
	include/unicorn/utility/concurrent/UnorderedMap.hpp
	include/unicorn/utility/Logger.hpp
	include/unicorn/utility/SharedMacros.hpp
	include/unicorn/utility/templates/Singleton.hpp
	include/unicorn/utility/templates/Singleton.imp
	include/unicorn/graphics/Graphics.hpp
	include/unicorn/graphics/Vulkan/VulkanRenderer.hpp
	include/unicorn/graphics/Vulkan/VulkanUtils.hpp
	include/unicorn/graphics/Vulkan/VulkanBuffer.hpp
	include/unicorn/graphics/Texture.hpp
	include/unicorn/graphics/Vertex.hpp
	include/unicorn/graphics/SceneGraph.hpp
	include/unicorn/graphics/Camera.hpp
	include/unicorn/graphics/Mesh.hpp
	include/unicorn/graphics/Model.hpp
	include/unicorn/graphics/Sprite.hpp
	include/unicorn/graphics/Shader.hpp
	include/unicorn/UnicornEngine.hpp
	include/unicorn/graphics/Vulkan/VulkanDevice.hpp
	include/unicorn/graphics/SceneNode.hpp

	sources/system/Settings.cpp
	sources/system/Timer.cpp
	sources/system/Window.cpp
	sources/utility/asset/Content.cpp
	sources/utility/asset/FileReader.cpp
	sources/utility/asset/Handler.cpp
	sources/utility/asset/SimpleStorage.cpp
	sources/utility/asset/Storage.cpp
	sources/utility/Logger.cpp
	sources/graphics/Graphics.cpp
	sources/graphics/Vulkan/VulkanRenderer.cpp
	sources/graphics/Texture.cpp
	sources/graphics/Vertex.cpp
	sources/graphics/SceneGraph.cpp
	sources/graphics/Vulkan/VulkanUtils.cpp
	sources/graphics/Camera.cpp
	sources/graphics/Mesh.cpp
	sources/graphics/Model.cpp
	sources/graphics/Shader.cpp
	sources/graphics/Sprite.cpp
	sources/graphics/Vulkan/VulkanDevice.cpp
	sources/graphics/Vulkan/VulkanGraphicsSubsystem.cpp
	sources/UnicornEngine.cpp
)

target_link_libraries(${UNICORN_ENGINE_LIB} ${GLFW_LIB} ${VULKAN_LIBRARY})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# Workaround for a bug regarding __builtin_cpu_supports("sse2") call on GCC 5+
	# Call is made by STB but is not directly a bug in STB
	target_link_libraries(${PROJECT_NAME} -lgcc_s -lgcc)
endif()
