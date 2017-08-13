# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

find_package(Threads REQUIRED)

set(UTILITY_HEADERS
    include/unicorn/utility/asset/Content.hpp
    include/unicorn/utility/asset/FileReader.hpp
    include/unicorn/utility/asset/Handler.hpp
    include/unicorn/utility/asset/SimpleStorage.hpp
    include/unicorn/utility/asset/Storage.hpp
    include/unicorn/utility/concurrent/UnorderedMap.hpp
    include/unicorn/utility/Logger.hpp
    include/unicorn/utility/SharedMacros.hpp
    include/unicorn/utility/templates/Singleton.hpp
    include/unicorn/utility/templates/Singleton.imp
    include/unicorn/utility/Memory.hpp
)

set(UTILITY_SOURCES
    sources/asset/Content.cpp
    sources/asset/FileReader.cpp
    sources/asset/Handler.cpp
    sources/asset/Storage.cpp
    sources/asset/SimpleStorage.cpp
    sources/Logger.cpp
    sources/Memory.cpp
)

add_library(utility OBJECT
    ${UTILITY_HEADERS}
    ${UTILITY_SOURCES}
)

include(InstallInfo)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${UNICORN_INSTALL_INCLUDE_DIR}
    COMPONENT godlike_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
