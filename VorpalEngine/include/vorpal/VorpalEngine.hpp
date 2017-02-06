/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_ENGINE_HPP
#define VORPAL_ENGINE_HPP

#include <vorpal/utility/SharedMacros.hpp>

namespace vp
{
namespace graphics
{
class Graphics;
class SceneGraph;
class Camera;
}
namespace system
{
class Window;
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

    graphics::SceneGraph* m_pSceneGraph;

private:
    bool m_isInitialized;
    system::Window* m_pWindow;
    graphics::Graphics* m_pGraphics;
    //graphics::Camera* m_pCamera;
};
}

#endif // VORPAL_ENGINE_HPP
