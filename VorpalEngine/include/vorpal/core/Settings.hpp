#ifndef VORPAL_SETTINGS_HPP
#define VORPAL_SETTINGS_HPP

#include <vorpal/core/Utility.hpp>
#include <string>

namespace vp {
namespace core {
class VORPAL_EXPORT Settings {
public:
  static Settings* instance() {
    if(s_settings == nullptr)
      s_settings = new Settings;
    return s_settings;
  }
  void init(int argc, char *argv[], const std::string&& logFileName);

  std::string applicationName() const;
  unsigned int applicationHeight() const;
  unsigned int applicationWidth() const;
  std::string vorpalEngineName() const;

  void applicationName(const std::string&& newName);
  void applicationHeight(unsigned int newHeight);
  void applicationWidth(unsigned int newWidth);
  void vorpalEngineName(const std::string&& newName);

private:
  Settings();
  static Settings* s_settings;

  unsigned int m_width, m_height;
  std::string m_applicationName, m_vorpalEngineName;
};
}
}

#endif //VORPAL_SETTINGS_HPP