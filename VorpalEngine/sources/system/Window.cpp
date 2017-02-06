#include <vorpal/system/Window.hpp>
#include <vorpal/system/Settings.hpp>
#include <vorpal/graphics/Graphics.hpp>

vp::system::Window::Window()
    : m_pWindow(nullptr), m_Graphics(nullptr)
{
}

bool vp::system::Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    const auto& settings = system::Settings::Instance();

    m_pWindow = glfwCreateWindow(static_cast<int>(settings.GetApplicationWidth()),
        static_cast<int>(settings.GetApplicationHeight()),
        settings.GetApplicationName().c_str(),
        nullptr,
        nullptr);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowSizeCallback(m_pWindow, onWindowResized);
    return true;
}

void vp::system::Window::Deinit()
{
    if (m_pWindow)
    {
        glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
}

void vp::system::Window::SetGraphicsEngine(vp::graphics::Graphics* graphicsEngine)
{
    m_Graphics = graphicsEngine;
}

void vp::system::Window::RetrieveEvents() const
{
    glfwPollEvents();
}

void vp::system::Window::onWindowResized(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0)
        return;
    Window* wnd =
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wnd->m_Graphics->WindowSizeChanged();
}

bool vp::system::Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_pWindow);
}

void vp::system::Window::Close()
{
    glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
}
