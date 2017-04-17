/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/Camera.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/system/WindowHint.hpp>
#include <unicorn/system/CustomValue.hpp>
#include <unicorn/Settings.hpp>

#include <iostream>

void onWindowSizeChange(unicorn::system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    std::cout << "Window[" << pWindow->GetId() << "]: size changed to " << size.first << "x" << size.second << std::endl;
}

int main(int argc, char* argv[])
{
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();

    if (unicornEngine->Init())
    {
        unicorn::video::Graphics* pGraphics = unicornEngine->GetGraphics();
        pGraphics->CreateVulkanContext();
        // Resizable
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
            unicorn::system::CustomValue::True);
       
        unicorn::system::Window* pWindow0 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicorn::system::Window* pWindow1 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("Hmm ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        auto vkRenderer0 = pGraphics->SpawnVulkanRenderer(pWindow0);
        auto vkRenderer1 = pGraphics->SpawnVulkanRenderer(pWindow1);

        pGraphics->BindWindowRenderer(pWindow0, vkRenderer0);
        pGraphics->BindWindowRenderer(pWindow1, vkRenderer1);

        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::Settings::Destroy();
    return 0;
}
