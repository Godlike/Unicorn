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

			    vk::Result Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, size_t size);
				void Destroy() const;
				void Write(const void* pData) const;

				size_t GetSize() const;
				vk::BufferUsageFlags GetUsage();

				vk::Buffer& GetVkBuffer();
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