/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/graphics/vulkan/VulkanUtils.hpp>
#include <unicorn/utility/Logger.hpp>

#include <vector>

namespace uc
{
bool CheckValidationLayerSupport(const std::vector<const char*>& requiredLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : requiredLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            LOG_ERROR("Can't find required Vulkan layers: ");

            for (auto& requiredLayer : requiredLayers)
            {
                LOG_ERROR("%s", requiredLayer);
            }

            return false;
        }
    }

    return true;
}

void WaitVulkanDeviceIdle(const VkDevice& device)
{
    vkDeviceWaitIdle(device);
}

}
