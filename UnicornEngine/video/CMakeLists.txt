# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)


set(GEOMETRY_HEADERS
    include/unicorn/video/geometry/Cube.hpp
    include/unicorn/video/geometry/Mesh.hpp
    include/unicorn/video/geometry/Quad.hpp
    include/unicorn/video/geometry/Triangle.hpp    
    include/unicorn/video/geometry/MeshDescriptor.hpp
)

set(GEOMETRY_SOURCES
    source/geometry/Cube.cpp
    source/geometry/Mesh.cpp
    source/geometry/Quad.cpp
    source/geometry/Triangle.cpp
    source/geometry/MeshDescriptor.cpp
)

set(VULKAN_HEADERS
    include/unicorn/video/vulkan/Context.hpp
    include/unicorn/video/vulkan/Renderer.hpp
    include/unicorn/video/vulkan/Buffer.hpp
    include/unicorn/video/vulkan/CommandBuffers.hpp
    include/unicorn/video/vulkan/ShaderModule.hpp
    include/unicorn/video/vulkan/ShaderProgram.hpp
    include/unicorn/video/vulkan/VkMesh.hpp
)

set(VULKAN_SOURCES
    source/vulkan/Context.cpp
    source/vulkan/Renderer.cpp
    source/vulkan/Buffer.cpp
    source/vulkan/CommandBuffers.cpp
    source/vulkan/ShaderModule.cpp
    source/vulkan/ShaderProgram.cpp
    source/vulkan/VkMesh.cpp
)

set(VIDEO_HEADERS
    include/unicorn/video/Graphics.hpp
    include/unicorn/video/Renderer.hpp
    include/unicorn/video/Camera.hpp
    include/unicorn/video/CameraController.hpp
    include/unicorn/video/CameraFpsController.hpp
    include/unicorn/video/Color.hpp
    ${VULKAN_HEADERS}
    ${GEOMETRY_HEADERS}
)

set(VIDEO_SOURCES
    source/Graphics.cpp
    source/Renderer.cpp
    source/Camera.cpp
    source/CameraController.cpp
    source/CameraFpsController.cpp
    source/Color.cpp
    ${VULKAN_SOURCES}
    ${GEOMETRY_SOURCES}
)

add_library(video OBJECT
    ${VIDEO_HEADERS}
    ${VIDEO_SOURCES}
)

if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
