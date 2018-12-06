# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(UnicornRender_Utility)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} OBJECT "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Unicorn::Render::Utility ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(UTILITY_HEADERS
    include/unicorn/utility/InternalLoggers.hpp
    include/unicorn/utility/Math.hpp
    include/unicorn/utility/Memory.hpp
    include/unicorn/utility/Settings.hpp
)

set(UTILITY_SOURCES
    source/Memory.cpp
    source/Math.cpp
    source/Settings.cpp
)

set(UTILITY_ALL_SOURCES
    ${UTILITY_HEADERS}
    ${UTILITY_SOURCES}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${UTILITY_ALL_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${WINK_SIGNALS_INCLUDE_DIR}
            ${ASSIMP_INCLUDE_DIRS}

            $<TARGET_PROPERTY:Mule::Utilities,INTERFACE_INCLUDE_DIRECTORIES>

        PRIVATE
            ${GLM_INCLUDE_DIR}
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
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "utility/InternalLoggers.hpp" EXCLUDE
        PATTERN "utility/Math.hpp" EXCLUDE
        PATTERN "utility/Memory.hpp" EXCLUDE
)

if (UNIX)
    set_target_properties(
        ${PROJECT_NAME}

        PROPERTIES

        POSITION_INDEPENDENT_CODE ON
    )
endif()

MakeSolutionFilters("${UTILITY_ALL_SOURCES}")
