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

#include <unicorn/system/Settings.hpp>

#include <iostream>

void onWindowSizeChange(unicorn::system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    std::cout << "Window[" << pWindow->GetId() << "]: size changed to " << size.first << "x" << size.second << std::endl;
}

int main(int argc, char* argv[])
{
    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* unicornEngine = new unicorn::UnicornEngine();

    if (unicornEngine->Init())
    {
        unicorn::video::Graphics* pGraphics = unicornEngine->GetGraphics();

        // Borderless undecorated
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::False);

        // Resizable
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Resizable,
            unicorn::system::CustomValue::True);

        const std::vector<unicorn::system::Monitor*>& monitors = pGraphics->GetMonitors();
        unicorn::system::Monitor* lastMonitor = monitors.back();
        unicorn::system::VideoMode activeMode = lastMonitor->GetActiveVideoMode();

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

        // Decorated, with borders
        pGraphics->SetWindowCreationHint(unicorn::system::WindowHint::Decorated,
            unicorn::system::CustomValue::True);

        unicorn::system::Window* pWindow2 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("wat ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        pWindow2->SizeChanged.connect(&onWindowSizeChange);

        pWindow0->Minimize();

        std::pair<int32_t, int32_t> win2pos = pWindow1->GetPosition();
        win2pos.first += 640;
        win2pos.second += 480;
        pWindow1->SetPosition(win2pos);
        pWindow1->SetName("Ayy lmao");

        pWindow2->Focus();

        unicornEngine->Run();
    }

    unicornEngine->Deinit();
    delete unicornEngine;

    unicorn::core::Settings::Destroy();
    return 0;
}
