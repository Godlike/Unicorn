# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(SanicJymper)

include(UnicornRenderConfig)

include_directories(${UNICORN_RENDER_INCLUDE_DIRS})
include_directories(SYSTEM ${UNICORN_RENDER_EXTERNAL_INCLUDE_DEPENDENCIES})

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${UNICORN_RENDER_LIB})
