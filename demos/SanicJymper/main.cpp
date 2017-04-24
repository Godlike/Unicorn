/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>
#include <unicorn/system/Input.hpp>

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

static unicorn::video::Graphics* s_pGraphics = nullptr;
static unicorn::system::Input* s_pInput = nullptr;

void onWindowSizeChange(unicorn::system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    std::cout << "Window[" << pWindow->GetId() << "]: size changed to " << size.first << "x" << size.second << std::endl;
}

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    std::cout << "Logic frame" << std::endl;
}

void onWindowKeyboard(unicorn::system::Window* pWindow, unicorn::system::input::Key key, uint32_t scancode, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask modifiers)
{
    using unicorn::system::CursorShape;
    using unicorn::system::MouseMode;
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;

    switch (action)
    {
        case Action::Release:
        {
            switch (key)
            {
                case Key::A:
                {
                    pWindow->SetCursorShape(CursorShape::Arrow);

                    break;
                }
                case Key::C:
                {
                    if (Modifier::Alt & modifiers)
                    {
                        pWindow->SetMouseMode(MouseMode::Captured);
                    }
                    else
                    {
                        pWindow->SetCursorShape(CursorShape::Crosshair);
                    }

                    break;
                }
                case Key::H:
                {
                    if (Modifier::Shift & modifiers)
                    {
                        pWindow->SetCursorShape(CursorShape::HorizontalResize);
                    }
                    else if (Modifier::Alt & modifiers)
                    {
                        pWindow->SetMouseMode(MouseMode::Hidden);
                    }
                    else
                    {
                        pWindow->SetCursorShape(CursorShape::Hand);
                    }

                    break;
                }
                case Key::N:
                {
                    if (Modifier::Alt & modifiers)
                    {
                        pWindow->SetMouseMode(MouseMode::Normal);
                    }

                    break;
                }
                case Key::R:
                {
                    pWindow->SetCursorShape(CursorShape::Reset);

                    break;
                }
                case Key::T:
                {
                    pWindow->SetCursorShape(CursorShape::TextBeam);

                    break;
                }
                case Key::V:
                {
                    if (Modifier::Shift & modifiers)
                    {
                        pWindow->SetCursorShape(CursorShape::VerticalResize);
                    }

                    break;
                }

                case Key::Escape:
                {
                    pWindow->SetCursorShape(CursorShape::Reset);
                    pWindow->SetMouseMode(MouseMode::Normal);

                    break;
                }

                default:
                {
                    break;
                }
            }

            break;
        }

        default:
        {
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

            break;
        }
    }
}

void onGamepadUpdate(unicorn::system::input::Gamepad* pGamepad)
{
    if (!s_pGraphics)
    {
        return;
    }

    unicorn::system::Window* pWindow = s_pGraphics->GetFocusedWindow();

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

void onGamepadCreated(unicorn::system::input::Gamepad* const& pGamepad)
{
    pGamepad->Updated.connect(&onGamepadUpdate);
}

int main(int argc, char* argv[])
{
    using unicorn::system::CustomValue;
    using unicorn::system::Monitor;
    using unicorn::system::Window;
    using unicorn::system::WindowHint;

    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();

    if (unicornEngine->Init())
    {
        s_pGraphics = unicornEngine->GetGraphics();
        s_pInput = unicornEngine->GetInput();

        s_pInput->GamepadCreated.connect(&onGamepadCreated);

        for (auto const& cit : s_pInput->GetGamepads())
        {
            cit.second->Updated.connect(&onGamepadUpdate);
        }

        unicornEngine->LogicFrame.connect(&onLogicFrame);

        // Borderless undecorated
        s_pGraphics->SetWindowCreationHint(WindowHint::Decorated,
            CustomValue::False);

        // Resizable
        s_pGraphics->SetWindowCreationHint(WindowHint::Resizable,
            CustomValue::True);

        auto const& monitors = s_pGraphics->GetMonitors();
        Monitor* lastMonitor = monitors.back();
        auto activeMode = lastMonitor->GetActiveVideoMode();

        Window* pWindow0 = s_pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr );

        Window* pWindow1 = s_pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("Hmm ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        pWindow1->Keyboard.connect(&onWindowKeyboard);

        // Decorated, with borders
        s_pGraphics->SetWindowCreationHint(WindowHint::Decorated,
            CustomValue::True);

        Window* pWindow2 = s_pGraphics->SpawnWindow(
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
