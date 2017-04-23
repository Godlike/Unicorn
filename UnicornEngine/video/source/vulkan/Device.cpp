/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Device.hpp>

namespace unicorn
{
namespace video
{
    namespace vulkan
    {
        Device::Device() {}
        Device::~Device() {}

        vk::Result Device::Create(vk::Instance instance, bool validate)
        {
            uint32_t physDevicesNum = 0;
            vk::Result result = instance.enumeratePhysicalDevices(&physDevicesNum, nullptr);
            if (result != vk::Result::eSuccess)
            {
                return result;
            }
            m_physicalDevices.resize(physDevicesNum);
            result = instance.enumeratePhysicalDevices(
                &physDevicesNum,
                m_physicalDevices.data());

            if (result != vk::Result::eSuccess)
            {
                return result;
            }

            m_devices.resize(m_physicalDevices.size());
            for (size_t deviceIndex = 0; deviceIndex<m_devices.size(); ++deviceIndex)
            {

                uint32_t queueFamilyPropertiesCount;
                m_physicalDevices[deviceIndex].getQueueFamilyProperties(&queueFamilyPropertiesCount, nullptr);
                std::vector<vk::QueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
                m_physicalDevices[deviceIndex].getQueueFamilyProperties(&queueFamilyPropertiesCount, queueFamilyProperties.data());

                uint32_t familyIndex = 0;
                for (int i = 0; i < queueFamilyProperties.size(); ++i)
                {
                    if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
                    {
                        familyIndex = i;
                        break;
                    }
                }
                vk::DeviceQueueCreateInfo queueInfo;
                float queueProperties{ 0.0f };
                queueInfo.setQueueFamilyIndex(familyIndex); //TODO: check here not initialized familyIndex
                queueInfo.setQueueCount(1);
                queueInfo.setPQueuePriorities(&queueProperties);

                vk::DeviceCreateInfo deviceInfo;
                deviceInfo.setQueueCreateInfoCount(1);
                deviceInfo.setPQueueCreateInfos(&queueInfo);

                std::vector<const char*> layers;
                std::vector<const char*> extensions;

                extensions.push_back("VK_KHR_swapchain");
                deviceInfo.setEnabledExtensionCount(extensions.size());
                deviceInfo.setPpEnabledExtensionNames(extensions.data());


                if (validate)
                {
                    layers.push_back("VK_LAYER_LUNARG_standard_validation");
                }

                deviceInfo.setEnabledLayerCount(layers.size());
                deviceInfo.setPpEnabledLayerNames(layers.data());

                result = m_physicalDevices[deviceIndex].createDevice(&deviceInfo, nullptr, &m_devices[deviceIndex]);
            }
            return result;
        }

        void Device::Destroy()
        {
            for (vk::Device& device : m_devices)
            {
                device.destroy(nullptr);
            }
        }

        vk::PhysicalDevice& Device::GetVkPhysicalDevice(size_t index)
        {
            return m_physicalDevices[index];
        }
        vk::Device& Device::GetVkDevice(size_t index)
        {
            return m_devices[index];
        }
    }
}
}
