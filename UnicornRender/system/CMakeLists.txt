# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(UnicornRender_System)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} OBJECT "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Unicorn::Render::System ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(SYSTEM_HEADERS
    include/unicorn/system/Input.hpp
    include/unicorn/system/Manager.hpp
    include/unicorn/system/Timer.hpp
)

set(SYSTEM_SOURCES
    source/Input.cpp
    source/Manager.cpp
    source/Timer.cpp
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

set(SYSTEM_ALL_SOURCES
    ${WINDOW_MANAGER_HEADERS}
    ${INPUT_HEADERS}
    ${SYSTEM_HEADERS}

    ${WINDOW_MANAGER_SOURCES}
    ${INPUT_SOURCES}
    ${SYSTEM_SOURCES}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SYSTEM_ALL_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${WINDOW_MANAGER_ADAPTER_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${WINK_SIGNALS_INCLUDE_DIR}

        PRIVATE
            $<TARGET_PROPERTY:Unicorn::Render::Utility,INTERFACE_INCLUDE_DIRECTORIES>
)

if (WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES FOLDER ${UNICORN_IDE_FOLDER}
    )
endif()

include(InstallInfo)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "system/adapter" EXCLUDE
        PATTERN "system/Manager.hpp" EXCLUDE
        PATTERN "system/profiler" EXCLUDE
)

if (UNIX)
    set_target_properties(
        ${PROJECT_NAME}

        PROPERTIES

        POSITION_INDEPENDENT_CODE ON
    )
endif()

MakeSolutionFilters("${SYSTEM_ALL_SOURCES}")
