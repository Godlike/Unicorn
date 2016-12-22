set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Flag to override default GLFW_BUILD_EXAMPLES value")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Flag to override default GLFW_BUILD_TESTS value")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Flag to override default GLFW_BUILD_DOCS value")

set(GLFW_INSTALL OFF CACHE BOOL "Flag to override default GLFW_INSTALL value")

set(GLFW_VULKAN_STATIC OFF CACHE BOOL "Flag to override default GLFW_VULKAN_STATIC value")

set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "Flag to override default GLFW_DOCUMENT_INTERNALS value")

set(GLFW_ROOT_DIR "${VORPAL_ENGINE_ROOT}/external/glfw" CACHE STRING "Path to GLFW root directory")
set(GLFW_LIB "glfw" CACHE STRING "Name of GLFW library")
set(GLFW_INCLUDE_DIR
    "${GLFW_ROOT_DIR}/include"
    "${GLFW_ROOT_DIR}/deps"

    CACHE LIST "List of GLFW include directories")
