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
#include <unicorn/system/Input.hpp>
#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/Primitives.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/video/CameraFpsController.hpp>
#include <unicorn/video/Material.hpp>

#include <ctime>
#include <array>
#include <cstdlib>
#include <iostream>
#include <list>

static unicorn::video::Graphics* pGraphics = nullptr;
static unicorn::video::CameraFpsController* pCameraController = nullptr;
static unicorn::system::Timer* timer = nullptr;
static unicorn::video::Renderer* vkRenderer = nullptr;
static unicorn::system::Input* pInput = nullptr;
static bool depthTest = true;
unicorn::system::Window* pWindow0 = nullptr;
std::list<unicorn::video::Mesh*> meshes;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void onLogicFrame(unicorn::UnicornEngine* /*engine*/)
{
    float currentFrame = static_cast<float>(timer->ElapsedMilliseconds().count()) / 1000;
    float newDeltatime = currentFrame - lastFrame;
    if(newDeltatime <= 0.0)
    {
        return;
    }
    deltaTime = newDeltatime;
    for(auto& mesh : meshes)
    {
        mesh->modelMatrix.Rotate(deltaTime * 30, {1, 1, 0});
    }
    lastFrame = currentFrame;
}

void onMouseButton(unicorn::system::Window::MouseButtonEvent const& mouseButtonEvent)
{
    using unicorn::system::input::MouseButton;
    using unicorn::system::input::Action;

    unicorn::system::input::Action const& action = mouseButtonEvent.action;

    if(action == Action::Release || action == Action::Repeat)
    {
        return;
    }

    unicorn::system::input::MouseButton const& button = mouseButtonEvent.button;

    switch(button)
    {
        case MouseButton::MouseLeft:
        {
            unicorn::video::Material cubematerial;
            cubematerial.SetColor({static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255});
            unicorn::video::Mesh* cube = vkRenderer->SpawnMesh(cubematerial);
            unicorn::video::Primitives::Cube(*cube);
            meshes.push_back(cube);
            cube->modelMatrix.SetPosition({std::rand() % 40 - 20, std::rand() % 40 - 20, std::rand() % 40 - 20});
            break;
        }
        case MouseButton::MouseMiddle:
        {
            if(meshes.size())
            {
                // Get random cube
                auto meshIt = meshes.begin();

                std::advance(meshIt, std::rand() % meshes.size());

                auto mesh = *meshIt;

                unicorn::video::Material cubeMaterial;
                cubeMaterial.SetColor({static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255});
                cubeMaterial.SetIsWired(true);
                mesh->SetMaterial(cubeMaterial);
            }
            break;
        }
        case MouseButton::MouseRight:
        {
            if(meshes.size())
            {
                // Get random cube
                auto meshIt = meshes.begin();

                std::advance(meshIt, std::rand() % meshes.size());

                auto mesh = *meshIt;

                // Release cube's mesh
                vkRenderer->DeleteMesh(mesh);

                //// Erase cube
                meshes.erase(meshIt);
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

    if(Action::Release == action)
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

    if(Modifier::Shift & modifiers)
    {
        delta *= 10;
    }

    if(Modifier::Alt & modifiers)
    {
        delta *= 5;
    }
    switch(key)
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
            if(Action::Repeat == action)
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
        case Key::Insert:
        {
            if(pInput)
            {
                switch(modifiers)
                {
                    case Modifier::Ctrl:
                    {
                        pInput->SetClipboard(std::string("Gotta go fast"));
                        break;
                    }
                    case Modifier::Shift:
                    {
                        std::cout << "Clipboard data: " << pInput->GetClipboard() << std::endl;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }

    if(positionChanged)
    {
        pWindow->SetPosition(position);
    }
}

void onRendererDestroyed(unicorn::video::Renderer* pRenderer)
{
    if(vkRenderer == pRenderer)
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
    if(unicornEngine->Init())
    {
        pGraphics = unicornEngine->GetGraphics();
        pInput = unicornEngine->GetInput();

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

        {
            //Loading textures
            unicorn::video::Texture texture, textureMandrill;
            unicorn::video::Texture frontSkyBox, backSkyBox, leftSkyBox, rightSkyBox, topSkyBox, bottomSkyBox;

            frontSkyBox.Load("data/textures/city_skybox/lmcity_ft.tga");
            backSkyBox.Load("data/textures/city_skybox/lmcity_bk.tga");
            leftSkyBox.Load("data/textures/city_skybox/lmcity_lf.tga");
            rightSkyBox.Load("data/textures/city_skybox/lmcity_rt.tga");
            topSkyBox.Load("data/textures/city_skybox/lmcity_up.tga");
            bottomSkyBox.Load("data/textures/city_skybox/lmcity_dn.tga");

            texture.Load("data/textures/texture.jpg");
            textureMandrill.Load("data/textures/earth.jpg");

            //Checks data
            if(!texture.IsLoaded()
                || !textureMandrill.IsLoaded()
                || !frontSkyBox.IsLoaded()
                || !backSkyBox.IsLoaded()
                || !leftSkyBox.IsLoaded()
                || !rightSkyBox.IsLoaded()
                || !topSkyBox.IsLoaded()
                || !bottomSkyBox.IsLoaded())
            {
                return -1;
            }

            unicorn::video::Material textureMaterial, blueMaterial,
                                     mandrillMaterial, frontTexture, backTexture,
                                     leftTexture, rightTexture, upTexture, bottomTexture;

            textureMaterial.SetAlbedo(texture);
            mandrillMaterial.SetAlbedo(textureMandrill);
            frontTexture.SetAlbedo(frontSkyBox);
            backTexture.SetAlbedo(backSkyBox);
            leftTexture.SetAlbedo(leftSkyBox);
            rightTexture.SetAlbedo(rightSkyBox);
            upTexture.SetAlbedo(topSkyBox);
            bottomTexture.SetAlbedo(bottomSkyBox);

            blueMaterial.SetColor(unicorn::video::Color::Blue);
            blueMaterial.SetIsColored(true);

            unicorn::video::Mesh* texturedQuad = vkRenderer->SpawnMesh(textureMaterial);
            unicorn::video::Primitives::Quad(*texturedQuad);
            unicorn::video::Mesh* mandrillQuad = vkRenderer->SpawnMesh(mandrillMaterial);
            unicorn::video::Primitives::Quad(*mandrillQuad);
            unicorn::video::Mesh* texturedCube = vkRenderer->SpawnMesh(textureMaterial);
            unicorn::video::Primitives::Cube(*texturedCube);
            unicorn::video::Mesh* frontBox = vkRenderer->SpawnMesh(frontTexture);
            unicorn::video::Primitives::Quad(*frontBox);
            unicorn::video::Mesh* backBox = vkRenderer->SpawnMesh(backTexture);
            unicorn::video::Primitives::Quad(*backBox);
            unicorn::video::Mesh* leftBox = vkRenderer->SpawnMesh(leftTexture);
            unicorn::video::Primitives::Quad(*leftBox);
            unicorn::video::Mesh* rightBox = vkRenderer->SpawnMesh(rightTexture);
            unicorn::video::Primitives::Quad(*rightBox);
            unicorn::video::Mesh* upBox = vkRenderer->SpawnMesh(upTexture);
            unicorn::video::Primitives::Quad(*upBox);
            unicorn::video::Mesh* bottomBox = vkRenderer->SpawnMesh(bottomTexture);
            unicorn::video::Primitives::Quad(*bottomBox);
            unicorn::video::Mesh* sphere = vkRenderer->SpawnMesh(mandrillMaterial);
            unicorn::video::Primitives::Sphere(*sphere, 1, 32, 32);

            sphere->modelMatrix.SetPosition({ -5.0, -1.0, 0.0 });
            mandrillQuad->modelMatrix.Translate({3.0, 0.0, 1.0});
            texturedCube->modelMatrix.SetPosition({0.0, 5.0, -5.0});

            //Skybox
            float skyBoxScaleFactor = 500;
            float skyBoxDistance = skyBoxScaleFactor - 1;

            frontBox->modelMatrix.Translate({0, 0, skyBoxDistance});
            frontBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

            backBox->modelMatrix.Translate({0, 0, -skyBoxDistance});
            backBox->modelMatrix.Rotate(180, {0, 1, 0});
            backBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

            leftBox->modelMatrix.Translate({-skyBoxDistance, 0, 0});
            leftBox->modelMatrix.Rotate(-90, {0, 1, 0});
            leftBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

            rightBox->modelMatrix.Translate({skyBoxDistance, 0, 0});
            rightBox->modelMatrix.Rotate(90, {0, 1, 0});
            rightBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

            upBox->modelMatrix.Translate({0, -skyBoxDistance, 0});
            upBox->modelMatrix.Rotate(90, {1, 0, 0});
            upBox->modelMatrix.Rotate(-90, {0, 0, 1});
            upBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

            bottomBox->modelMatrix.Translate({0, skyBoxDistance, 0});
            bottomBox->modelMatrix.Rotate(-90, {1, 0, 0});
            bottomBox->modelMatrix.Rotate(90, {0, 0, 1});
            bottomBox->modelMatrix.Scale({skyBoxScaleFactor, skyBoxScaleFactor, 0});

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
