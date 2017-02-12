/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace uc
{
namespace graphics
{
class Vertex;

/** 
 * @brief  Vulkan renderer class
 */
class VulkanRenderer
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

    VulkanRenderer(const VulkanRenderer& other) = delete;
    VulkanRenderer(const VulkanRenderer&& other) = delete;
    VulkanRenderer& operator=(const VulkanRenderer& other) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&& other) = delete;

    bool Init();
    void Deinit();
    void Render();  
};
}
}

#endif // VORPAL_VIDEO_RENDERER_HPP
