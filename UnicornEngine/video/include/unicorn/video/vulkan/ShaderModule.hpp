/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_SHADER_MODULE_HPP
#define UNICORN_VIDEO_VULKAN_SHADER_MODULE_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class ShaderModule
            {
            public:
                ShaderModule();
                ~ShaderModule();

                vk::Result Create(vk::Device device, size_t size, void* code);
                void Destroy();

                vk::ShaderModule& GetVkShaderModule();
            private:
                vk::Device m_device;
                vk::ShaderModule m_shaderModule;
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_SHADER_MODULE_HPP