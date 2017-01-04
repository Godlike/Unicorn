#include <vorpal/VorpalEngine.hpp>

int main(int argc, char *argv[])
{
    vp::VorpalEngine::instance()->init();
    vp::VorpalEngine::instance()->run();
    vp::VorpalEngine::instance()->deinit();
    return 0;
}
