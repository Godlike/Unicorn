/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/DescriptorSets.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            DescriptorSets::DescriptorSets()
            {
            }
            DescriptorSets::~DescriptorSets()
            {
            }

            vk::Result DescriptorSets::Create(
                vk::Device device,
                const std::vector<vk::DescriptorSetLayoutBinding>& descriptorBindings,
                uint32_t setsCount)
            {
                m_device = device;

                vk::DescriptorSetLayoutCreateInfo layoutInfo;
                layoutInfo.setBindingCount(descriptorBindings.size());
                layoutInfo.setPBindings(descriptorBindings.data());

                vk::Result result = m_device.createDescriptorSetLayout(&layoutInfo, nullptr, &m_descriptorSetLayout);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                std::vector<vk::DescriptorPoolSize> poolSizes(descriptorBindings.size());
                for (size_t i = 0; i<poolSizes.size(); ++i)
                {
                    poolSizes[i].setType(descriptorBindings[i].descriptorType);
                    poolSizes[i].setDescriptorCount(descriptorBindings[i].descriptorCount);
                }

                vk::DescriptorPoolCreateInfo poolInfo;
                poolInfo.setPoolSizeCount(poolSizes.size());
                poolInfo.setPPoolSizes(poolSizes.data());
                poolInfo.setMaxSets(setsCount);

                result = m_device.createDescriptorPool(&poolInfo, nullptr, &m_descriptorPool);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                m_descriptorSets.resize(setsCount);

                vk::DescriptorSetAllocateInfo setInfo;
                setInfo.setDescriptorPool(m_descriptorPool);
                setInfo.setDescriptorSetCount(setsCount);
                setInfo.setPSetLayouts(&m_descriptorSetLayout);

                result = m_device.allocateDescriptorSets(&setInfo, m_descriptorSets.data());
                return result;
            }
            vk::DescriptorSet& DescriptorSets::GetVkDescriptorSet(size_t index)
            {
                return m_descriptorSets.at(index);
            }
            vk::DescriptorSetLayout& DescriptorSets::GetVkDescriptorSetLayout()
            {
                return m_descriptorSetLayout;
            }
        }
    }
}