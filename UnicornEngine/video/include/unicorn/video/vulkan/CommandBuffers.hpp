/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_COMMAND_BUFFERS_HPP
#define UNICORN_VIDEO_VULKAN_COMMAND_BUFFERS_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
/**
 * @brief Wrapper for raw command buffers of Vulkan API
 */
class CommandBuffers
{
public:
    /**
     * @brief Default constructor
     */
    CommandBuffers();
    /**
     * @brief Destructor which calls Destroy()
     */
    ~CommandBuffers();

    /**
     * @brief Creates new command buffers
     * @param device device to allocate from
     * @param count command buffer size
     * @return true if was allocated successfully and false if not
     */
    vk::Result Create(vk::Device device, uint32_t count);
    /**
     * @brief Destroys all data
     */
    void Destroy() const;

    /**
     * @brief Returns command pool
     * @return reference to vk::CommandPool
     */
    vk::CommandPool& GetVkCommandPool();
    /**
     * @brief Returns command buffer
     * @param index Index of needed command buffer
     * @return reference for needed command buffer
     */
    vk::CommandBuffer& GetVkCommandBuffer(uint32_t index);
    /**
     * @brief Returns all command buffers
     * @return vector of all command buffer
     */
    const std::vector<vk::CommandBuffer>& GetVkCommandBuffers();
private:
    vk::Device m_device;
    vk::CommandPool m_cmdPool;
    std::vector<vk::CommandBuffer> m_cmdBuffers;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_COMMAND_BUFFERS_HPP
