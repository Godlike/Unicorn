cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED VORPAL_ENGINE_NAME)
    set(VORPAL_ENGINE_NAME "VorpalEngine" CACHE STRING "Project name for VorpalEngine library")
endif ()

if (NOT DEFINED VORPAL_ENGINE_LIB)
    set(VORPAL_ENGINE_LIB ${VORPAL_ENGINE_NAME} CACHE STRING "Name of VorpalEngine library")
endif ()

project(${VORPAL_ENGINE_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

include(AssimpConfig)
add_subdirectory("external/assimp")

include(GlfwConfig)
add_subdirectory("external/glfw")

include(GlmConfig)
add_subdirectory("external/glm")

include(loguru)
include(stb)

find_library(Vulkan REQUIRED)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()

include_directories(
        ${VORPAL_ENGINE_INCLUDE_DIR}
        ${ASSIMP_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${LOGURU_INCLUDE_DIR}
        ${STB_INCLUDE_DIR}
        ${VULKAN_INCLUDE_DIR}
)

if (BUILD_SHARED_LIBS)
    add_definitions(-DVORPAL_SHARED)
endif ()

add_library(${VORPAL_ENGINE_LIB}
        include/vorpal/VorpalEngine.hpp
        include/vorpal/video/Renderer.hpp
        include/vorpal/video/Graphics.hpp
        include/vorpal/core/Utility.hpp
        include/vorpal/core/Timer.hpp
        include/vorpal/core/Settings.hpp
        include/vorpal/core/Logger.hpp

        sources/VorpalEngine.cpp
        sources/Renderer.cpp
        sources/Graphics.cpp
        sources/Timer.cpp
        sources/Settings.cpp
        sources/Logger.cpp)

target_link_libraries(${VORPAL_ENGINE_LIB} ${GLFW_LIB} ${VULKAN_LIBRARY})
