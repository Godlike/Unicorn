/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UBER_UNICORN_ENGINE_HPP
#define UBER_UNICORN_ENGINE_HPP

#include <unicorn/utility/SharedMacros.hpp>

namespace uc
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
class UNICORN_EXPORT UnicornEngine
{
public:
    UnicornEngine();
    ~UnicornEngine();

    UnicornEngine(const UnicornEngine& other) = delete;
    UnicornEngine& operator=(const UnicornEngine& other) = delete;

    UnicornEngine(UnicornEngine&& other) = delete;
    UnicornEngine& operator=(UnicornEngine&& other) = delete;

    bool Init();
    void Run();
    void Deinit();

    graphics::SceneGraph* m_pSceneGraph;

private:
    bool m_isInitialized;
    system::Window* m_pWindow;
    graphics::Graphics* m_pGraphics;
    graphics::Camera* m_pCamera;
};
}

#endif // UBER_UNICORN_ENGINE_HPP
