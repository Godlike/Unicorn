# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

add_subdirectory(WindowManager)

include_directories(
    ${WINDOW_MANAGER_ADAPTER_INCLUDE_DIR}
)

set(SYSTEM_HEADERS
    include/unicorn/system/Manager.hpp
    include/unicorn/system/Timer.hpp
    include/unicorn/system/Settings.hpp
)

set(SYSTEM_SOURCES
    source/Manager.cpp
    source/Timer.cpp
    source/Settings.cpp
)

add_library(system STATIC
    ${SYSTEM_HEADERS}
    ${SYSTEM_SOURCES}
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

target_link_libraries(system ${WINDOW_MANAGER_LIB})
