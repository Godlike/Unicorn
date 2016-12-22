#include <vorpal/VorpalEngine.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vp
{

VorpalEngine::VorpalEngine() : m_window(nullptr)
{

}

VorpalEngine::~VorpalEngine()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
}

void VorpalEngine::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(640, 480, "Vulkan", nullptr, nullptr);
}

void VorpalEngine::run()
{
    while ( !glfwWindowShouldClose(m_window) )
    {
        glfwPollEvents();
    }
}

}
