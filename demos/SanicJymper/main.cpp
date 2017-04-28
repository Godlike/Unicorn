/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/Camera.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/system/WindowHint.hpp>
#include <unicorn/system/CustomValue.hpp>
#include <unicorn/Settings.hpp>
#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Gamepad.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/geometry/Triangle.hpp>
#include <unicorn/video/Renderer.hpp>
#include <cmath>
#include <iostream>

static unicorn::video::Graphics* pGraphics = nullptr;
static unicorn::video::Camera* pCamera = nullptr;
unicorn::system::Window* pWindow0;
void onWindowSizeChange(unicorn::system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    std::cout << "Window[" << pWindow->GetId() << "]: size changed to " << size.first << "x" << size.second << std::endl;
}


int nbFrames = 0;
double elapsedTime = 0;
double fps = 0;

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    static unicorn::system::Timer timer(true);
    nbFrames++;
    double elapsedTime = timer.ElapsedMilliseconds().count();
    if( elapsedTime >= 1000) {
        fps = nbFrames * 1000.0 / double(elapsedTime);
        nbFrames = 0;
        timer.Start();
    }    
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

    float delta = 0.1;

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
     
        case Key::W:
        {
            pCamera->Translate({0.0f, 0.0f, -delta });
            break;
        }
        case Key::S:
        {
            pCamera->Translate({ 0.0f, 0.0f, delta});
            break;
        }
        case Key::A:
        {
            pCamera->Translate({ -delta, 0.0f, 0.0f });
            break;
        }
        case Key::D:
        {
            pCamera->Translate({ delta, 0.0f, 0.0f });
            break;
        }
        case Key::Q:
        {
            pCamera->Translate({ 0.0f, -delta, 0.0f });
            break;
        }
        case Key::E:
        {
            pCamera->Translate({ 0.0f, delta, 0.0f });
            break;
        }
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

    uint32_t size = static_cast<std::uint32_t>(axes.size());

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
            position.first += static_cast<std::int32_t>(round(speed * x));
        }

        if (fabs(y) > deadZone)
        {
            position.second += static_cast<std::int32_t>(round(speed * y));
        }
    }

    if (oldX != position.first || oldY != position.second)
    {
        pWindow->SetPosition(position);
    }
}

int main(int argc, char* argv[])
{
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init(argc, argv, "Sanic_Jymper.log");
    settings.SetApplicationName("SANIC JYMPER");
    
    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();
    if (unicornEngine->Init())
    {
        pGraphics = unicornEngine->GetGraphics();

        for (auto const& cit : unicornEngine->GetGamepads())
        {
            cit.second->Updated.connect(&onGamepadUpdate);
        }

        unicornEngine->LogicFrame.connect(&onLogicFrame);

        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::True);

        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
            unicorn::system::CustomValue::True);

        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr);

        auto vkRenderer0 = pGraphics->SpawnVulkanRenderer(pWindow0);
        pCamera = vkRenderer0->GetCamera();
        unicorn::video::geometry::Triangle triangle0(vkRenderer0->SpawnMesh());
        triangle0.SetColor(unicorn::video::Color::Red);
        triangle0.Move({-1.5f, 0.0, 0.0});
        unicorn::video::geometry::Triangle triangle2(vkRenderer0->SpawnMesh());
        triangle2.SetColor(unicorn::video::Color::Green);
        triangle2.Move({ -1.3f, 0.0, -1.0 });
        unicorn::video::geometry::Triangle triangle1(vkRenderer0->SpawnMesh());
        triangle1.SetColor(unicorn::video::Color::Blue);
        triangle1.Move({ 1.5f, 0.0, 0.0 });

        pWindow0->SizeChanged.connect(&onWindowSizeChange);
        pWindow0->Keyboard.connect(&onWindowKeyboard);
        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
    return 0;
}
