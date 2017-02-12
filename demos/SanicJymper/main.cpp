/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/system/Settings.hpp>
#include <unicorn/graphics/Sprite.hpp>
#include <unicorn/graphics/Texture.hpp>
#include <unicorn/graphics/Model.hpp>

int main(int argc, char* argv[])
{
    uc::system::Settings& settings = uc::system::Settings::Instance();

    settings.Init(argc, argv, "sanic_jymper.log");
    settings.SetApplicationName("Unicorn Engine : Sanic Jymper");

    uc::UnicornEngine* ucEngine = new uc::UnicornEngine();
    bool result = ucEngine->Init();

    uc::graphics::Texture texture;
    result = texture.LoadFromFile("data/models/cabin/chalet.jpg");
    
    uc::graphics::Mesh mesh;
    result = mesh.LoadFromFile("data/models/cabin/chalet.obj");
 
    if (result)
    {
        ucEngine->Run();
    }

    ucEngine->Deinit();
    delete ucEngine;

    uc::system::Settings::Destroy();
    return 0;
}
