# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(UnicornRender_Video)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} OBJECT "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Unicorn::Render::Video ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(VULKAN_HEADERS
    include/unicorn/video/vulkan/Context.hpp
    include/unicorn/video/vulkan/Renderer.hpp
    include/unicorn/video/vulkan/Buffer.hpp
    include/unicorn/video/vulkan/CommandBuffers.hpp
    include/unicorn/video/vulkan/ShaderProgram.hpp
    include/unicorn/video/vulkan/VkMesh.hpp
    include/unicorn/video/vulkan/VkTexture.hpp
    include/unicorn/video/vulkan/Image.hpp
    include/unicorn/video/vulkan/Memory.hpp
    include/unicorn/video/vulkan/VulkanHelper.hpp
    include/unicorn/video/vulkan/VkMaterial.hpp
)

set(VULKAN_SOURCES
    source/vulkan/Context.cpp
    source/vulkan/Renderer.cpp
    source/vulkan/Buffer.cpp
    source/vulkan/CommandBuffers.cpp
    source/vulkan/ShaderProgram.cpp
    source/vulkan/VkMesh.cpp
    source/vulkan/VkTexture.cpp
    source/vulkan/Image.cpp
    source/vulkan/Memory.cpp
    source/vulkan/VulkanHelper.cpp
)

set(VIDEO_HEADERS
    include/unicorn/video/Graphics.hpp
    include/unicorn/video/Renderer.hpp
    include/unicorn/video/Camera.hpp
    include/unicorn/video/Camera2DController.hpp
    include/unicorn/video/CameraProjection.hpp
    include/unicorn/video/OrthographicCamera.hpp
    include/unicorn/video/PerspectiveCamera.hpp
    include/unicorn/video/CameraFpsController.hpp
    include/unicorn/video/Color.hpp
    include/unicorn/video/Texture.hpp
    include/unicorn/video/Mesh.hpp
    include/unicorn/video/Material.hpp
    include/unicorn/video/Primitives.hpp
    include/unicorn/video/Transform.hpp
)

set(VIDEO_SOURCES
    source/Graphics.cpp
    source/Renderer.cpp
    source/Camera2DController.cpp
    source/CameraProjection.cpp
    source/OrthographicCamera.cpp
    source/PerspectiveCamera.cpp
    source/CameraFpsController.cpp
    source/Color.cpp
    source/Texture.cpp
    source/Mesh.cpp
    source/Material.cpp
    source/Primitives.cpp
    source/Transform.cpp
)

set(VIDEO_ALL_SOURCES
    ${VIDEO_HEADERS}
    ${VIDEO_SOURCES}
    ${VULKAN_HEADERS}
    ${VULKAN_SOURCES}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${VIDEO_ALL_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${GLM_INCLUDE_DIR}

            $<TARGET_PROPERTY:Mule::Utilities,INTERFACE_INCLUDE_DIRECTORIES>

            $<TARGET_PROPERTY:Unicorn::Render::System,INTERFACE_INCLUDE_DIRECTORIES>

        PRIVATE
            ${STB_INCLUDE_DIR}

            $<TARGET_PROPERTY:Unicorn::Render::Utility,INTERFACE_INCLUDE_DIRECTORIES>
)

if (WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES FOLDER ${UNICORN_IDE_FOLDER}
    )
endif()

include(InstallInfo)

install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${UNICORN_INSTALL_INCLUDE_DIR}"
    COMPONENT unicorn_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "video/vulkan" EXCLUDE
)

if (UNIX)
    set_target_properties(
        ${PROJECT_NAME}

        PROPERTIES

        POSITION_INDEPENDENT_CODE ON
    )
endif()

MakeSolutionFilters("${VIDEO_ALL_SOURCES}")
