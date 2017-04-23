# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

include_directories(
    ${UNICORN_ENGINE_SYSTEM_INCLUDE_DIR}
    ${WINDOW_MANAGER_ADAPTER_INCLUDE_DIR}
)

set(SYSTEM_HEADERS
    include/unicorn/system/Input.hpp
    include/unicorn/system/Manager.hpp
)

set(SYSTEM_SOURCES
    source/Input.cpp
    source/Manager.cpp
)

set(WINDOW_MANAGER_HEADERS
    include/unicorn/system/adapter/${UNICORN_WINDOW_MANAGER}/config.hpp
    include/unicorn/system/adapter/${UNICORN_WINDOW_MANAGER}/Adapter.hpp

    include/unicorn/system/profiler/MonitorProfiler.hpp
    include/unicorn/system/profiler/WindowProfiler.hpp

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
    source/adapter/${UNICORN_WINDOW_MANAGER}/Adapter.cpp

    source/profiler/MonitorProfiler.cpp
    source/profiler/WindowProfiler.cpp

    source/GammaRamp.cpp
    source/Monitor.cpp
    source/Window.cpp
)

set(INPUT_HEADERS
    include/unicorn/system/CursorShape.hpp
    include/unicorn/system/MouseMode.hpp

    include/unicorn/system/input/Action.hpp
    include/unicorn/system/input/Gamepad.hpp
    include/unicorn/system/input/GamepadState.hpp
    include/unicorn/system/input/Key.hpp
    include/unicorn/system/input/MouseButton.hpp

    include/unicorn/system/profiler/GamepadProfiler.hpp
    include/unicorn/system/profiler/KeyProfiler.hpp
    include/unicorn/system/profiler/MouseProfiler.hpp
)

set(INPUT_SOURCES
    source/profiler/GamepadProfiler.cpp
    source/profiler/KeyProfiler.cpp
    source/profiler/MouseProfiler.cpp

    source/input/Gamepad.cpp
)

add_library(system STATIC
    ${WINDOW_MANAGER_HEADERS}
    ${INPUT_HEADERS}
    ${SYSTEM_HEADERS}

    ${WINDOW_MANAGER_SOURCES}
    ${INPUT_SOURCES}
    ${SYSTEM_SOURCES}
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

target_link_libraries(system ${WINDOW_MANAGER_LIBS})
