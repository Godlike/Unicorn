/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_BUFFER_HPP
#define UNICORN_VIDEO_VULKAN_BUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
	namespace video
	{
		namespace vulkan
		{
			class Buffer
			{
			public:
				Buffer();
				~Buffer();

				Result create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, size_t size);
				void destroy();
				void write(const void* pData);

				size_t getSize();
				vk::BufferUsageFlags getUsage();

				vk::Buffer& getVkBuffer();
			private:
				vk::Device m_device;
				vk::BufferUsageFlags m_usage;
				size_t m_size;
				vk::Buffer m_buffer;
				vk::DeviceMemory m_memory;
			};
		}
	}
}

#endif // UNICORN_VIDEO_VULKAN_BUFFER_HPP