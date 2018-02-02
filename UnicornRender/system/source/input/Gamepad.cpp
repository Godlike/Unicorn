/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/input/Gamepad.hpp>

#include <unicorn/system/adapter/Helper.hpp>

#include <mule/Logger.hpp>

namespace unicorn
{
namespace system
{
namespace input
{

Gamepad::Gamepad(void* handle)
    : m_id( static_cast<uint32_t>( reinterpret_cast<uintptr_t>(handle) ) )
    , m_handle( handle )
{
    m_name = GAMEPAD_ADAPTER::GetGamepadName(m_handle);
    UpdateData();
}

Gamepad::~Gamepad()
{
    Destroyed.emit(this);
}

void Gamepad::UpdateData()
{
    bool anyUpdates = false;

    auto iterateAndUpdate = [&](auto& dest, auto& source)
    {
        for (uint32_t i = 0; i < source.size(); ++i)
        {
            if (!anyUpdates && dest[i] != source[i])
            {
                anyUpdates = true;
            }

            dest[i] = source[i];
        }
    };

    {
        std::vector<float> axes = GAMEPAD_ADAPTER::GetGamepadAxes(m_handle);

        if (!m_axes.empty() && axes.size() != m_axes.size())
        {
            LOG_WARNING("Gamepad[%d]: number of axes changed during runtime", m_id);
        }

        m_axes.resize(axes.size());

        iterateAndUpdate(m_axes, axes);
    }

    {
        std::vector<bool> buttons = GAMEPAD_ADAPTER::GetGamepadButtons(m_handle);

        if (!m_buttons.empty() && buttons.size() != m_buttons.size())
        {
            LOG_WARNING("Gamepad[%d]: number of buttons changed during runtime", m_id);
        }

        m_buttons.resize(buttons.size());

        iterateAndUpdate(m_buttons, buttons);
    }

    if (anyUpdates)
    {
        Updated.emit(this);
    }
}

}
}
}
