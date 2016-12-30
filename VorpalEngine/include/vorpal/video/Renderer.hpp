// 73s studio

#ifndef VORPAL_RENDERER_HPP
#define VORPAL_RENDERER_HPP

#include <vorpal/core/VkDeleter.hpp>

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

            void init();
            void deinit();
            void update();
        private:
            GLFWwindow* _pWindow;
          //VkDeleter<VkInstance> _vkInstance{vkDestroyInstance};
        };
    }    
}

#endif // VORPAL_RENDERER_HPP
