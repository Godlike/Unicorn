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
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/geometry/Primitives.hpp>
#include <unicorn/video/CameraFpsController.hpp>

#include <array>
#include <cstdlib>
#include <iostream>
#include <list>

static unicorn::video::Graphics* pGraphics = nullptr;
static unicorn::video::CameraFpsController* pCameraController = nullptr;
static unicorn::system::Timer* timer = nullptr;
static unicorn::video::Renderer* vkRenderer = nullptr;
static bool depthTest = true;
unicorn::system::Window* pWindow0 = nullptr;
std::list<unicorn::video::geometry::MeshDescriptor*> cubes;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    float currentFrame = static_cast<float>(timer->ElapsedMilliseconds().count()) / 1000;
    float newDeltatime = currentFrame - lastFrame;
    if (newDeltatime <= 0.0)
    {
        return;
    }
    deltaTime = newDeltatime;
    for (auto& mesh : cubes)
    {
        mesh->Rotate(deltaTime, {1, 1, 0});
    }
    lastFrame = currentFrame;
}

void onMouseButton(unicorn::system::Window::MouseButtonEvent const& mouseButtonEvent)
{
    using unicorn::system::input::MouseButton;
    using unicorn::system::input::Action;
    using unicorn::video::geometry::MeshDescriptor;
    using unicorn::video::geometry::Primitives;

    unicorn::system::input::Action const& action = mouseButtonEvent.action;

    if (action == Action::Release || action == Action::Repeat)
    {
        return;
    }

    unicorn::system::input::MouseButton const& button = mouseButtonEvent.button;

    switch (button)
    {
        case MouseButton::MouseLeft:
        {
            MeshDescriptor* obj = new MeshDescriptor(Primitives::Cube(*(vkRenderer->SpawnMesh())));
            obj->Translate({std::rand() % 40 - 20, std::rand() % 40 - 20, std::rand() % 40 - 20});
            obj->SetColor({static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255});
            cubes.push_back(obj);

            break;
        }
        case MouseButton::MouseRight:
        {
            if (cubes.size())
            {
                // Get random cube
                auto meshIt = cubes.begin();

                std::advance(meshIt, std::rand() % cubes.size());

                // Fetch cube's mesh
                auto const& mesh = (*meshIt)->GetMesh();

                // Erase cube
                cubes.erase(meshIt);

                // Release cube's mesh
                vkRenderer->DeleteMesh(&mesh);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

void onCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->UpdateView(pos.first, pos.second);
}

void onMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->Scroll(static_cast<float>(pos.second / 50)); // 50 is zoom coefficient
}

void onWindowKeyboard(unicorn::system::Window::KeyboardEvent const& keyboardEvent)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;
    using unicorn::system::MouseMode;

    unicorn::system::input::Action const& action = keyboardEvent.action;

    if (Action::Release == action)
    {
        return;
    }

    unicorn::system::Window* const& pWindow = keyboardEvent.pWindow;
    unicorn::system::input::Key const& key = keyboardEvent.key;
    uint32_t const& scancode = keyboardEvent.scancode;
    unicorn::system::input::Modifier::Mask const& modifiers = keyboardEvent.modifiers;

    std::pair<int32_t, int32_t> position = pWindow->GetPosition();
    bool positionChanged = false;

    float delta = deltaTime * 0.1f;

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
            position.second -= static_cast<uint32_t>(delta);
            positionChanged = true;
            break;
        }
    case Key::Down:
        {
            position.second += static_cast<uint32_t>(delta);
            positionChanged = true;
            break;
        }
    case Key::Left:
        {
            position.first -= static_cast<uint32_t>(delta);
            positionChanged = true;
            break;
        }
    case Key::Right:
        {
            position.first += static_cast<uint32_t>(delta);
            positionChanged = true;
            break;
        }
    case Key::C:
        {
            pWindow->SetMouseMode(MouseMode::Captured);
            break;
        }
    case Key::V:
    {
        if (Action::Repeat == action)
        {
            return;
        }
        depthTest = !depthTest;
        pGraphics->SetDepthTest(depthTest);
        break;
    }
    case Key::Escape:
        {
            pWindow->SetMouseMode(MouseMode::Normal);
            break;
        }
    default:
        {
            break;
        }
    }

    if (positionChanged)
    {
        pWindow->SetPosition(position);
    }
}

void onRendererDestroyed(unicorn::video::Renderer* pRenderer)
{
    if (vkRenderer == pRenderer)
    {
        vkRenderer = nullptr;
    }
}

int main(int argc, char* argv[])
{
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init(argc, argv, "Sanic_Jymper.log");
    settings.SetApplicationName("SANIC JYMPER");
    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();
    timer = new unicorn::system::Timer(true);
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

        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow(settings.GetApplicationWidth(),
                                                                   settings.GetApplicationHeight(),
                                                                   settings.GetApplicationName(),
                                                                   nullptr,
                                                                   nullptr);
        pWindow0->SetMouseMode(unicorn::system::MouseMode::Captured);

        vkRenderer = pGraphics->SpawnRenderer(pWindow0);
        vkRenderer->Destroyed.connect(&onRendererDestroyed);

        vkRenderer->SetBackgroundColor(unicorn::video::Color::LightPink);
        pCameraController = new unicorn::video::CameraFpsController(vkRenderer->GetCamera());

        using unicorn::video::geometry::Mesh;
        using unicorn::video::geometry::MeshDescriptor;
        using unicorn::video::geometry::Primitives;

        std::array<Mesh*, 3> meshes = {
            vkRenderer->SpawnMesh(),
            vkRenderer->SpawnMesh(),
            vkRenderer->SpawnMesh()
        };

        {
            MeshDescriptor triangle1 = Primitives::Triangle(*meshes[0]);
            triangle1.SetColor(unicorn::video::Color::Red);
            triangle1.Translate({-2.0f, 0.0f, 0.0f});
            triangle1.Scale({0.5, 0.5, 0.5});

            MeshDescriptor triangle2 = Primitives::Triangle(*meshes[1]);
            triangle2.SetColor(unicorn::video::Color::Green);

            MeshDescriptor cube = Primitives::Cube(*meshes[2]);
            cube.Translate({5.0, 0.0f, 5.0f});

            pWindow0->MousePosition.connect(&onCursorPositionChanged);
            pWindow0->Scroll.connect(&onMouseScrolled);
            pWindow0->Keyboard.connect(&onWindowKeyboard);
            pWindow0->MouseButton.connect(&onMouseButton);

            unicornEngine->Run();
        }

        if (vkRenderer)
        {
            for (auto& pMesh : meshes)
            {
                vkRenderer->DeleteMesh(pMesh);
            }
        }
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
}
