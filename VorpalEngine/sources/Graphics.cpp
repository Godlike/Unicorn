#include <vorpal/video/Graphics.hpp>
#include <vorpal/video/Renderer.hpp>

vp::video::Graphics::Graphics() : _pRenderer(nullptr)
{

}

void vp::video::Graphics::init()
{
    _pRenderer = new Renderer();
    _pRenderer->init();
}

void vp::video::Graphics::deinit()
{
    _pRenderer->deinit();
    if(_pRenderer)
        delete _pRenderer;
}

void vp::video::Graphics::update()
{
    _pRenderer->update();
}
