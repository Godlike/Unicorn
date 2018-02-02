/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/system/Manager.hpp>
#include <mule/templates/Singleton.hpp>

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
class Context : public mule::templates::Singleton<Context>
{
public:
    Context();

    /**
     * @brief Check if context was initialized
     * @return true if initialized and false if not
     */
    bool IsInitialized();

    /**
     * @brief Creates Vulkan Context
     * @param manager system manager which Vulkan can obtain needed extensions
     * @return true if initialized correctly and false if not
     */
    bool Initialize(system::Manager& manager);

    /**
     * @brief Destructs context data
     */
    void Deinitialize();

    /**
     * @brief Returns raw Vulkan Instance
     * @return copy of vk::Instance
     */
    vk::Instance GetVkInstance();

    /*
     * @brief Returns validation layers
     */
    std::vector<char const*> const& GetValidationLayers();

    /*
    * @brief Returns device extensions
    */
    std::vector<char const*> const& GetDeviceExtensions();

    /*
    * @brief Returns instance extensions
    */
    std::vector<char const*> const& GetInstanceExtensions();
private:
    friend class mule::templates::Singleton<Context>;

    Context(Context const& other) = delete;
    Context& operator=(Context const& other) = delete;
    Context(Context&& other) = delete;
    Context& operator=(Context&& other) = delete;
    ~Context() = delete;

    /**
     * @brief Checks for validation layers support
     * @return true if they all are found, false if not
     */
    bool CheckValidationLayerSupport();
    /**
     * @brief Obtains all required extensions
     * @param manager system manager which produce it
     * @return All required extensions
     */
    std::vector<char const*> FillRequiredExtensions(system::Manager& manager);
    /**
     * @brief Creates debug report callback
     * @param pCreateInfo filled create info
     * @return VkResult true if was created successfully and false if not
     */
    VkResult CreateDebugReportCallbackEXT(VkDebugReportCallbackCreateInfoEXT const* pCreateInfo);
    /**
     * @brief Destroys callback
     */
    void DestroyDebugReportCallbackEXT();
    /**
     * @brief Fills all needed create info and calls CreateDebugReportCallbackEXT()
     */
    void SetupDebugCallback();
    /**
     * @brief Destroys callback preliminarily check if it exists, calls DestroyDebugReportCallbackEXT()
     */
    void FreeDebugCallback();
    /**
    * @brief Validation layers for Vulkan API
    */
    std::vector<char const*> m_validationLayers;
    /**
    * @brief Extensions for Vulkan logical device
    */
    std::vector<char const*> m_deviceExtensions;
    /**
    * @brief Extensions for Vulkan Instance
    */
    std::vector<char const*> m_instanceExtensions;

    vk::Instance m_vkInstance;
    VkDebugReportCallbackEXT m_vulkanCallback;
    static bool const s_enableValidationLayers;
};
}
}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_CONTEXT_HPP
