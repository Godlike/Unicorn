# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

set(UTILITY_HEADERS
    include/unicorn/utility/InternalLoggers.hpp
    include/unicorn/utility/Math.hpp
    include/unicorn/utility/Memory.hpp
)

set(UTILITY_SOURCES
    sources/Memory.cpp
    sources/Math.cpp
)

set(UTILITY_ALL_SOURCES
    ${UTILITY_HEADERS}
    ${UTILITY_SOURCES}
)

add_library(utility OBJECT ${UTILITY_ALL_SOURCES})

include(InstallInfo)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT unicorn_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "utility/InternalLoggers.hpp" EXCLUDE
        PATTERN "utility/Math.hpp" EXCLUDE
        PATTERN "utility/Memory.hpp" EXCLUDE
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

MakeSolutionFilters("${UTILITY_ALL_SOURCES}")
