/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/system/Manager.hpp>
#include <unicorn/utility/templates/Singleton.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Device;

/**
 * @brief Vulkan API context which creates vk::Instance once
 */
class Context    
{
public:
    /**
     * @brief Check if context was initialized
     * @return true if initialized and false if not
     */
    static bool IsInitialized();

    /**
     * @brief Creates Vulkan Context
     * @param manager System Manager which Vulkan can obtain needed extensions
     * @return true if initialized correctly and false if not
     */
    static bool Initialize(system::Manager& manager);
    /**
     * @brief Destructs context data
     */
    static void Deinitialize();
    /**
     * @brief Getter for raw Vulkan Instance
     * @return copy of vk::Instance
     */
    static vk::Instance GetVkInstance();
    /*
     * @brief Getter for validation layers
     */
    static const std::vector<const char*>& GetValidationLayers();
    /*
    * @brief Getter for device extensions
    */
    static const std::vector<const char*>& GetDeviceExtensions();
    /*
    * @brief Getter for instance extensions
    */
    static const std::vector<const char*>& GetInstanceExtensions();
private:
    Context(const Context& other) = delete;
    Context& operator=(const Context& other) = delete;
    Context(Context&& other) = delete;
    Context& operator=(Context&& other) = delete;
    ~Context() = delete;

    /**
     * @brief Checks for validation layers support
     * @return true if they all are found, false if not
     */
    static bool CheckValidationLayerSupport();
    /**
     * @brief Obtains all required extensions
     * @param manager System manager which produce it
     * @return All required extensions
     */
    static std::vector<const char*> FillRequiredExtensions(system::Manager& manager);
    /**
     * @brief Creates debug report callback
     * @param pCreateInfo filled create info
     * @return VkResult true if was created successfully and false if not
     */
    static VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
    /**
     * @brief Destroys callback
     */
    static void DestroyDebugReportCallbackEXT();
    /**
     * @brief Fills all needed create info and calls CreateDebugReportCallbackEXT()
     */
    static void SetupDebugCallback();
    /**
     * @brief Destroys callback preliminarily check if it exists, calls DestroyDebugReportCallbackEXT()
     */
    static void FreeDebugCallback();
    /**
    * @brief Validation layers for Vulkan API
    */
    static std::vector<const char*> m_validationLayers;
    /**
    * @brief Extensions for Vulkan logical device
    */
    static std::vector<const char*> m_deviceExtensions;
    /**
    * @brief Extensions for Vulkan Instance
    */
    static std::vector<const char*> m_instanceExtensions;

    static vk::Instance m_vkInstance;
    static VkDebugReportCallbackEXT m_vulkanCallback;
    #ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
    #else
    static const bool s_enableValidationLayers = true;
    #endif
};
}
}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP
