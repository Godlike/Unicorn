// 73s studio

#include <vorpal/video/Graphics.hpp>
#include <vorpal/VorpalEngine.hpp>

vp::VorpalEngine::VorpalEngine()
{

}

vp::VorpalEngine::~VorpalEngine()
{

}

void vp::VorpalEngine::init()
{
    _pGraphics = new video::Graphics();
    _pGraphics->init();
}

void vp::VorpalEngine::run()
{
    _pGraphics->update();
}

void vp::VorpalEngine::deinit() {
    _pGraphics->deinit();
    if (_pGraphics)
        delete _pGraphics;
}


