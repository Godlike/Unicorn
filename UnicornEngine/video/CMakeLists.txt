# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

set(VIDEO_HEADERS
    include/unicorn/video/Graphics.hpp
    include/unicorn/video/Renderer.hpp
)

set(VIDEO_SOURCES
    source/Graphics.cpp
    source/Renderer.cpp
)

add_library(video STATIC
    ${VIDEO_HEADERS}
    ${VIDEO_SOURCES}
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
