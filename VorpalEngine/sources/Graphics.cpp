#include <vorpal/video/Graphics.hpp>
#include <vorpal/video/Renderer.hpp>
#include <vorpal/core/Logger.hpp>

vp::video::Graphics::Graphics() : m_pRenderer(nullptr) {}

bool vp::video::Graphics::init() {
  LOG_INFO("Graphics initialization started.");
  m_pRenderer = new Renderer();
  if (!m_pRenderer->init())
    return false;
  LOG_INFO("Graphics initialized correctly.");
  return true;
}

void vp::video::Graphics::deinit() {
  m_pRenderer->deinit();
}

void vp::video::Graphics::render() { m_pRenderer->render(); }
