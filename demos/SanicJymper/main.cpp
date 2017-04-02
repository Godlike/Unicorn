/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>
#include <unicorn/core/Settings.hpp>

int main(int argc, char* argv[])
{
    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();

    if (unicornEngine->Init())
    {
        unicorn::video::Graphics* pGraphics = unicornEngine->GetGraphics();

        unicorn::WindowManager::Window* pWindow1 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicorn::WindowManager::Window* pWindow2 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("Hmm ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicorn::WindowManager::Window* pWindow3 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("wat ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::core::Settings::Destroy();
    return 0;
}
