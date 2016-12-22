#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vp
{
    class VorpalEngine {
        public:
        void init() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        }
        void run() 
        {
            while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
        }
    };
}