#include <vorpal/VorpalEngine.hpp>

int main(int argc, char *argv[])
{
    vp::VorpalEngine engine;
    engine.init();
    engine.run();
  engine.deinit();
    return 0;
}
