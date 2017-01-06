#include <vorpal/VorpalEngine.hpp>
#include <vorpal/core/Settings.hpp>

int main(int argc, char *argv[]) {
  vp::core::Settings::instance()->init(argc, argv, "SANIC_JYMPER.log");
  vp::core::Settings::instance()->applicationName("SANIC JYMPER");
  vp::VorpalEngine *vpEngine = new vp::VorpalEngine;
  if(vpEngine->init())
    vpEngine->run();
  vpEngine->deinit();
  delete vpEngine;
  return 0;
}
