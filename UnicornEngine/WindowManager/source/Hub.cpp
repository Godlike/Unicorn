/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/Hub.hpp>

#include <unicorn/window_manager/adapters/Helper.hpp>

namespace unicorn
{
namespace WindowManager
{

Hub::Hub()
{

}

Window* Hub::CreateWindow(uint32_t width,
    uint32_t height,
    const std::string& name,
    Monitor* pMonitor,
    Window* pSharedWindow)
{
    Window* result = new Window(m_windows.size(), width, height, name, pMonitor, pSharedWindow);

    m_windows.insert(std::make_pair(result->GetId(), result));

    return result;
}

Window* Hub::GetWindow(uint32_t id) const
{
    std::map<uint32_t, Window*>::const_iterator cit = m_windows.find(id);

    return cit != m_windows.cend() ? cit->second : nullptr;
}

bool Hub::DestroyWindow(uint32_t id)
{
    return DestroyWindow(GetWindow(id));
}

bool Hub::DestroyWindow(Window* pWindow)
{
    if (pWindow && m_windows.erase(pWindow->GetId()))
    {
        delete pWindow;

        return true;
    }

    return false;
}

void Hub::Init()
{
    WINDOW_MANAGER_ADAPTER::Init();
}

void Hub::Deinit()
{
    for (std::map<uint32_t, Window*>::const_iterator cit = m_windows.cbegin(); cit != m_windows.cend(); ++cit)
    {
        delete cit->second;
    }

    m_windows.clear();

    WINDOW_MANAGER_ADAPTER::Deinit();
}

void Hub::PollEvents() const
{
    WINDOW_MANAGER_ADAPTER::PollEvents();
}

bool Hub::IsVulkanSupported() const
{
    return WINDOW_MANAGER_ADAPTER::IsVulkanSupported();
}

std::vector<const char*> Hub::GetRequiredVulkanExtensions() const
{
    return WINDOW_MANAGER_ADAPTER::GetRequiredVulkanExtensions();
}

}
}
