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
     * 
     */
    CommandBuffers();
    /**
     * @brief Default destructor which calls @sa Destroy()
     */
    ~CommandBuffers();

    /**
     * @brief Creating new command buffers
     * @param device Which device you want use to allocation process
     * @param count Command buffer size
     * @return true if was allocated successfully and false if not
     */
    vk::Result Create(vk::Device device, uint32_t count);
    /**
     * @brief Destroys all data
     */
    void Destroy() const;

    /**
     * @brief Getter for passed command pool
     * @return reference to vk::CommandPool
     */
    vk::CommandPool& GetVkCommandPool();
    /**
     * @brief Getter for command buffer
     * @param index Index of needed command buffer
     * @return Reference for needed command buffer
     */
    vk::CommandBuffer& GetVkCommandBuffer(uint32_t index);
    /**
     * @brief Getter for all command buffers
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
