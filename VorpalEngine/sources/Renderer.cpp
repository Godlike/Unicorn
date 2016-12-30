// 73s studio

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vorpal/video/Renderer.hpp>


void vp::video::Renderer::deinit()
{
    if (_pWindow)
    {
        glfwDestroyWindow(_pWindow);
    }
}



void vp::video::Renderer::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _pWindow = glfwCreateWindow(640, 480, "Vulkan", nullptr, nullptr);
}

void vp::video::Renderer::update()
{
    while ( !glfwWindowShouldClose(_pWindow) )
    {
        glfwPollEvents();
    }
}

vp::video::Renderer::Renderer()  : _pWindow(nullptr)
{

}
