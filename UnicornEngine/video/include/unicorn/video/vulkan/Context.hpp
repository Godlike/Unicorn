/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/system/Manager.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Device;

/**
 * @brief Vulkan API context which create vk::Instance once.
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
     * @brief Create Vulkan Context
     * @param manager System Manager which Vulkan can obtain needed extensions
     * @return true if initialized correctly and false if not
     */
    static bool Initialize(system::Manager& manager);
    /**
     * @brief Destruct context data
     */
    static void Deinitialize();
    /**
     * @brief Getter for raw Vulkan Instance
     * @return copy of vk::Instance
     */
    static vk::Instance GetVkInstance();
    /**
     * @brief Validation layers for Vulkan API
     */
    static std::vector<const char*> validationLayers;
    /**
     * @brief Extensions for Vulkan logical device
     */
    static std::vector<const char*> deviceExtensions;
    /**
     * @brief Extensions for Vulkan Instance
     */
    static std::vector<const char*> instanceExtensions;

private:
    Context(const Context& other) = delete;
    Context& operator=(const Context& other) = delete;
    Context(Context&& other) = delete;
    Context& operator=(Context&& other) = delete;
    ~Context() = delete;

    /**
     * @brief Check all required validation layers support in operation system. Needed only for debug.
     * @return true if them all found and available, false if not
     */
    static bool CheckValidationLayerSupport();
    /**
     * @brief Obtain all required extensions
     * @param manager System manager which will give it to us
     * @return All required extensions
     */
    static std::vector<const char*> FillRequiredExtensions(system::Manager& manager);
    /**
     * @brief Creating debug report callback for address in driver DLL
     * @param pCreateInfo filled create info
     * @return VkResult true if was created successfully and false if not
     */
    static VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
    /**
     * @brief Destroy callback
     */
    static void DestroyDebugReportCallbackEXT();
    /**
     * @brief Fill all needed create info and calls @sa CreateDebugReportCallbackEXT
     */
    static void SetupDebugCallback();
    /**
     * @brief Destroy callback preliminarily check if it exists, calls @sa DestroyDebugReportCallbackEXT
     */
    static void FreeDebugCallback();

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
