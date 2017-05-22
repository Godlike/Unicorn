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
/**
 * @brief Small wrapper for vk::ShaderModule
 */
class ShaderModule
{
public:
    ShaderModule();
    ~ShaderModule();

    /**
     * @brief Creating and allocating shader module
     * @param device Which device to use
     * @param size Size of shader
     * @param code SPIR-V compiled code
     * @return true if created successful and false if not
     */
    vk::Result Create(vk::Device device, size_t size, void* code);
    /**
     * @brief Free GPU acquired memory
     */
    void Destroy();

    /**
     * @brief Getter for Vulkan shader module
     * @return Reference to vk::ShaderModule
     */
    vk::ShaderModule& GetVkShaderModule();
private:
    vk::Device m_device;
    vk::ShaderModule m_shaderModule;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_SHADER_MODULE_HPP
