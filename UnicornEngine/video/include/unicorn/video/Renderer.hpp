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
/**
 * @brief Abstract class for all renderer system
 */
class Renderer
{
public:
    Renderer(system::Manager& manager, system::Window* window);

    virtual ~Renderer();

    Renderer(Renderer const& other) = delete;
    Renderer(Renderer const&& other) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer& operator=(Renderer const&& other) = delete;

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
    wink::signal<wink::slot<void(Renderer*)>> Destroyed;

    /**
     * @brief Turns on or off depth test
     * @param [in] enabled if true - depth test is enabled, false - disabled
     */
    virtual void SetDepthTest(bool enabled) = 0;

    /**
     * @brief Adds mesh to rendering system
     * @param [in] mesh pointer to mesh
     * @return true if mesh was added and false if not
     */
    virtual bool AddMesh(Mesh* mesh) = 0;

    /**
    * @brief Removes mesh from rendering system
    * @param [in] mesh pointer to mesh
    * @return true if mesh was sucessfully deleted from system and false if not
    */
    virtual bool DeleteMesh(Mesh const* mesh) = 0;
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
    bool m_depthTestEnabled;

    //! Array of geometry meshes		
    std::list<Mesh*> m_meshes;
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
