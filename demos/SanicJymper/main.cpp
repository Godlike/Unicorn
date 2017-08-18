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
#include <unicorn/video/Primitives.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/video/CameraFpsController.hpp>
#include <unicorn/video/Material.hpp>
#include <unicorn/video/Model.hpp>

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
    lastFrame = currentFrame;
}

void onMouseButton(unicorn::system::Window* /*pWindow*/, unicorn::system::input::MouseButton button, unicorn::system::input::Action action, unicorn::system::input::Modifier::Mask)
{

}

void onCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->UpdateView(pos.first, pos.second);
}

void onMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->Scroll(static_cast<float>(pos.second / 50)); // 50 is zoom coefficient
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

    float delta = deltaTime;

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
        break;
    }
    case Key::Down:
    {
        position.second += static_cast<uint32_t>(delta);
        break;
    }
    case Key::Left:
    {
        position.first -= static_cast<uint32_t>(delta);
        break;
    }
    case Key::Right:
    {
        position.first += static_cast<uint32_t>(delta);
        break;
    }
    case Key::C:
    {
        pWindow->SetMouseMode(MouseMode::Captured);
        break;
    }
    case Key::V:
    {
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
        positionChanged = false;
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

        using unicorn::video::Mesh;
        using unicorn::video::Model;
        using unicorn::video::Cube;
        using unicorn::video::Quad;
        {
            //Loading textures
            unicorn::video::Texture* texture = new unicorn::video::Texture("data/textures/texture.jpg");
            unicorn::video::Texture* textureMandrill = new unicorn::video::Texture("data/textures/mandrill.png");

            //Checks data
            if (!texture->IsLoaded()
                || !textureMandrill->IsLoaded())
            {
                return -1;
            }
            Quad* texturedQuad = new Quad;
            texturedQuad->AddTexture(texture);
            Model* mymodel = new Model(texturedQuad, { false });

            vkRenderer->AddModel(mymodel);

            pWindow0->MousePosition.connect(&onCursorPositionChanged);
            pWindow0->Scroll.connect(&onMouseScrolled);
            pWindow0->Keyboard.connect(&onWindowKeyboard);
            pWindow0->MouseButton.connect(&onMouseButton);

            unicornEngine->Run();
        }
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
}
