/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/Color.hpp>
#include <unicorn/video/Camera.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <array>

namespace unicorn
{
namespace system
{
class Manager;
class Window;
class Timer;
}

namespace video
{
class Model;
class Texture;

class Renderer
{
public:
    Renderer(system::Manager& manager, system::Window* window);

    virtual ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer(const Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(const Renderer&& other) = delete;

    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    virtual bool Render() = 0;
    UNICORN_EXPORT Camera& GetCamera();

    UNICORN_EXPORT void SetBackgroundColor(const glm::vec3& backgroundColor);

    /** @brief  Event triggered from destructor before the renderer is destroyed
     *
     *  Event is emitted with the following signature:
     *  -# renderer pointer
     */
    wink::signal< wink::slot<void(Renderer*)> > Destroyed;

    /**
     * @brief Turns on or off depth test
     * @param enabled if true - depth test is enabled, false - disabled
     */
    virtual void DepthTest(bool enabled) = 0;

    /**
    * @brief Allocates texture on GPU
    * @param texture texture which provides data for allocation
    * @return true if allocation was successful and false if not
    */
    virtual bool AllocateTexture(const Texture& texture) = 0;

    virtual bool AddModel(const Model& model) = 0;
protected:
    bool m_isInitialized;

    //! Reference to window manager
    system::Manager& m_systemManager;
    //! Pointer to associated window
    system::Window* m_pWindow;
    //! Main view camera
    Camera m_camera;
    //! Background filling color
    std::array<float, 4> m_backgroundColor;
    //! Depth test
    bool m_depthEnabled;
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
