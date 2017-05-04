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

class CommandBuffers
{
public:
	CommandBuffers();
	~CommandBuffers();

	vk::Result Create(vk::Device device, size_t count);
	void Destroy() const;

	vk::CommandPool& GetVkCommandPool();
	vk::CommandBuffer& GetVkCommandBuffer(size_t index);
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