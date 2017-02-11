/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/


#ifndef VORPAL_GRAPHICS_VULKAN_CONTEXT_HPP
#define VORPAL_GRAPHICS_VULKAN_CONTEXT_HPP

namespace vp
{
    namespace graphics
    {
        class VulkanContext
        {
        public:
            VulkanContext();
            ~VulkanContext();

            VulkanContext(const VulkanContext& other) = delete;
            VulkanContext(const VulkanContext&& other) = delete;
            VulkanContext& operator=(const VulkanContext& other) = delete;
            VulkanContext& operator=(const VulkanContext&& other) = delete;

            bool CreateTexture();
            bool Init();

        };
    }
}

#endif // VORPAL_GRAPHICS_VULKAN_CONTEXT_HPP