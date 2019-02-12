# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(SanicJymper)

include(UnicornRenderConfig)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Unicorn::Render)
