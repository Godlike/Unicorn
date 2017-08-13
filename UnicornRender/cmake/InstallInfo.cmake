# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

if (NOT DEFINED UNICORN_RENDER_NAME)
    set(UNICORN_RENDER_NAME "UnicornRender" CACHE STRING "Project name for Unicorn Render library")
endif()

if (NOT DEFINED INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include/godlike" CACHE STRING "Path to directory holding headers")
endif()

if (NOT DEFINED INSTALL_LIBRARY_DIR)
    set(INSTALL_LIBRARY_DIR "lib" CACHE STRING "Path to directory holding libraries")
endif()

if (NOT DEFINED INSTALL_DATA_DIR)
    set(INSTALL_DATA_DIR "share" CACHE STRING "Path to directory holding additional data")
endif()

set(UNICORN_INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})
set(UNICORN_INSTALL_LIBRARY_DIR ${INSTALL_LIBRARY_DIR}/${UNICORN_RENDER_NAME})
set(UNICORN_INSTALL_DATA_DIR ${INSTALL_DATA_DIR}/${UNICORN_RENDER_NAME})

