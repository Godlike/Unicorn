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
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/geometry/Triangle.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/geometry/Quad.hpp>
#include <unicorn/video/geometry/Cube.hpp>

static unicorn::video::Graphics* pGraphics = nullptr;
static unicorn::video::Camera* pCamera = nullptr;
unicorn::system::Window* pWindow0;

int nbFrames = 0;
double elapsedTime = 0;
double fps = 0;

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    static unicorn::system::Timer timer(true);
    nbFrames++;
    double elapsedTime = timer.ElapsedMilliseconds().count();
    if (elapsedTime >= 1000)
    {
        fps = nbFrames * 1000.0 / double(elapsedTime);
        nbFrames = 0;
        timer.Reset();
    }
}

void onCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCamera->UpdateMouseView(pos.first, pos.second);
}

void onWindowKeyboard(unicorn::system::Window* pWindow, unicorn::system::input::Key key, uint32_t scancode, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask modifiers)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;
    using unicorn::system::MouseMode;

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
            pCamera->Translate({0.0f, 0.0f, -delta});
            break;
        }
        case Key::S:
        {
            pCamera->Translate({0.0f, 0.0f, delta});
            break;
        }
        case Key::A:
        {
            pCamera->Translate({-delta, 0.0f, 0.0f});
            break;
        }
        case Key::D:
        {
            pCamera->Translate({delta, 0.0f, 0.0f});
            break;
        }
        case Key::Q:
        {
            pCamera->Translate({0.0f, -delta, 0.0f});
            break;
        }
        case Key::E:
        {
            pCamera->Translate({0.0f, delta, 0.0f});
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
        case Key::C:
        {
            pWindow->SetMouseMode(MouseMode::Captured);
            break;
        }
        case Key::Escape:
        {
            pWindow->SetMouseMode(MouseMode::Normal);
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

int main(int argc, char* argv[])
{
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init(argc, argv, "Sanic_Jymper.log");
    settings.SetApplicationName("SANIC JYMPER");
    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();
    if (unicornEngine->Init())
    {
        pGraphics = unicornEngine->GetGraphics();

        unicornEngine->LogicFrame.connect(&onLogicFrame);

        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::True);

        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
            unicorn::system::CustomValue::True);

        auto h = pGraphics->GetMonitors().back()->GetActiveVideoMode().height;
        auto w = pGraphics->GetMonitors().back()->GetActiveVideoMode().width;
        settings.SetApplicationHeight(h);
        settings.SetApplicationWidth(w);
        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr);
        pWindow0->SetMouseMode(unicorn::system::MouseMode::Captured);
        auto vkRenderer0 = pGraphics->SpawnVulkanRenderer(pWindow0);
        pCamera = vkRenderer0->GetCamera();
        unicorn::video::geometry::Triangle triangle0(vkRenderer0->SpawnMesh());
        triangle0.SetColor(unicorn::video::Color::Red);
        triangle0.Move({-1.5f, 0.0f, 0.0f});
        unicorn::video::geometry::Triangle triangle2(vkRenderer0->SpawnMesh());
        triangle2.SetColor(unicorn::video::Color::Green);
        triangle2.Move({-1.3f, 0.0, -1.0});
        unicorn::video::geometry::Quad quad0(vkRenderer0->SpawnMesh());
        unicorn::video::geometry::Cube cube0(vkRenderer0->SpawnMesh());
        cube0.Move({10.0, 0.0, -2.0f});
        unicorn::video::geometry::Triangle triangle1(vkRenderer0->SpawnMesh());
        triangle1.SetColor(unicorn::video::Color::Blue);
        triangle1.Move({1.5f, 0.0, 0.0});

        pWindow0->MousePosition.connect(&onCursorPositionChanged);
        pWindow0->Keyboard.connect(&onWindowKeyboard);
        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
    return 0;
}
