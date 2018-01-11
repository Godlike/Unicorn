# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

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

add_library(video OBJECT ${VIDEO_ALL_SOURCES})

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

foreach(_source IN ITEMS ${VIDEO_ALL_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()
