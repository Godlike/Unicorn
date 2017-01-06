#include <vorpal/core/Logger.hpp>
#include <vorpal/core/Settings.hpp>

vp::core::Settings *vp::core::Settings::s_settings = nullptr;

vp::core::Settings::Settings()
    : m_width(640), m_height(480), m_applicationName("SAMPLE NAME"), m_vorpalEngineName("Vorpal Engine") {
}

void vp::core::Settings::init(int argc, char **argv, const std::string &&logFileName) {
  loguru::init(argc, argv);
  loguru::add_file(logFileName.c_str(), loguru::Append, loguru::Verbosity_INFO);
}

std::string vp::core::Settings::applicationName() const {
  return m_applicationName;
}
unsigned int vp::core::Settings::applicationHeight() const { return m_height; }
unsigned int vp::core::Settings::applicationWidth() const { return m_width; }
std::string vp::core::Settings::vorpalEngineName() const {
  return m_vorpalEngineName;
}
void vp::core::Settings::applicationName(const std::string &&newName) {

}
void vp::core::Settings::applicationHeight(unsigned int newHeight) {
  m_height = newHeight;
}
void vp::core::Settings::applicationWidth(unsigned int newWidth) {
  m_width = newWidth;
}
void vp::core::Settings::vorpalEngineName(const std::string &&newName) {
  m_vorpalEngineName = newName;
}
