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
    static bool IsInitialized();
    VulkanDevice* GetNewDevice();
    bool DestroyDevice(VulkanDevice* device);
    static void Destroy();

    bool Initialize();
    static VulkanInstance& Instance(system::Manager& manager);
    static VulkanInstance& Instance();
    VulkanInstance(system::Manager& manager);
    const vk::Instance GetRawInstance() const;
    const std::vector<const char*> GetRequiredExtensions() const;
    const std::vector<const char*> GetValidationLayers() const;
private:
    //friend class utility::templates::Singleton<VulkanInstance>;

    VulkanInstance(const VulkanInstance& other) = delete;
    VulkanInstance& operator=(const VulkanInstance& other) = delete;
    VulkanInstance(VulkanInstance&& other) = delete;
    VulkanInstance& operator=(VulkanInstance&& other) = delete;
    ~VulkanInstance() = delete;

    bool CheckValidationLayerSupport();
    std::vector<const char*> FillRequiredExtensions();
    static VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
    static void DestroyDebugReportCallbackEXT();
    static void SetupDebugCallback();
    static void FreeDebugCallback();

    system::Manager& m_systemManager;
    static vk::Instance m_vkInstance;
    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_deviceExtensions;
    std::vector<const char*> m_instanceExtensions;
    static VulkanInstance* s_instance;
    static VkDebugReportCallbackEXT m_vulkanCallback;
#ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
#else
    static const bool s_enableValidationLayers = true;
#endif
};
}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_INSTANCE_HPP
