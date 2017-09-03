/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornRender.hpp>
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

void onLogicFrame(unicorn::UnicornRender* /*render*/)
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
        mesh->modelMatrix = glm::rotate(mesh->modelMatrix, glm::radians(deltaTime * 30), { 1, 1, 0 });
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
            cubematerial.color = {static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255};
            unicorn::video::Mesh* cube = vkRenderer->SpawnMesh(cubematerial);
            unicorn::video::Primitives::Cube(*cube);
            meshes.push_back(cube);
            cube->modelMatrix = glm::translate(cube->modelMatrix, { std::rand() % 40 - 20, std::rand() % 40 - 20, std::rand() % 40 - 20 });
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
                cubeMaterial.color = {static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255, static_cast<float>(std::rand() % 255) / 255};
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
            if (pInput)
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
    unicorn::UnicornRender* unicornRender = new unicorn::UnicornRender();
    timer = new unicorn::system::Timer(true);
    if (unicornRender->Init())
    {
        pGraphics = unicornRender->GetGraphics();
        pInput = unicornRender->GetInput();

        unicornRender->LogicFrame.connect(&onLogicFrame);

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
            textureMandrill.Load("data/textures/mandrill.png");

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

            unicorn::video::Material textureMaterial,
                                     mandrillMaterial, frontTexture, backTexture,
                                     leftTexture, rightTexture, upTexture, bottomTexture;

            textureMaterial.SetAlbedo(&texture);
            mandrillMaterial.SetAlbedo(&textureMandrill);
            frontTexture.SetAlbedo(&frontSkyBox);
            backTexture.SetAlbedo(&backSkyBox);
            leftTexture.SetAlbedo(&leftSkyBox);
            rightTexture.SetAlbedo(&rightSkyBox);
            upTexture.SetAlbedo(&topSkyBox);
            bottomTexture.SetAlbedo(&bottomSkyBox);

            unicorn::video::Mesh* texturedQuad = vkRenderer->SpawnMesh(textureMaterial);
            unicorn::video::Primitives::Quad(*texturedQuad);
            unicorn::video::Mesh* texturedQuad2 = vkRenderer->SpawnMesh(textureMaterial);
            unicorn::video::Primitives::Quad(*texturedQuad2);
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

            mandrillQuad->modelMatrix = glm::translate(mandrillQuad->modelMatrix, {3.0, 0.0, 1.0});
            texturedCube->modelMatrix = glm::translate(texturedCube->modelMatrix, {0.0, 5.0, -5.0});

            meshes.push_back(texturedQuad);
            meshes.push_back(texturedQuad2);

            //Skybox
            float skyBoxScaleFactor = 500;
            float skyBoxDistance = skyBoxScaleFactor - 1;

            frontBox->modelMatrix = glm::translate(frontBox->modelMatrix, {0, 0, skyBoxDistance});
            frontBox->modelMatrix = glm::scale(frontBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            backBox->modelMatrix = glm::translate(backBox->modelMatrix, {0, 0, -skyBoxDistance});
            backBox->modelMatrix = glm::rotate(backBox->modelMatrix, glm::radians(180.f), {0, 1, 0});
            backBox->modelMatrix = glm::scale(backBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            leftBox->modelMatrix = glm::translate(leftBox->modelMatrix, {-skyBoxDistance, 0, 0});
            leftBox->modelMatrix = glm::rotate(leftBox->modelMatrix, glm::radians(-90.f), {0, 1, 0});
            leftBox->modelMatrix = glm::scale(leftBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            rightBox->modelMatrix = glm::translate(rightBox->modelMatrix, {skyBoxDistance, 0, 0});
            rightBox->modelMatrix = glm::rotate(rightBox->modelMatrix, glm::radians(90.f), {0, 1, 0});
            rightBox->modelMatrix = glm::scale(rightBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            upBox->modelMatrix = glm::translate(upBox->modelMatrix, {0, -skyBoxDistance, 0});
            upBox->modelMatrix = glm::rotate(upBox->modelMatrix, glm::radians(90.f), {1, 0, 0});
            upBox->modelMatrix = glm::rotate(upBox->modelMatrix, glm::radians(-90.f), {0, 0, 1});
            upBox->modelMatrix = glm::scale(upBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            bottomBox->modelMatrix = glm::translate(bottomBox->modelMatrix, {0, skyBoxDistance, 0});
            bottomBox->modelMatrix = glm::rotate(bottomBox->modelMatrix, glm::radians(-90.f), {1, 0, 0});
            bottomBox->modelMatrix = glm::rotate(bottomBox->modelMatrix, glm::radians(90.f), {0, 0, 1});
            bottomBox->modelMatrix = glm::scale(bottomBox->modelMatrix, {skyBoxScaleFactor, skyBoxScaleFactor, 0});

            pWindow0->MousePosition.connect(&onCursorPositionChanged);
            pWindow0->Scroll.connect(&onMouseScrolled);
            pWindow0->Keyboard.connect(&onWindowKeyboard);
            pWindow0->MouseButton.connect(&onMouseButton);

            unicornRender->Run();
        }
    }

    unicornRender->Deinit();
    delete unicornRender;

    unicorn::Settings::Destroy();
}
