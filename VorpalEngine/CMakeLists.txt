cmake_minimum_required(VERSION 3.2)
cmake_policy(VERSION 3.2)

project(VorpalEngine)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(LOGURU REQUIRED)
find_package(STB REQUIRED)

message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
message(STATUS "Found LOGURU in ${LOGURU_INCLUDE_DIR}")
message(STATUS "Found STB in ${LOGURU_INCLUDE_DIR}")