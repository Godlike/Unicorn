/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_RENDERER_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/vulkan/VkMesh.hpp>

namespace unicorn
{
	namespace system
	{
		class Manager;
		class Window;
	}

	namespace video
	{
		namespace vulkan
		{
			struct QueueFamilyIndices
			{
				int graphicsFamily = -1;
				int presentFamily = -1;

				bool IsComplete() const
				{
					return graphicsFamily >= 0 && presentFamily >= 0;
				}
			};

			struct SwapChainSupportDetails
			{
				vk::SurfaceCapabilitiesKHR capabilities;
				std::vector<vk::SurfaceFormatKHR> formats;
				std::vector<vk::PresentModeKHR> presentModes;
			};

			class ShaderProgram;

			class Renderer : public video::Renderer
			{
			public:
				Renderer(system::Manager& manager, system::Window* window);

				~Renderer() override;

				Renderer(const Renderer& other) = delete;
				Renderer(const Renderer&& other) = delete;
				Renderer& operator=(const Renderer& other) = delete;
				Renderer& operator=(const Renderer&& other) = delete;

				bool Init() override;
				void Deinit() override;
				bool Render() override;
				bool RecreateSwapChain();
				std::shared_ptr<geometry::Mesh> SpawnMesh() override;
			private:
				vk::PhysicalDevice m_vkPhysicalDevice;
				vk::Device m_vkLogicalDevice;
				vk::SwapchainKHR m_vkSwapChain;
				vk::Queue m_graphicsQueue;
				vk::Queue m_presentQueue;
				vk::SurfaceKHR m_vkWindowSurface;
				vk::Format m_swapChainImageFormat;
				vk::Extent2D m_swapChainExtent;
				vk::PipelineLayout m_pipelineLayout;
				vk::Pipeline m_graphicsPipeline;
				vk::RenderPass m_renderPass;
				vk::CommandPool m_commandPool;
				vk::Semaphore m_imageAvailableSemaphore;
				vk::Semaphore m_renderFinishedSemaphore;
				ShaderProgram* m_shaderProgram;
				std::string m_gpuName;
				std::vector<vk::Image> m_swapChainImages;
				std::vector<vk::ImageView> m_swapChainImageViews;
				std::vector<vk::Framebuffer> m_swapChainFramebuffers;
				std::vector<vk::CommandBuffer> m_commandBuffers;
                std::vector<VkMesh*> m_vkMeshes;
#ifdef NDEBUG
				static const bool s_enableValidationLayers = false;
#else
				static const bool s_enableValidationLayers = true;
#endif
				void FreeSurface();
				void FreeLogicalDevice();
				void FreeSwapChain();
				void FreeImageViews();
				void FreeRenderPass();
				void FreeGraphicsPipeline();
				void FreeFrameBuffers();
				void FreeCommandPool();
				void FreeCommandBuffers();
				void FreeSemaphores();

				bool PickPhysicalDevice();
				bool CreateLogicalDevice();
				bool CreateSurface();
				bool CreateSwapChain();
				bool CreateImageViews();
				bool CreateRenderPass();
				bool CreateGraphicsPipeline();
				bool CreateFramebuffers();
				bool CreateCommandPool();
				bool CreateCommandBuffers();
				bool CreateSemaphores();
				bool IsDeviceSuitable(const vk::PhysicalDevice& device);
				bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device);
				bool Frame();
				QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device);
				bool QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device);
				vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
				vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
				vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
				// Callbacks for window events
				void OnWindowDestroyed(system::Window* pWindow) override;
				void OnWindowSizeChanged(system::Window* pWindow, std::pair<int32_t, int32_t> size) override;
			};
		}
	}
}
#endif // UNICORN_VIDEO_VULKAN_VULKAN_RENDERER_HPP
