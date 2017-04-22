/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_DESCRIPTOR_POOL_HPP
#define UNICORN_VIDEO_VULKAN_DESCRIPTOR_POOL_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
	namespace video
	{
		namespace vulkan
		{
			class DescriptorSets
			{
			public:
				DescriptorSets();
				~DescriptorSets();

			    vk::Result Create(
					vk::Device device,
					const std::vector<vk::DescriptorSetLayoutBinding>& descriptorBindings,
					uint32_t setsCount);
				void Destroy();

				vk::DescriptorSet& GetVkDescriptorSet(size_t index);
				vk::DescriptorSetLayout& GetVkDescriptorSetLayout();
			private:
				vk::Device m_device;
				vk::DescriptorPool m_descriptorPool;
				vk::DescriptorSetLayout m_descriptorSetLayout;
				std::vector<vk::DescriptorSet> m_descriptorSets;
			};
		}
	}
}

#endif // UNICORN_VIDEO_VULKAN_DESCRIPTOR_POOL_HPP