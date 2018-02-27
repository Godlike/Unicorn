# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(MULE_BUILD_TESTS OFF CACHE BOOL "Flag to override default MULE_BUILD_TESTS value")

if (NOT DEFINED MULE_ROOT)
    set(MULE_ROOT "${UNICORN_RENDER_ROOT}/godlike/Mule" CACHE STRING "Path to Mule root directory")
endif()
