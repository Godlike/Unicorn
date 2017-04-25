/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/UniformObject.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            UniformObject::UniformObject(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue) : 
            m_device(device),
            m_physicalDevice(physicalDevice),
            m_pool(pool),
            m_queue(queue)
            {
            }

            UniformObject::~UniformObject()
            {
            }

            bool UniformObject::Create()
            {
                vk::DescriptorSetLayoutBinding uboLayoutBinding;
                uboLayoutBinding.setBinding(0);
                uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
                uboLayoutBinding.descriptorCount = 1;
                uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
                uboLayoutBinding.pImmutableSamplers = nullptr;

                vk::DescriptorSetLayoutCreateInfo layoutInfo;
                layoutInfo.bindingCount = 1;
                layoutInfo.pBindings = &uboLayoutBinding;

                vk::Result result = m_device.createDescriptorSetLayout(&layoutInfo, nullptr, &m_descriptorSetLayout);
                if (result != vk::Result::eSuccess)
                {
                    LOG_ERROR("Failed to create descriptor set layout for camera!");
                    return false;
                }

                vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
                
                m_uniformStagingBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, bufferSize);
                m_uniformBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, bufferSize);
                return true;
            }

            void UniformObject::Destroy()
            {
                if (m_descriptorSetLayout)
                {
                    m_device.destroyDescriptorSetLayout(m_descriptorSetLayout);
                }
            }

            vk::DescriptorSetLayout UniformObject::GetDescriptorLayout() const
            {
                return m_descriptorSetLayout;
            }

            void UniformObject::Update(float deltaTime, const UniformBufferObject& ubo)
            {
                m_uniformStagingBuffer.Write(&ubo);
                m_uniformStagingBuffer.CopyToBuffer(m_pool, m_queue, m_uniformBuffer, sizeof(ubo));
            }
        }
    }
}
