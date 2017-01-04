#ifndef VORPAL_RENDERER_HPP
#define VORPAL_RENDERER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace vp
{
    namespace video
    {
        /**
         * @brief The Renderer class abstract low level Vulkan initialization and updating.
         */
        class Renderer {
        public:
            Renderer();
            Renderer(const Renderer& other) = delete;
            Renderer(const Renderer&& other) = delete;
            Renderer& operator=(const Renderer& other) = delete;
            Renderer& operator=(const Renderer&& other) = delete;

            bool init();
            void deinit();
            void render();
        private:
            GLFWwindow* m_pWindow;
            VkInstance m_vkInstance;

            bool m_createInstance();
        };
    }    
}

#endif // VORPAL_RENDERER_HPP
