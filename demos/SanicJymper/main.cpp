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
#include <unicorn/Settings.hpp>
#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/video/geometry/Triangle.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/geometry/Quad.hpp>
#include <unicorn/video/geometry/Cube.hpp>
#include <unicorn/video/CameraFPSController.hpp>
#include <cstdlib>

static unicorn::video::Graphics* pGraphics = nullptr;
static unicorn::video::CameraFpsController* pCameraController = nullptr;
static unicorn::system::Timer* timer = nullptr;
static unicorn::video::Renderer* vkRenderer0 = nullptr;
unicorn::system::Window* pWindow0;
std::vector<unicorn::video::geometry::Cube*> cubes;

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    timer->Reset();
}

void onMouseButton(unicorn::system::Window* pWindow, unicorn::system::input::MouseButton button, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask)
{
    if(button == unicorn::system::input::MouseButton::MouseLeft)
    {
		auto kek = new unicorn::video::geometry::Cube(vkRenderer0->SpawnMesh());
		kek->Move({ std::rand() % 20 - 10, std::rand() % 20 - 10, std::rand() % 20 - 10});
		kek->SetColor({ (float)(std::rand() % 255) / 255, (float)(std::rand() % 255) / 255, (float)(std::rand() % 255) / 255 });
		cubes.push_back(kek);
    }
}

void onCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    static bool herpderp = true;
    
    if (herpderp)
    {
        pCameraController->SetCenterPosition(pos.first, pos.second);
        herpderp = false;
    }

    pCameraController->UpdateMouseView(pos.first, pos.second);
}

void onMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->Scroll(pos.second / 50);
}

void onWindowKeyboard(unicorn::system::Window* pWindow, unicorn::system::input::Key key, uint32_t scancode, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask modifiers)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;
    using unicorn::system::MouseMode;

    if ( Action::Release == action )
    {
        return;
    }

    std::pair<int32_t, int32_t> position = pWindow->GetPosition();
    bool positionChanged = true;

    float delta = static_cast< float >( timer->ElapsedMilliseconds().count() ) / 1000;

    if ( Modifier::Shift & modifiers )
    {
        delta *= 10;
    }

    if ( Modifier::Alt & modifiers )
    {
        delta *= 5;
    }
    switch ( key )
    {
        case Key::W:
        {
            pCameraController->MoveForward(delta);
            break;
        }
        case Key::S:
        {
            pCameraController->MoveBackward(delta);
            break;
        }
        case Key::A:
        {
            pCameraController->MoveLeft(delta);
            break;
        }
        case Key::D:
        {
            pCameraController->MoveRight(delta);
            break;
        }
        case Key::Q:
        {
            pCameraController->MoveUp(delta);
            break;
        }
        case Key::E:
        {
            pCameraController->MoveDown(delta);
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

    if ( positionChanged )
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
    timer = new unicorn::system::Timer(true);
    if ( unicornEngine->Init() )
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

        vkRenderer0 = pGraphics->SpawnVulkanRenderer(pWindow0);
        vkRenderer0->SetBackgroundColor(unicorn::video::Color::LightPink);
        pCameraController = new unicorn::video::CameraFpsController(vkRenderer0->GetCamera());

        unicorn::video::geometry::Triangle triangle1(vkRenderer0->SpawnMesh());
        triangle1.SetColor(unicorn::video::Color::Red);
        triangle1.Move({ -2.0f, 0.0f, 0.0f });

        unicorn::video::geometry::Triangle triangle2(vkRenderer0->SpawnMesh());
        triangle2.SetColor(unicorn::video::Color::Green);

        unicorn::video::geometry::Cube cube(vkRenderer0->SpawnMesh());
        cube.Move({ 5.0, 0.0f, 5.0f });

        pWindow0->MousePosition.connect(&onCursorPositionChanged);
        pWindow0->Scroll.connect(&onMouseScrolled);
        pWindow0->Keyboard.connect(&onWindowKeyboard);
        pWindow0->MouseButton.connect(&onMouseButton);
        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
    return 0;
}
