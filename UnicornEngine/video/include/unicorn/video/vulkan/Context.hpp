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

			class Context
			{
			public:
				static bool IsInitialized();
				static Device* GetNewDevice();
				static bool DestroyDevice(Device* device);
				static void Destroy();

				static bool Initialize(system::Manager& manager);
				static  vk::Instance GetVkInstance();
				static std::vector<const char*> validationLayers, deviceExtensions, instanceExtensions;
			private:
				Context(const Context& other) = delete;
				Context& operator=(const Context& other) = delete;
				Context(Context&& other) = delete;
				Context& operator=(Context&& other) = delete;
				~Context() = delete;

				static bool CheckValidationLayerSupport();
				static std::vector<const char*> FillRequiredExtensions(system::Manager& manager);
				static VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
				static void DestroyDebugReportCallbackEXT();
				static void SetupDebugCallback();
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
