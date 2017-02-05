/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vorpal/utility/Logger.hpp>

#include <vector>

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
