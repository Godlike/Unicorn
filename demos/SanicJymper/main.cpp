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
    float currentFrame = static_cast<float>( timer->ElapsedMilliseconds().count() ) / 1000;
    float newDeltatime = currentFrame - lastFrame;
    if( newDeltatime <= 0.0 )
    {
        return;
    }
    deltaTime = newDeltatime;
    for( auto& mesh : meshes )
    {
        mesh->modelMatrix.Rotate( deltaTime * 30, { 0, 0, 1 } ); // 30 - speed
    }
    lastFrame = currentFrame;
}

void onMouseButton(unicorn::system::Window::MouseButtonEvent const& mouseButtonEvent)
{
}

void onCursorPositionChanged(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->UpdateView( pos.first, pos.second );
}

void onMouseScrolled(unicorn::system::Window* pWindow, std::pair<double, double> pos)
{
    pCameraController->Scroll( static_cast<float>( pos.second / 50 ) ); // 50 is zoom coefficient
}

void onWindowKeyboard(unicorn::system::Window::KeyboardEvent const& keyboardEvent)
{
    using unicorn::system::input::Key;
    using unicorn::system::input::Modifier;
    using unicorn::system::input::Action;
    using unicorn::system::MouseMode;

    unicorn::system::input::Action const& action = keyboardEvent.action;

    if( Action::Release == action )
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

    if( Modifier::Shift & modifiers )
    {
        delta *= 10;
    }

    if( Modifier::Alt & modifiers )
    {
        delta *= 5;
    }
    switch( key )
    {
        case Key::W:
            {
                pCameraController->MoveForward( delta );
                break;
            }
        case Key::S:
            {
                pCameraController->MoveBackward( delta );
                break;
            }
        case Key::A:
            {
                pCameraController->MoveLeft( delta );
                break;
            }
        case Key::D:
            {
                pCameraController->MoveRight( delta );
                break;
            }
        case Key::Q:
            {
                pCameraController->MoveUp( delta );
                break;
            }
        case Key::E:
            {
                pCameraController->MoveDown( delta );
                break;
            }
        case Key::Up:
            {
                position.second -= static_cast<uint32_t>( delta );
                positionChanged = true;
                break;
            }
        case Key::Down:
            {
                position.second += static_cast<uint32_t>( delta );
                positionChanged = true;
                break;
            }
        case Key::Left:
            {
                position.first -= static_cast<uint32_t>( delta );
                positionChanged = true;
                break;
            }
        case Key::Right:
            {
                position.first += static_cast<uint32_t>( delta );
                positionChanged = true;
                break;
            }
        case Key::C:
            {
                pWindow->SetMouseMode( MouseMode::Captured );
                break;
            }
        case Key::V:
            {
                if( Action::Repeat == action )
                {
                    return;
                }
                depthTest = !depthTest;
                pGraphics->SetDepthTest( depthTest );
                break;
            }
        case Key::Escape:
            {
                pWindow->SetMouseMode( MouseMode::Normal );
                break;
            }
        case Key::Insert:
            {
                if( pInput )
                {
                    switch( modifiers )
                    {
                        case Modifier::Ctrl:
                            {
                                pInput->SetClipboard( std::string( "Gotta go fast" ) );
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

    if( positionChanged )
    {
        pWindow->SetPosition( position );
    }
}

void onRendererDestroyed(unicorn::video::Renderer* pRenderer)
{
    if( vkRenderer == pRenderer )
    {
        vkRenderer = nullptr;
    }
}

int main(int argc, char* argv[])
{
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init( argc, argv, "Sanic_Jymper.log" );
    settings.SetApplicationName( "SANIC JYMPER" );
    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();
    timer = new unicorn::system::Timer( true );
    if( unicornEngine->Init() )
    {
        pGraphics = unicornEngine->GetGraphics();
        pInput = unicornEngine->GetInput();

        unicornEngine->LogicFrame.connect( &onLogicFrame );

        pGraphics->SetWindowCreationHint( unicorn::system::WindowHint::Decorated,
                                          unicorn::system::CustomValue::True );

        pGraphics->SetWindowCreationHint( unicorn::system::WindowHint::Resizable,
                                          unicorn::system::CustomValue::True );

        auto h = pGraphics->GetMonitors().back()->GetActiveVideoMode().height;
        auto w = pGraphics->GetMonitors().back()->GetActiveVideoMode().width;
        settings.SetApplicationHeight( h );
        settings.SetApplicationWidth( w );

        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow( settings.GetApplicationWidth(),
                                                                    settings.GetApplicationHeight(),
                                                                    settings.GetApplicationName(),
                                                                    nullptr,
                                                                    nullptr );
        pWindow0->SetMouseMode( unicorn::system::MouseMode::Captured );

        vkRenderer = pGraphics->SpawnRenderer( pWindow0 );
        vkRenderer->Destroyed.connect( &onRendererDestroyed );

        vkRenderer->SetBackgroundColor( unicorn::video::Color::LightPink );
        pCameraController = new unicorn::video::CameraFpsController( vkRenderer->GetCamera() );

        using unicorn::video::Mesh;
        using unicorn::video::Cube;
        using unicorn::video::Quad;
        {
            //Loading textures
            unicorn::video::Texture texture, textureMandrill;
            unicorn::video::Texture frontSkyBox, backSkyBox, leftSkyBox, rightSkyBox, topSkyBox, bottomSkyBox;

            frontSkyBox.Load( "data/textures/city_skybox/lmcity_ft.tga" );
            backSkyBox.Load( "data/textures/city_skybox/lmcity_bk.tga" );
            leftSkyBox.Load( "data/textures/city_skybox/lmcity_lf.tga" );
            rightSkyBox.Load( "data/textures/city_skybox/lmcity_rt.tga" );
            topSkyBox.Load( "data/textures/city_skybox/lmcity_up.tga" );
            bottomSkyBox.Load( "data/textures/city_skybox/lmcity_dn.tga" );

            texture.Load( "data/textures/texture.jpg" );
            textureMandrill.Load( "data/textures/mandrill.png" );

            //Checks data
            if( !texture.IsLoaded()
                || !textureMandrill.IsLoaded()
                || !frontSkyBox.IsLoaded()
                || !backSkyBox.IsLoaded()
                || !leftSkyBox.IsLoaded()
                || !rightSkyBox.IsLoaded()
                || !topSkyBox.IsLoaded()
                || !bottomSkyBox.IsLoaded() )
            {
                return -1;
            }

            unicorn::video::Material textureMaterial;
            textureMaterial.SetAlbedo( texture );
            unicorn::video::Material mandrillMaterial;
            mandrillMaterial.SetAlbedo( textureMandrill );
            unicorn::video::Material defaultMaterial;

            unicorn::video::Material blueMaterial;
            blueMaterial.SetColor( unicorn::video::Color::Blue );
            blueMaterial.SetWireframe( true );

            unicorn::video::Material wrongMaterial;
            wrongMaterial.SetIsColored( false );

            Quad* texturedQuad = new Quad( textureMaterial );
            Quad* mandrillQuad = new Quad( mandrillMaterial );
            Quad* coloredQuad = new Quad( blueMaterial );
            Cube* texturedCube = new Cube( textureMaterial );
            Cube* texturedCube2 = new Cube( textureMaterial );

            mandrillQuad->modelMatrix.Translate( { 3.0, 0.0, 1.0 } );
            coloredQuad->modelMatrix.Translate( { -3.0, 0.0, 6.0 } );
            texturedCube->modelMatrix.Translate( { 0.0, 5.0, -5.0 } );
            texturedCube2->modelMatrix.Translate( { 0.0, -5.0, -5.0 } );

            meshes.push_back( texturedQuad );

            vkRenderer->AddMesh( texturedQuad );
            vkRenderer->AddMesh( mandrillQuad );
            vkRenderer->AddMesh( coloredQuad );
            vkRenderer->AddMesh( texturedCube );
            vkRenderer->AddMesh( texturedCube2 );

            //Skybox
            float skyBoxScaleFactor = 500;
            float skyBoxDistance = skyBoxScaleFactor - 1; // Pifagor

            unicorn::video::Material frontTexture;
            frontTexture.SetAlbedo( frontSkyBox );
            Quad* frontBox = new Quad( frontTexture );
            frontBox->modelMatrix.Translate( { 0, 0, skyBoxDistance } );
            frontBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            unicorn::video::Material backTexture;
            backTexture.SetAlbedo( backSkyBox );
            Quad* backBox = new Quad( backTexture );
            backBox->modelMatrix.Translate( { 0, 0, -skyBoxDistance } );
            backBox->modelMatrix.Rotate( 180, { 0, 1, 0 } );
            backBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            unicorn::video::Material leftTexture;
            leftTexture.SetAlbedo( leftSkyBox );
            Quad* leftBox = new Quad( leftTexture );
            leftBox->modelMatrix.Translate( { -skyBoxDistance, 0, 0 } );
            leftBox->modelMatrix.Rotate( -90, { 0, 1, 0 } );
            leftBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            unicorn::video::Material rightTexture;
            rightTexture.SetAlbedo( rightSkyBox );
            Quad* rightBox = new Quad( rightTexture );
            rightBox->modelMatrix.Translate( { skyBoxDistance, 0, 0 } );
            rightBox->modelMatrix.Rotate( 90, { 0, 1, 0 } );
            rightBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            unicorn::video::Material upTexture;
            upTexture.SetAlbedo( topSkyBox );
            Quad* upBox = new Quad( upTexture );
            upBox->modelMatrix.Translate( { 0, -skyBoxDistance, 0 } );
            upBox->modelMatrix.Rotate( 90, { 1, 0, 0 } );
            upBox->modelMatrix.Rotate( -90, { 0, 0, 1 } );
            upBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            unicorn::video::Material bottomTexture;
            bottomTexture.SetAlbedo( bottomSkyBox );
            Quad* bottomBox = new Quad( bottomTexture );
            bottomBox->modelMatrix.Translate( { 0, skyBoxDistance, 0 } );
            bottomBox->modelMatrix.Rotate( -90, { 1, 0, 0 } );
            bottomBox->modelMatrix.Scale( { skyBoxScaleFactor, skyBoxScaleFactor, 0 } );

            vkRenderer->AddMesh( frontBox );
            vkRenderer->AddMesh( backBox );
            vkRenderer->AddMesh( leftBox );
            vkRenderer->AddMesh( rightBox );
            vkRenderer->AddMesh( upBox );
            vkRenderer->AddMesh( bottomBox );

            pWindow0->MousePosition.connect( &onCursorPositionChanged );
            pWindow0->Scroll.connect( &onMouseScrolled );
            pWindow0->Keyboard.connect( &onWindowKeyboard );
            pWindow0->MouseButton.connect( &onMouseButton );

            unicornEngine->Run();
        }
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();

    system( "pause" );
}
