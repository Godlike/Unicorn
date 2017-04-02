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

Window::Window(uint32_t id, uint32_t width, uint32_t height,
    const std::string& name, Monitor* pMonitor, Window* pSharedWindow)
    : m_id( id )
    , m_width( width )
    , m_height( height )
    , m_name( name )
    , m_pMonitor( pMonitor )
    , m_pSharedWindow( pSharedWindow )
    , m_handle( nullptr )
{
    m_handle = WINDOW_MANAGER_ADAPTER::CreateWindow(m_width
        , m_height
        , m_name.c_str()
        , m_pMonitor ? m_pMonitor->GetHandle() : nullptr
        , m_pSharedWindow ? m_pSharedWindow->GetHandle() : nullptr
    );
}

Window::~Window()
{
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

}
}
