# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

include_directories(
    ${WINDOW_MANAGER_ADAPTER_INCLUDE_DIR}
)

set(WINDOW_MANAGER_HEADERS
    include/unicorn/system/adapters/${UNICORN_WINDOW_MANAGER}/config.hpp
    include/unicorn/system/adapters/${UNICORN_WINDOW_MANAGER}/Adapter.hpp

    include/unicorn/system/profilers/MonitorProfiler.hpp
    include/unicorn/system/profilers/WindowProfiler.hpp

    include/unicorn/system/CustomValue.hpp
    include/unicorn/system/GammaRamp.hpp
    include/unicorn/system/Monitor.hpp
    include/unicorn/system/MonitorMemento.hpp
    include/unicorn/system/VideoMode.hpp
    include/unicorn/system/Window.hpp
    include/unicorn/system/WindowAttribute.hpp
    include/unicorn/system/WindowHint.hpp
)

set(WINDOW_MANAGER_SOURCES
    source/adapters/${UNICORN_WINDOW_MANAGER}/Adapter.cpp

    source/profilers/MonitorProfiler.cpp
    source/profilers/WindowProfiler.cpp

    source/GammaRamp.cpp
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
