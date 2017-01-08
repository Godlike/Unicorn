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

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

## Should be uncommented when Assimp library is used
# include(AssimpConfig)
# add_subdirectory(${ASSIMP_ROOT_DIR})

include(GlfwConfig)
add_subdirectory(${GLFW_ROOT_DIR})

include(GlmConfig)
add_subdirectory(${GLM_ROOT_DIR})

include(loguru)
include(stb)

find_library(Vulkan REQUIRED)

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
    include/vorpal/core/Settings.hpp
    include/vorpal/core/Timer.hpp
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
    include/vorpal/video/Graphics.hpp
    include/vorpal/video/Renderer.hpp
    include/vorpal/VorpalEngine.hpp

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
    sources/VorpalEngine.cpp
)

target_link_libraries(${VORPAL_ENGINE_LIB} ${GLFW_LIB} ${VULKAN_LIBRARY})
