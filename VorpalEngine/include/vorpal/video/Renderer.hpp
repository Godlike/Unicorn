#ifndef VORPAL_VIDEO_RENDERER_HPP
#define VORPAL_VIDEO_RENDERER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>

struct GLFWwindow;

namespace vp
{
    namespace video
    {

        struct QueueFamilyIndices {
            int graphicsFamily = -1;    // -1 mean not found.

            bool isComplete() {
                return graphicsFamily >= 0;
            }
        };
        /** @brief  Vulkan renderer class
         *
         *  Initializes and controls Vulkan API
         */
        class Renderer
        {
        public:
            Renderer();
            ~Renderer();

            Renderer(const Renderer &other) = delete;
            Renderer(const Renderer &&other) = delete;
            Renderer &operator=(const Renderer &other) = delete;
            Renderer &operator=(const Renderer &&other) = delete;

            bool Init();
            void Deinit();
            void Render();

        private:
            bool m_isInitialized;
            GLFWwindow *m_pWindow;

            VkInstance m_vkInstance;
            VkPhysicalDevice m_vkPhysicalDevice;
            std::vector<const char*> m_validationLayers;
            std::vector<const char*> m_extensions;
            VkDebugReportCallbackEXT m_vulkanCallback;

        #ifdef NDEBUG
            static const bool s_enableValidationLayers = false;
        #else
            static const bool s_enableValidationLayers = true;
        #endif

            bool CreateInstance();
            bool CheckValidationLayerSupport() const;
            bool PickPhysicalDevice();
            bool IsDeviceSuitable(VkPhysicalDevice device);            
            std::vector<const char*> GetRequiredExtensions();
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

            bool SetupDebugCallback();            
            void DestroyDebugReportCallbackEXT();
            VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
        };
    }
}

#endif // VORPAL_VIDEO_RENDERER_HPP
