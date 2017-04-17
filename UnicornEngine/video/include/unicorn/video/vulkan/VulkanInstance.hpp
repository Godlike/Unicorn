/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP

#include <vulkan/vulkan.hpp>

#include <unicorn/utility/templates/Singleton.hpp>
#include <unicorn/system/Manager.hpp>

namespace unicorn
{
namespace video
{
class VulkanDevice;

class VulkanInstance //: public utility::templates::Singleton<VulkanInstance>
{
public:
    bool IsInitialized() const;
    VulkanDevice* GetNewDevice();
    bool DestroyDevice(VulkanDevice* device);
    void Destroy();

    bool Initialize();
    static VulkanInstance& Instance(system::Manager& manager);
    VulkanInstance(system::Manager& manager)
        : m_systemManager(manager){};

private:
    //friend class utility::templates::Singleton<VulkanInstance>;

    VulkanInstance(const VulkanInstance& other) = delete;
    VulkanInstance& operator=(const VulkanInstance& other) = delete;
    VulkanInstance(VulkanInstance&& other) = delete;
    VulkanInstance& operator=(VulkanInstance&& other) = delete;
    ~VulkanInstance() = delete;

    bool CheckValidationLayerSupport() const;
    std::vector<const char*> GetRequiredExtensions();

    system::Manager& m_systemManager;
    vk::Instance m_vkInstance;
    std::vector<const char*> m_validationLayers;
    static VulkanInstance* s_instance;
#ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
#else
    static const bool s_enableValidationLayers = true;
#endif
};
}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP
