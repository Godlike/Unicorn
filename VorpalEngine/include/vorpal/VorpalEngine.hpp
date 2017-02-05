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
    graphics::Graphics* m_pGraphics;
    graphics::Camera* m_pCamera;
    graphics::SceneGraph* m_pSceneGraph;
};
}

#endif // VORPAL_ENGINE_HPP
