/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Window.hpp>
#include <unicorn/system/Settings.hpp>
#include <unicorn/graphics/Graphics.hpp>

uc::system::Window::Window()
    : m_pWindow(nullptr), m_Graphics(nullptr)
{
}

uc::system::Window::~Window()
{
    Deinit();
}

bool uc::system::Window::Init()
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

void uc::system::Window::Deinit()
{
    if (m_pWindow)
    {
        glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
}

void uc::system::Window::RetrieveEvents() const
{
    glfwPollEvents();
}

void uc::system::Window::onWindowResized(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0)
        return;
    Window* wnd =
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wnd->m_Graphics->WindowSizeChanged();
}

bool uc::system::Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_pWindow);
}

void uc::system::Window::Close()
{
    glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
}


GLFWwindow* uc::system::Window::GetWindowPointer() const
{
    return m_pWindow;
}
