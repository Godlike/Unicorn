# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

# This file initializes selected window manager

set(SUPPORTED_WINDOW_MANAGERS
    "glfw")

message(STATUS "Supported window managers: ${SUPPORTED_WINDOW_MANAGERS}")

if (NOT DEFINED UNICORN_WINDOW_MANAGER)
    set(UNICORN_WINDOW_MANAGER "glfw" CACHE STRING "Window manager subsystem for UnicornEngine")
    message(STATUS "No window manager specified. Using default value: ${UNICORN_WINDOW_MANAGER}.")
else()
    message(STATUS "Using ${UNICORN_WINDOW_MANAGER} window manager")
endif()

list(FIND
    SUPPORTED_WINDOW_MANAGERS
    "${UNICORN_WINDOW_MANAGER}"
    SUPPORTED_WINDOW_MANAGERS_INDEX
)

set(WINDOW_MANAGER_INCLUDE_DIR
    "${UNICORN_ENGINE_ROOT}/WindowManager/include"
)

set(WINDOW_MANAGER_LIB
    WindowManager
)

if (${SUPPORTED_WINDOW_MANAGERS_INDEX} GREATER -1)

    ## GLFW
    if (UNICORN_WINDOW_MANAGER STREQUAL "glfw")
        # GLFW shall always be statically linked
        include(GlfwConfig)

        # Force BUILD_SHARED_LIBS to be OFF while saving initial value to temp variable
        set(TMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force GLFW to build static library" FORCE)

        add_subdirectory(${GLFW_ROOT_DIR})

        # Restore initial BUILD_SHARED_LIBS value
        set(BUILD_SHARED_LIBS ${TMP_BUILD_SHARED_LIBS} CACHE BOOL "Restored initial BUILD_SHARED_LIBS value" FORCE)

        set(WINDOW_MANAGER_ADAPTER_INCLUDE_DIR
            ${GLFW_INCLUDE_DIR}
        )

        set(WINDOW_MANAGER_LIB
            ${WINDOW_MANAGER_LIB}
            ${GLFW_LIB}
        )
    endif()

    add_definitions(-DUNICORN_WINDOW_MANAGER=${UNICORN_WINDOW_MANAGER})

else()
    message(FATAL_ERROR "Unknown window manager: ${UNICORN_WINDOW_MANAGER}. Supported values: ${SUPPORTED_WINDOW_MANAGERS}")
endif()
