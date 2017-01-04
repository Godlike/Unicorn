// 73s studio

#ifndef VORPAL_VORPAL_ENGINE_HPP
#define VORPAL_VORPAL_ENGINE_HPP

#include <string>

#include <vorpal/core/Utility.hpp>
#include <vorpal/video/Graphics.hpp>

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
        static VorpalEngine* instance() {
          if (!s_engine)
            s_engine = new VorpalEngine;
          return s_engine;
        }

        std::string applicationName() const;
        unsigned int applicationHeight() const;
        unsigned int applicationWidth() const;
        std::string vorpalEngineName() const;
    private:
      static VorpalEngine* s_engine;
      unsigned int _width, _height;
      std::string _applicationName;
      video::Graphics* _pGraphics;
    };
}

#endif /* VORPAL_VORPAL_ENGINE_HPP */
