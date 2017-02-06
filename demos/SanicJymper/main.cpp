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

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    vp::VorpalEngine* vpEngine = new vp::VorpalEngine();
    bool result = vpEngine->Init();

    vp::graphics::Texture texture;
    texture.LoadFromFile("data/models/cabin/chalet.jpg");

    /*vp::graphics::Camera camera;
    camera.SetPerspective(45.0f, 45.0f, 0.1f, 1000.0f);
    camera.SetPosition({0.0f, 0.0f, 0.0f});
    */

    if (result)
    {
        vpEngine->Run();
    }

    vpEngine->Deinit();
    delete vpEngine;

    vp::system::Settings::Destroy();
    return 0;
}
