/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/VorpalEngine.hpp>
#include <vorpal/system/Settings.hpp>
#include <vorpal/graphics/Sprite.hpp>
#include <vorpal/graphics/Texture.hpp>
#include <vorpal/graphics/Model.hpp>

int main(int argc, char* argv[])
{
    vp::system::Settings& settings = vp::system::Settings::Instance();

    settings.Init(argc, argv, "sanic_jymper.log");
    settings.SetApplicationName("Unicorn Engine : Sanic Jymper");

    vp::VorpalEngine* vpEngine = new vp::VorpalEngine();
    bool result = vpEngine->Init();

    vp::graphics::Texture texture;
    result = texture.LoadFromFile("data/models/cabin/chalet.jpg");
    
    vp::graphics::Mesh mesh;
    result = mesh.LoadFromFile("data/models/cabin/chalet.obj");
 
    if (result)
    {
        vpEngine->Run();
    }

    vpEngine->Deinit();
    delete vpEngine;

    vp::system::Settings::Destroy();
    return 0;
}
