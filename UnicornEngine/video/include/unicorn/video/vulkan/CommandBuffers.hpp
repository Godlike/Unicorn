/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_COMMAND_BUFFERS_HPP
#define UNICORN_VIDEO_VULKAN_BUFFER_HPP

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

				Result create(vk::Device device, size_t count);
				void destroy();

				vk::CommandPool& getVkCommandPool();
				vk::CommandBuffer& getVkCommandBuffer(size_t index);
				const std::vector<vk::CommandBuffer>& getVkCommandBuffers();
			private:
				vk::Device m_device;
				vk::CommandPool m_cmdPool;
				std::vector<vk::CommandBuffer> m_cmdBuffers;
			};
		}
	}
}