// 73s studio

#ifndef VORPAL_VORPAL_ENGINE_HPP
#define VORPAL_VORPAL_ENGINE_HPP

#include <vorpal/core/Utility.hpp>
#include <vorpal/video/Graphics.hpp>

namespace vp
{
    class VORPAL_EXPORT VorpalEngine
    {
    public:
        VorpalEngine();
        ~VorpalEngine();

        VorpalEngine(const VorpalEngine& other) = delete;
        VorpalEngine(const VorpalEngine&& other) = delete;
        VorpalEngine& operator=(const VorpalEngine& other) = delete;
        VorpalEngine& operator=(const VorpalEngine&& other) = delete;

        void init();
        void run();
        void deinit();
    private:
        video::Graphics* _pGraphics;
    };
}

#endif /* VORPAL_VORPAL_ENGINE_HPP */
