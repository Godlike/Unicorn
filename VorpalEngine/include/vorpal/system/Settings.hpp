/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_SYSTEM_SETTINGS_HPP
#define VORPAL_SYSTEM_SETTINGS_HPP

#include <vorpal/utility/SharedMacros.hpp>
#include <vorpal/utility/templates/Singleton.hpp>

#include <string>
#include <cstdint>
#include <array>

namespace vp
{
namespace system
{
class Settings : public utility::templates::Singleton<Settings>
{
public:
    VORPAL_EXPORT void Init(int argc, char* argv[], const std::string& logFileName);

    VORPAL_EXPORT void SetApplicationName(const std::string&& name) { m_applicationName = name; }
    VORPAL_EXPORT void SetVorpalEngineName(const std::string&& name) { m_vorpalEngineName = name; }
    VORPAL_EXPORT void SetApplicationWidth(uint32_t width) { m_width = width; }
    VORPAL_EXPORT void SetApplicationHeight(uint32_t height) { m_height = height; }
    VORPAL_EXPORT void SetAnisatropicFiltering(uint8_t filterPower) { m_anisatropicFiltering = filterPower; }
    VORPAL_EXPORT void SetApplicationVersion(std::array<uint32_t, 3> appVersion) { m_applicationVersion = appVersion; }
    VORPAL_EXPORT void SetEngineVersion(std::array<uint32_t, 3> engVersion) { m_engineVersion = engVersion; }

    VORPAL_EXPORT uint32_t GetApplicationWidth() const { return m_width; }
    VORPAL_EXPORT uint32_t GetApplicationHeight() const { return m_height; }
    VORPAL_EXPORT const std::string& GetApplicationName() const { return m_applicationName; }
    VORPAL_EXPORT const std::string& GetVorpalEngineName() const { return m_vorpalEngineName; }
    VORPAL_EXPORT uint8_t GetAnisatropicFiltering() const { return m_anisatropicFiltering; }
    VORPAL_EXPORT std::array<uint32_t, 3> GetApplicationVersion() const { return m_applicationVersion; }
    VORPAL_EXPORT std::array<uint32_t, 3> GetEngineVersion() const { return m_engineVersion; }

private:
    friend class utility::templates::Singleton<Settings>;

    VORPAL_EXPORT Settings();
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

#endif // VORPAL_SYSTEM_SETTINGS_HPP
