/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/core/Settings.hpp>

int main(int argc, char* argv[])
{
    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "SANIC_JYMPER.log");
    settings.SetApplicationName("SANIC JYMPER");

    unicorn::UnicornEngine* vpEngine = new unicorn::UnicornEngine();

    if (vpEngine->Init())
    {
        vpEngine->Run();
    }

    vpEngine->Deinit();
    delete vpEngine;

    unicorn::core::Settings::Destroy();
    return 0;
}
