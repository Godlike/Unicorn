#include <vorpal/video/Graphics.hpp>
#include <vorpal/VorpalEngine.hpp>
#include <loguru.hpp>

vp::VorpalEngine* vp::VorpalEngine::s_engine = nullptr;

vp::VorpalEngine::VorpalEngine(std::string appName, unsigned int width, unsigned int height) : _applicationName(appName), m_width(width), _height(height)
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
}
std::string vp::VorpalEngine::applicationName() const {
  return _applicationName;
}
unsigned int vp::VorpalEngine::applicationHeight() const {
  return _height;
}
unsigned int vp::VorpalEngine::applicationWidth() const {
  return  m_width;
}
std::string vp::VorpalEngine::vorpalEngineName() const {
  return  "Vorpal Engine"; //TODO: remove hardcode
}
void vp::VorpalEngine::logError(const std::string &&error)
{
  LOG_F(ERROR ,error.c_str());
}


