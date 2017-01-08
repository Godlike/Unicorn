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
            std::vector<const char*> m_validationLayers;
            VkDebugReportCallbackEXT m_vulkanCallback;

        #ifdef NDEBUG
            static const bool s_enableValidationLayers = false;
        #else
            static const bool s_enableValidationLayers = true;
        #endif

            bool CreateInstance();
            bool CheckValidationLayerSupport() const;
            static std::vector<const char*> GetRequiredExtensions();

            bool SetupDebugCallback();
            static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT objType,
                uint64_t obj,
                size_t location,
                int32_t code,
                const char* layerPrefix,
                const char* msg,
                void* userData);
            void DestroyDebugReportCallbackEXT();
        };
    }
}

#endif // VORPAL_VIDEO_RENDERER_HPP
