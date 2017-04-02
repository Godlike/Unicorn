/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/Window.hpp>

#include <unicorn/window_manager/adapters/Helper.hpp>

namespace unicorn
{
namespace WindowManager
{

Window::Window(uint32_t id, int32_t width, int32_t height,
    const std::string& name, Monitor* pMonitor, Window* pSharedWindow)
    : m_id( id )
    , m_size( width, height )
    , m_position( 0, 0 )
    , m_name( name )
    , m_pMonitor( pMonitor )
    , m_pSharedWindow( pSharedWindow )
    , m_handle( nullptr )
{
    m_handle = WINDOW_MANAGER_ADAPTER::CreateWindow(m_size.first
        , m_size.second
        , m_name.c_str()
        , m_pMonitor ? m_pMonitor->GetHandle() : nullptr
        , m_pSharedWindow ? m_pSharedWindow->GetHandle() : nullptr
    );

    WINDOW_MANAGER_ADAPTER::GetWindowPosition(m_handle, &m_position.first, &m_position.second);

    WINDOW_MANAGER_ADAPTER::WindowPositionChanged.connect(this, &Window::OnWindowPositionChanged);
    WINDOW_MANAGER_ADAPTER::WindowSizeChanged.connect(this, &Window::OnWindowSizeChanged);
    WINDOW_MANAGER_ADAPTER::WindowClose.connect(this, &Window::OnWindowClose);
    WINDOW_MANAGER_ADAPTER::WindowContentRefresh.connect(this, &Window::OnWindowContentRefresh);
    WINDOW_MANAGER_ADAPTER::WindowFocused.connect(this, &Window::OnWindowFocused);
    WINDOW_MANAGER_ADAPTER::WindowMinimized.connect(this, &Window::OnWindowMinimized);
    WINDOW_MANAGER_ADAPTER::WindowMaximized.connect(this, &Window::OnWindowMaximized);
    WINDOW_MANAGER_ADAPTER::WindowFramebufferResized.connect(this, &Window::OnWindowFramebufferResized);
}

Window::~Window()
{
    Destroyed.emit(this);

    Destroyed.clear();
    PositionChanged.clear();
    SizeChanged.clear();
    Close.clear();
    ContentRefresh.clear();
    Focused.clear();
    Minimized.clear();
    Maximized.clear();
    FramebufferResized.clear();

    WINDOW_MANAGER_ADAPTER::DestroyWindow(m_handle);
}

VkResult Window::CreateVulkanSurface(VkInstance instance,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return WINDOW_MANAGER_ADAPTER::CreateVulkanSurface(instance, m_handle, allocator, surface);
}

bool Window::ShouldClose() const
{
    return WINDOW_MANAGER_ADAPTER::WindowShouldClose(m_handle);
}

void Window::SetShouldClose(bool flag) const
{
    WINDOW_MANAGER_ADAPTER::SetWindowShouldClose(m_handle, flag);
}

void Window::OnWindowPositionChanged(void* handle, std::pair<int32_t, int32_t> position)
{
    if (handle == m_handle)
    {
        m_position = position;

        PositionChanged.emit(this, m_position);
    }
}

void Window::OnWindowSizeChanged(void* handle, std::pair<int32_t, int32_t> size)
{
    if (handle == m_handle)
    {
        m_size = size;

        SizeChanged.emit(this, m_size);
    }
}

void Window::OnWindowClose(void* handle)
{
    if (handle == m_handle)
    {
        Close.emit(this);
    }
}

void Window::OnWindowContentRefresh(void* handle)
{
    if (handle == m_handle)
    {
        ContentRefresh.emit(this);
    }
}

void Window::OnWindowFocused(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        Focused.emit(this, flag);
    }
}

void Window::OnWindowMinimized(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        Minimized.emit(this, flag);
    }
}

void Window::OnWindowMaximized(void* handle, bool flag)
{
    if (handle == m_handle)
    {
        Maximized.emit(this, flag);
    }
}

void Window::OnWindowFramebufferResized(void* handle, std::pair<int32_t, int32_t> size)
{
    if (handle == m_handle)
    {
        FramebufferResized.emit(this, size);
    }
}

}
}
