/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_DEVICE_HPP
#define UNICORN_VIDEO_VULKAN_DEVICE_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Device
{
public:
    Device();
    ~Device();

    vk::Result Create(vk::Instance instance, bool validate);
    void Destroy();

    vk::PhysicalDevice& GetVkPhysicalDevice(size_t index);
    vk::Device& GetVkDevice(size_t index);
private:
    std::vector<vk::PhysicalDevice> m_physicalDevices;
    std::vector<vk::Device> m_devices;
};
}
}
}
#endif // UNICORN_VIDEO_VULKAN_DEVICE_HPP

