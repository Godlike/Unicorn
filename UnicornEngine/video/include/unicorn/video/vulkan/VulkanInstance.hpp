/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP

#include <vulkan/vulkan.hpp>

#include <unicorn/utility/templates/Singleton.hpp>

namespace unicorn
{
namespace video
{
class VulkanDevice;

namespace system
{
class Manager;
}

class VulkanInstance : public utility::templates::Singleton<VulkanInstance>
{
    VulkanInstance();
    ~VulkanInstance();

    bool InstanceInitialised() const;

    VulkanDevice* GetNewDevice();
    bool DestroyDevice(VulkanDevice* device);

private:
    friend class utility::templates::Singleton<VulkanInstance>;

    VulkanInstance(const VulkanInstance& other) = delete;
    VulkanInstance& operator=(const VulkanInstance& other) = delete;

    VulkanInstance(VulkanInstance&& other) = delete;
    VulkanInstance& operator=(VulkanInstance&& other) = delete;

    bool CreateInstance();
    void DestroyInstance();

    bool CheckValidationLayerSupport() const;
    std::vector<const char*> GetRequiredExtensions();

    system::Manager& m_systemManager;
    vk::Instance m_vkInstance;
    std::vector<const char*> m_validationLayers;
#ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
#else
    static const bool s_enableValidationLayers = true;
#endif
};
}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP
