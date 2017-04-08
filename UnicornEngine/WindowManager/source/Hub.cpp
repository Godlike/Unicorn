/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/Hub.hpp>

#include <unicorn/window_manager/adapters/Helper.hpp>

#include <unicorn/window_manager/CustomValue.hpp>
#include <unicorn/window_manager/MonitorMemento.hpp>
#include <unicorn/window_manager/WindowHint.hpp>

#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace WindowManager
{

Hub::Hub()
{

}

Hub::~Hub()
{
    Deinit();
}

Window* Hub::CreateWindow(int32_t width,
    int32_t height,
    const std::string& name,
    Monitor* pMonitor,
    Window* pSharedWindow)
{
    Window* result = new Window(m_windows.size(), width, height, name, pMonitor, pSharedWindow);

    std::pair<int32_t, int32_t> position;
    WINDOW_MANAGER_ADAPTER::GetWindowPosition(result->GetHandle(), &position.first, &position.second);
    result->SetPosition(position);

    m_windows.insert(std::make_pair(result->GetId(), result));

    WindowCreated.emit(result);

    return result;
}

VkResult Hub::CreateVulkanSurfaceForWindow(const Window& window,
    VkInstance instance,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return WINDOW_MANAGER_ADAPTER::CreateVulkanSurface(instance, window.GetHandle(), allocator, surface);
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

    WINDOW_MANAGER_ADAPTER::SetWindowHint(WindowHint::ClientAPI, CustomValue::No_API);
    WINDOW_MANAGER_ADAPTER::SetWindowHint(WindowHint::Resizable, CustomValue::True);

    WINDOW_MANAGER_ADAPTER::MonitorStateChanged.connect(this, &Hub::OnMonitorStateChanged);

    const std::vector<MonitorMemento>& monitors = WINDOW_MANAGER_ADAPTER::GetMonitors();

    m_monitors.reserve(monitors.size());

    Monitor* pMonitor = nullptr;

    for (std::vector<MonitorMemento>::const_iterator cit = monitors.cbegin(); cit != monitors.cend(); ++cit)
    {
        pMonitor = new Monitor(*cit);

        m_monitors.push_back(pMonitor);

        MonitorCreated.push(pMonitor);
    }

    MonitorCreated.emit();
}

void Hub::Deinit()
{
    WindowCreated.clear();

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

Monitor* Hub::GetMonitor(uint32_t id) const
{
    Monitor* result = nullptr;

    for (std::vector<Monitor*>::const_iterator cit = m_monitors.cbegin(); cit != m_monitors.cend(); ++cit)
    {
        if (id == (*cit)->GetId())
        {
            result = *cit;
            break;
        }
    }

    return result;
}

void Hub::OnMonitorStateChanged(void* handle, MonitorMemento::State state)
{
    Monitor* pMonitor = GetMonitor(handle);

    if (nullptr == pMonitor)
    {
        pMonitor = new Monitor(WINDOW_MANAGER_ADAPTER::GetMonitor(handle));
        pMonitor->SetState(state);

        m_monitors.push_back(pMonitor);

        MonitorCreated.push(pMonitor);
    }

    MonitorCreated.emit();
}

Monitor* Hub::GetMonitor(void* handle) const
{
    Monitor* result = nullptr;

    for (std::vector<Monitor*>::const_iterator cit = m_monitors.cbegin(); cit != m_monitors.cend(); ++cit)
    {
        if (handle == (*cit)->GetHandle())
        {
            result = *cit;
            break;
        }
    }

    return result;
}

Monitor* Hub::GetWindowMonitor(const Window& window) const
{
    return GetMonitor(WINDOW_MANAGER_ADAPTER::GetWindowMonitor(window.GetHandle()));
}

void Hub::SetWindowMonitor(const Window& window,
    Monitor* pMonitor,
    std::pair<int32_t, int32_t> position,
    std::pair<int32_t, int32_t> size,
    int32_t refreshRate) const
{
    WINDOW_MANAGER_ADAPTER::SetWindowMonitor(window.GetHandle(),
        pMonitor != nullptr ? pMonitor->GetHandle() : nullptr,
        position,
        size,
        refreshRate
    );
}

}
}