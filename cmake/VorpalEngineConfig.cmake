set(VORPAL_ENGINE_NAME "VorpalEngine" CACHE STRING "Project name for VorpalEngine library")

if (NOT DEFINED VORPAL_ENGINE_ROOT)
	set(VORPAL_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/VorpalEngine" CACHE STRING "Path to VorpalEngine root directory")
endif()

set(VORPAL_ENGINE_INCLUDE_DIR "${VORPAL_ENGINE_ROOT}/include" CACHE STRING "Path to VorpalEngine include directory")
set(VORPAL_ENGINE_LIB ${VORPAL_ENGINE_NAME} CACHE STRING "Name of VorpalEngine library")
