# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

include_directories(
    ${WINDOW_MANAGER_ADAPTER_INCLUDE_DIR}
)

set(WINDOW_MANAGER_HEADERS
    include/unicorn/window_manager/adapters/${UNICORN_WINDOW_MANAGER}/config.hpp
    include/unicorn/window_manager/adapters/${UNICORN_WINDOW_MANAGER}/Adapter.hpp

    include/unicorn/window_manager/profilers/WindowProfiler.hpp

    include/unicorn/window_manager/GammaRamp.hpp
    include/unicorn/window_manager/Hub.hpp
    include/unicorn/window_manager/Monitor.hpp
    include/unicorn/window_manager/MonitorMemento.hpp
    include/unicorn/window_manager/VideoMode.hpp
    include/unicorn/window_manager/Window.hpp
)

set(WINDOW_MANAGER_SOURCES
    source/adapters/${UNICORN_WINDOW_MANAGER}/Adapter.cpp

    source/profilers/WindowProfiler.cpp

    source/GammaRamp.cpp
    source/Hub.cpp
    source/Monitor.cpp
    source/Window.cpp
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(WindowManager STATIC
    ${WINDOW_MANAGER_HEADERS}
    ${WINDOW_MANAGER_SOURCES}
)
