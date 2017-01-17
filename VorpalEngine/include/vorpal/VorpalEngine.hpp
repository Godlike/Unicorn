/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

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
        VorpalEngine& operator=(const VorpalEngine& other) = delete;

        VorpalEngine(VorpalEngine&& other) = delete;
        VorpalEngine& operator=(VorpalEngine&& other) = delete;

        bool Init();
        void Run();
        void Deinit();

    private:
        bool m_isInitialized;
        video::Graphics* m_pGraphics;
    };
}

#endif // VORPAL_ENGINE_HPP
