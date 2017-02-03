/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/VorpalEngine.hpp>
#include <vorpal/system/Settings.hpp>

int main(int argc, char* argv[])
{
    vp::system::Settings& settings = vp::system::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    vp::VorpalEngine* vpEngine = new vp::VorpalEngine();

    if (vpEngine->Init())
    {
        vpEngine->Run();
    }

    vpEngine->Deinit();
    delete vpEngine;

    vp::system::Settings::Destroy();
    return 0;
}
