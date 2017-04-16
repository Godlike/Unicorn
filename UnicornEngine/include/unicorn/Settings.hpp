/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SETTINGS_HPP
#define UNICORN_SETTINGS_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/utility/templates/Singleton.hpp>

#include <string>
#include <cstdint>

namespace unicorn
{

/** @brief  Provides an access to application settings */
class Settings : public utility::templates::Singleton<Settings>
{
public:
    /** @brief  Initializes logging system
     *
     *  @param  argc        argument count
     *  @param  argv        arguments
     *  @param  logFileName name of the log file
     */
    UNICORN_EXPORT void Init(int argc, char* argv[], const std::string& logFileName);

    /** @brief  Returns application width
     *
     *  @return width
     */
    UNICORN_EXPORT uint32_t GetApplicationWidth() const { return m_width; }

    /** @brief  Returns application height
     *
     *  @return height
     */
    UNICORN_EXPORT uint32_t GetApplicationHeight() const { return m_height; }

    /** @brief  Sets application width
     *
     *  @param  width   new width
     */
    UNICORN_EXPORT void SetApplicationWidth(uint32_t width) { m_width = width; }

    /** @brief  Sets application height
     *
     *  @param  height   new height
     */
    UNICORN_EXPORT void SetApplicationHeight(uint32_t height) { m_height = height; }

    /** @brief  Returns application name
     *
     *  @return application name
     */
    UNICORN_EXPORT const std::string& GetApplicationName() const { return m_applicationName; }

    /** @brief  Returns engine name
     *
     *  @return engine name
     */
    UNICORN_EXPORT const std::string& GetUnicornEngineName() const { return m_unicornEngineName; }

    /** @brief  Sets an application name
     *
     *  @param  name new name
     */
    UNICORN_EXPORT void SetApplicationName(const std::string&& name) { m_applicationName = name; }

    /** @brief  Sets an engine name
     *
     *  @param  name new name
     */
    UNICORN_EXPORT void SetUnicornEngineName(const std::string&& name) { m_unicornEngineName = name; }
private:
    friend class utility::templates::Singleton<Settings>;

    /** @brief  Constructs an object with default settings */
    UNICORN_EXPORT Settings();
    ~Settings() = default;

    Settings(const Settings& other) = delete;
    Settings& operator=(const Settings& other) = delete;

    Settings(Settings&& other) = delete;
    Settings& operator=(Settings&& other) = delete;

    //! Application width
    uint32_t m_width;

    //! Application height
    uint32_t m_height;

    //! Application name
    std::string m_applicationName;

    //! Engine name
    std::string m_unicornEngineName;
};
}

#endif // UNICORN_SETTINGS_HPP
