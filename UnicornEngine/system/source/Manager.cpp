/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Manager.hpp>

#include <unicorn/system/adapter/Helper.hpp>

#include <unicorn/system/CustomValue.hpp>
#include <unicorn/system/Monitor.hpp>
#include <unicorn/system/MonitorMemento.hpp>
#include <unicorn/system/Window.hpp>

#include <unicorn/system/input/Gamepad.hpp>

#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace system
{

Manager::Manager()
{

}

Manager::~Manager()
{
    Deinit();
}

Window* Manager::CreateWindow(int32_t width,
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

VkResult Manager::CreateVulkanSurfaceForWindow(const Window& window,
    VkInstance instance,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return WINDOW_MANAGER_ADAPTER::CreateVulkanSurface(instance, window.GetHandle(), allocator, surface);
}

Window* Manager::GetWindow(uint32_t id) const
{
    std::map<uint32_t, Window*>::const_iterator cit = m_windows.find(id);

    return cit != m_windows.cend() ? cit->second : nullptr;
}

bool Manager::DestroyWindow(uint32_t id)
{
    return DestroyWindow(GetWindow(id));
}

bool Manager::DestroyWindow(Window* pWindow)
{
    if (pWindow && m_windows.erase(pWindow->GetId()))
    {
        delete pWindow;

        return true;
    }

    return false;
}

void Manager::Init()
{
    WINDOW_MANAGER_ADAPTER::Init();

    //! @todo   Make this a part of Vulkan initialization
    SetWindowCreationHint(WindowHint::ClientAPI, CustomValue::No_API);

    WINDOW_MANAGER_ADAPTER::MonitorStateChanged.connect(this, &Manager::OnMonitorStateChanged);

    {
        std::vector<MonitorMemento> monitors = WINDOW_MANAGER_ADAPTER::GetMonitors();

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

    GAMEPAD_ADAPTER::GamepadStateChanged.connect(this, &Manager::OnGamepadStateChanged);

    {
        std::vector<void*> gamepads = GAMEPAD_ADAPTER::GetConnectedGamepads();

        input::Gamepad* pGamepad = nullptr;

        for (std::vector<void*>::const_iterator cit = gamepads.cbegin(); cit != gamepads.cend(); ++cit)
        {
            pGamepad = new input::Gamepad(*cit);
            m_gamepads.insert( std::pair<uint32_t, input::Gamepad*>(pGamepad->GetId(), pGamepad) );

            GamepadCreated.push(pGamepad);
        }

        GamepadCreated.emit();
    }
}

void Manager::Deinit()
{
    WindowCreated.clear();

    for (std::map<uint32_t, Window*>::const_iterator cit = m_windows.cbegin(); cit != m_windows.cend(); ++cit)
    {
        delete cit->second;
    }

    m_windows.clear();

    WINDOW_MANAGER_ADAPTER::Deinit();
}

void Manager::PollEvents() const
{
    WINDOW_MANAGER_ADAPTER::PollEvents();
}

bool Manager::IsVulkanSupported() const
{
    return WINDOW_MANAGER_ADAPTER::IsVulkanSupported();
}

std::vector<const char*> Manager::GetRequiredVulkanExtensions() const
{
    return WINDOW_MANAGER_ADAPTER::GetRequiredVulkanExtensions();
}

Monitor* Manager::GetMonitor(uint32_t id) const
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

void Manager::OnMonitorStateChanged(void* handle, MonitorMemento::State state)
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

void Manager::OnGamepadStateChanged(void* handle, input::GamepadState state)
{
    uint32_t id = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    std::map<uint32_t, input::Gamepad*>::const_iterator cit = m_gamepads.find(id);

    switch (state)
    {
        case input::GamepadState::Connected:
        {
            if (cit == m_gamepads.cend())
            {
                input::Gamepad* pGamepad = new input::Gamepad(handle);

                m_gamepads.insert(std::pair<uint32_t, input::Gamepad*>(pGamepad->GetId(), pGamepad));

                GamepadCreated.push(pGamepad);

                GamepadCreated.emit();
            }
            break;
        }
        case input::GamepadState::Disconnected:
        {
            if(cit != m_gamepads.cend())
            {
                delete cit->second;
                m_gamepads.erase(cit);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

Monitor* Manager::GetMonitor(void* handle) const
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

Monitor* Manager::GetWindowMonitor(const Window& window) const
{
    return GetMonitor(WINDOW_MANAGER_ADAPTER::GetWindowMonitor(window.GetHandle()));
}

void Manager::SetWindowMonitor(const Window& window,
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

void Manager::SetWindowCreationHint(WindowHint hint, int32_t value) const
{
    WINDOW_MANAGER_ADAPTER::SetWindowCreationHint(hint, value);
}

}
}
