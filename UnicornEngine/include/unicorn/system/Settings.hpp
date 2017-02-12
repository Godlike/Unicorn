/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_SETTINGS_HPP
#define UNICORN_SYSTEM_SETTINGS_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/utility/templates/Singleton.hpp>

#include <string>
#include <cstdint>
#include <array>

namespace uc
{
namespace system
{
class Settings : public utility::templates::Singleton<Settings>
{
public:
    UNICORN_EXPORT void Init(int argc, char* argv[], const std::string& logFileName);

    UNICORN_EXPORT void SetApplicationName(const std::string&& name) { m_applicationName = name; }
    UNICORN_EXPORT void SetVorpalEngineName(const std::string&& name) { m_vorpalEngineName = name; }
    UNICORN_EXPORT void SetApplicationWidth(uint32_t width) { m_width = width; }
    UNICORN_EXPORT void SetApplicationHeight(uint32_t height) { m_height = height; }
    UNICORN_EXPORT void SetAnisatropicFiltering(uint8_t filterPower) { m_anisatropicFiltering = filterPower; }
    UNICORN_EXPORT void SetApplicationVersion(std::array<uint32_t, 3> appVersion) { m_applicationVersion = appVersion; }
    UNICORN_EXPORT void SetEngineVersion(std::array<uint32_t, 3> engVersion) { m_engineVersion = engVersion; }

    UNICORN_EXPORT uint32_t GetApplicationWidth() const { return m_width; }
    UNICORN_EXPORT uint32_t GetApplicationHeight() const { return m_height; }
    UNICORN_EXPORT const std::string& GetApplicationName() const { return m_applicationName; }
    UNICORN_EXPORT const std::string& GetVorpalEngineName() const { return m_vorpalEngineName; }
    UNICORN_EXPORT uint8_t GetAnisatropicFiltering() const { return m_anisatropicFiltering; }
    UNICORN_EXPORT std::array<uint32_t, 3> GetApplicationVersion() const { return m_applicationVersion; }
    UNICORN_EXPORT std::array<uint32_t, 3> GetEngineVersion() const { return m_engineVersion; }

private:
    friend class utility::templates::Singleton<Settings>;

    UNICORN_EXPORT Settings();
    ~Settings() = default;

    Settings(const Settings& other) = delete;
    Settings& operator=(const Settings& other) = delete;

    Settings(Settings&& other) = delete;
    Settings& operator=(Settings&& other) = delete;

    uint32_t m_width;
    uint32_t m_height;

    std::string m_applicationName;
    std::string m_vorpalEngineName;
    uint8_t m_anisatropicFiltering;
    std::array<uint32_t, 3> m_applicationVersion;
    std::array<uint32_t, 3> m_engineVersion;
};
}
}

#endif // UNICORN_SYSTEM_SETTINGS_HPP
