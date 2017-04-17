/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>

#include <unicorn/system/Window.hpp>
#include <unicorn/system/WindowHint.hpp>
#include <unicorn/system/CustomValue.hpp>

#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Gamepad.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>

#include <unicorn/core/Settings.hpp>

#include <cmath>
#include <iostream>

static unicorn::video::Graphics* pGraphics = nullptr;

void onWindowSizeChange(unicorn::system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    std::cout << "Window[" << pWindow->GetId() << "]: size changed to " << size.first << "x" << size.second << std::endl;
}

void onWindowKeyboard(unicorn::system::Window* pWindow, unicorn::system::input::Key key, uint32_t scancode, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask modifiers)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;

    if (Action::Release == action)
    {
        return;
    }

    std::pair<int32_t, int32_t> position = pWindow->GetPosition();
    bool positionChanged = true;

    uint32_t delta = 1;

    if (Modifier::Shift & modifiers)
    {
        delta *= 10;
    }

    if (Modifier::Alt & modifiers)
    {
        delta *= 5;
    }

    switch (key)
    {
        case Key::Up:
        {
            position.second -= delta;
            break;
        }

        case Key::Down:
        {
            position.second += delta;
            break;
        }
        case Key::Left:
        {
            position.first -= delta;
            break;
        }
        case Key::Right:
        {
            position.first += delta;
            break;
        }
        default:
        {
            positionChanged = false;
            break;
        }
    }

    if (positionChanged)
    {
        pWindow->SetPosition(position);
    }
}

void onGamepadUpdate(unicorn::system::input::Gamepad* pGamepad)
{
    if (!pGraphics)
    {
        return;
    }

    unicorn::system::Window* pWindow = pGraphics->GetFocusedWindow();

    if (!pWindow)
    {
        return;
    }

    static const float speed = 5.0f;
    static const float deadZone = 0.05f;

    const std::vector<float>& axes = pGamepad->GetAxes();

    uint32_t size = axes.size();

    if (size % 2 != 0)
    {
        --size;
    }

    std::pair<int32_t, int32_t> position = pWindow->GetPosition();
    int32_t oldX = position.first;
    int32_t oldY = position.second;

    for (uint32_t i = 0; i < size / 2; i += 2) // checking only sticks (at least for xbox)
    {
        const float x = axes[i];
        const float y = axes[i + 1];

        if (fabs(x) > deadZone)
        {
            position.first += round(speed * x);
        }

        if (fabs(y) > deadZone)
        {
            position.second += round(speed * y);
        }
    }

    if (oldX != position.first || oldY != position.second)
    {
        pWindow->SetPosition(position);
    }
}

int main(int argc, char* argv[])
{
    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();

    if (unicornEngine->Init())
    {
        pGraphics = unicornEngine->GetGraphics();

        const std::map<uint32_t, unicorn::system::input::Gamepad*>& gamepads = unicornEngine->GetGamepads();

        for (auto cit = gamepads.cbegin(); cit != gamepads.cend(); ++cit)
        {
            cit->second->Updated.connect(&onGamepadUpdate);
        }

        // Borderless undecorated
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::False);

        // Resizable
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
            unicorn::system::CustomValue::True);

        const std::vector<unicorn::system::Monitor*>& monitors = pGraphics->GetMonitors();
        unicorn::system::Monitor* lastMonitor = monitors.back();
        unicorn::system::VideoMode activeMode = lastMonitor->GetActiveVideoMode();

        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicorn::system::Window* pWindow1 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("Hmm ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        pWindow1->Keyboard.connect(&onWindowKeyboard);

        // Decorated, with borders
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::True);

        unicorn::system::Window* pWindow2 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("wat ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        pWindow2->SizeChanged.connect(&onWindowSizeChange);
        pWindow2->Keyboard.connect(&onWindowKeyboard);

        pWindow0->Minimize();

        std::pair<int32_t, int32_t> win2pos = pWindow1->GetPosition();
        win2pos.first += 640;
        win2pos.second += 480;
        pWindow1->SetPosition(win2pos);
        pWindow1->SetName("Ayy lmao");

        pWindow2->Focus();

        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::core::Settings::Destroy();
    return 0;
}
