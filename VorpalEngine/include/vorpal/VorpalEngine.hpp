// 73s studio

#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <string>

#include <vorpal/core/Utility.hpp>
#include <vorpal/video/Graphics.hpp>
#include <vorpal/core/Settings.hpp>

namespace vp
{
    class VORPAL_EXPORT VorpalEngine
    {
    public:
        VorpalEngine(std::string appName = "Vorpal Engine Application", unsigned int width = 640, unsigned int height = 480);
        ~VorpalEngine();

        VorpalEngine(const VorpalEngine& other) = delete;
        VorpalEngine(const VorpalEngine&& other) = delete;
        VorpalEngine& operator=(const VorpalEngine& other) = delete;
        VorpalEngine& operator=(const VorpalEngine&& other) = delete;

        void init();
        void run();
        void deinit();

        std::string applicationName() const;
        unsigned int applicationHeight() const;
        unsigned int applicationWidth() const;
        std::string vorpalEngineName() const;
        static void logInfo(const std::string&& information);
        static void logWarning(const std::string&& warningMessage);
        static void logError(const std::string&& error);
    private:
        unsigned int m_width, _height;
        std::string _applicationName;
        video::Graphics* _pGraphics;
    };
}

#endif /* VORPAL_ENGINE_HPP */
