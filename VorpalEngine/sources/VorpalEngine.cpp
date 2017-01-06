#include <vorpal/VorpalEngine.hpp>
#include <vorpal/core/Logger.hpp>
#include <vorpal/video/Graphics.hpp>

vp::VorpalEngine::VorpalEngine() {

}

vp::VorpalEngine::~VorpalEngine() {}

bool vp::VorpalEngine::init() {
  LOG_INFO("Engine initialization started.");
  m_pGraphics = new video::Graphics();
  if (!m_pGraphics->init())
    return false;
  LOG_INFO("Engine initialization finished.");
}

void vp::VorpalEngine::deinit() {
  m_pGraphics->deinit();
  LOG_INFO("Engine shutdown correctly.");
}
void vp::VorpalEngine::run() { m_pGraphics->render(); }
