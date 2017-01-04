
#include <vorpal/VorpalEngine.hpp>
#include <vorpal/core/Logger.hpp>

vp::VorpalEngine::VorpalEngine() {

}

vp::VorpalEngine::~VorpalEngine() {}

void vp::VorpalEngine::init() {
  m_pGraphics = new video::Graphics();
  if(!m_pGraphics->init())
    deinit();
  LOG_INFO("Engine initialized correctly.");
}

void vp::VorpalEngine::deinit() { m_pGraphics->deinit(); LOG_INFO("Engine shutdown correctly.");}
void vp::VorpalEngine::run() { m_pGraphics->render(); }
