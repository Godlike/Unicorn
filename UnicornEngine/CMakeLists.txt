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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
add_definitions(-DVULKAN_HPP_NO_EXCEPTIONS)

## Should be uncommented when TinyObjLoader library is used
# include(TinyObjLoaderConfig)
# add_subdirectory(${TINYOBJLOADER_ROOT_DIR})

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
	include_directories(
		${VULKAN_INCLUDE_DIR}
	)
endif ()

include_directories(
	${UNICORN_ENGINE_INCLUDE_DIR}
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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror")
endif()

set(UNICORN_ENGINE_HEADERS
	include/unicorn/core/Settings.hpp
	include/unicorn/core/Timer.hpp
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
	include/unicorn/video/Graphics.hpp
	include/unicorn/video/Renderer.hpp
	include/unicorn/UnicornEngine.hpp
)

set(UNICORN_ENGINE_SOURCES
	sources/core/Settings.cpp
	sources/core/Timer.cpp
	sources/utility/asset/Content.cpp
	sources/utility/asset/FileReader.cpp
	sources/utility/asset/Handler.cpp
	sources/utility/asset/SimpleStorage.cpp
	sources/utility/asset/Storage.cpp
	sources/utility/Logger.cpp
	sources/video/Graphics.cpp
	sources/video/Renderer.cpp
	sources/UnicornEngine.cpp
)

add_library(${UNICORN_ENGINE_LIB}
	${UNICORN_ENGINE_HEADERS}
	${UNICORN_ENGINE_SOURCES}
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# Workaround for a bug regarding __builtin_cpu_supports("sse2") call on GCC 5+
	# Call is made by STB but is not directly a bug in STB
	target_link_libraries(${PROJECT_NAME} -lgcc_s -lgcc)
endif()

if (UNICORN_BUILD_DOCUMENTATION)
	find_package(Doxygen)

	if (DOXYGEN_FOUND)
		set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
		set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/include)

		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
		add_custom_target( docs
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
			COMMENT "Generating UnicornEngine API documentation"
		)
	else()
		message(FATAL "Doxygen is required to build documentation")
	endif()
endif()

target_link_libraries(${UNICORN_ENGINE_LIB} ${VULKAN_LIBRARY} ${GLFW_LIB})

if(UNIX)
# TODO Add shader compilation at Linux.
elseif(WIN32)
if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
	"${CMAKE_SOURCE_DIR}/UnicornEngine/data/shaders/*.frag"
	"${CMAKE_SOURCE_DIR}/UnicornEngine/data/shaders/*.vert"
	)
	
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_BINARY_DIR}/data/shaders/${FILE_NAME}.spv")
  add_custom_command(
	OUTPUT ${SPIRV}
	COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/data/shaders/"
	COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
	DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
	Shaders 
	DEPENDS ${SPIRV_BINARY_FILES}
	)

add_dependencies(${UNICORN_ENGINE_LIB} Shaders)

add_custom_command(TARGET ${UNICORN_ENGINE_LIB} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${UNICORN_ENGINE_LIB}>/data/shaders/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_BINARY_DIR}/data/shaders"
		"$<TARGET_FILE_DIR:${UNICORN_ENGINE_LIB}>/data/shaders"
		)
endif()