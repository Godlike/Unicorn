/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>

#include <unicorn/video/Graphics.hpp>

#include <unicorn/window_manager/Window.hpp>
#include <unicorn/window_manager/WindowHint.hpp>
#include <unicorn/window_manager/CustomValue.hpp>

#include <unicorn/core/Settings.hpp>

#include <iostream>

void onWindowSizeChange(unicorn::WindowManager::Window* pWindow, std::pair<int32_t, int32_t> size)
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
        pGraphics->SetWindowCreationHint(unicorn::WindowManager::WindowHint::Decorated,
            unicorn::WindowManager::CustomValue::False);

        // Resizable
        pGraphics->SetWindowCreationHint(unicorn::WindowManager::WindowHint::Resizable,
            unicorn::WindowManager::CustomValue::True);

        const std::vector<unicorn::WindowManager::Monitor*>& monitors = pGraphics->GetMonitors();
        unicorn::WindowManager::Monitor* lastMonitor = monitors.back();
        unicorn::WindowManager::VideoMode activeMode = lastMonitor->GetActiveVideoMode();

        unicorn::WindowManager::Window* pWindow0 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            settings.GetApplicationName(),
            nullptr,
            nullptr );

        unicorn::WindowManager::Window* pWindow1 = pGraphics->SpawnWindow(
            settings.GetApplicationWidth(),
            settings.GetApplicationHeight(),
            std::string("Hmm ") + settings.GetApplicationName(),
            nullptr,
            nullptr );

        // Decorated, with borders
        pGraphics->SetWindowCreationHint(unicorn::WindowManager::WindowHint::Decorated,
            unicorn::WindowManager::CustomValue::True);

        unicorn::WindowManager::Window* pWindow2 = pGraphics->SpawnWindow(
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
