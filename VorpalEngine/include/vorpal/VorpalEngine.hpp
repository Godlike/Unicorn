#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <vorpal/utility/SharedMacros.hpp>

namespace vp
{
    namespace video
    {
        class Graphics;
    }

    class VORPAL_EXPORT VorpalEngine
    {
    public:
        VorpalEngine();
        ~VorpalEngine();

        VorpalEngine(const VorpalEngine& other) = delete;
        VorpalEngine(const VorpalEngine&& other) = delete;
        VorpalEngine& operator=(const VorpalEngine& other) = delete;
        VorpalEngine& operator=(const VorpalEngine&& other) = delete;

        bool Init();
        void Run();
        void Deinit();

    private:
        bool m_isInitialized;
        video::Graphics* m_pGraphics;
    };
}

#endif // VORPAL_ENGINE_HPP
