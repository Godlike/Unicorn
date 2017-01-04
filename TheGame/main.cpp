#include <vorpal/VorpalEngine.hpp>

int main(int argc, char *argv[])
{
    vp::core::Settings vorpal_settings;
    vp::VorpalEngine* vpEngine = new vp::VorpalEngine;
    vpEngine->init();
    vpEngine->run();
    vpEngine->deinit();
    delete vpEngine;
    return 0;
}
