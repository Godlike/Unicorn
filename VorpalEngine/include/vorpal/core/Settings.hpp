#ifndef VORPAL_CORE_SETTINGS_HPP
#define VORPAL_CORE_SETTINGS_HPP

#include <vorpal/utility/SharedMacros.hpp>
#include <vorpal/utility/templates/Singleton.hpp>

#include <string>
#include <cstdint>

namespace vp
{
    namespace core
    {
        class VORPAL_EXPORT Settings : public utility::templates::Singleton<Settings>
        {
        public:
            void Init(int argc, char* argv[], const std::string&& logFileName);

            uint32_t GetApplicationWidth() const { return m_width; }
            uint32_t GetApplicationHeight() const { return m_height; }

            void SetApplicationWidth(uint32_t width) { m_width = width; }
            void SetApplicationHeight(uint32_t height) { m_height = height; }

            const std::string& GetApplicationName() const { return m_applicationName; }
            const std::string& GetVorpalEngineName() const { return m_vorpalEngineName; }

            void SetApplicationName(const std::string&& name) { m_applicationName = name; }
            void SetVorpalEngineName(const std::string&& name) { m_vorpalEngineName = name; }

        private:
            friend class utility::templates::Singleton<Settings>;

            Settings();

            uint32_t m_width;
            uint32_t m_height;

            std::string m_applicationName;
            std::string m_vorpalEngineName;
        };
    }
}

#endif // VORPAL_CORE_SETTINGS_HPP
