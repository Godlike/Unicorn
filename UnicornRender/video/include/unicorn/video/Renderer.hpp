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
#include <list>

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
    /**
     * @brief Constructor
     * @param[in,out] manager Describes required extensions, creates window surface
     * @param[in,out] window output window
     * @param[in] camera main camera
     */
    Renderer(system::Manager& manager, system::Window* window, Camera const& camera);

    virtual ~Renderer();

    Renderer(Renderer const& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    virtual bool Render() = 0;

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
    UNICORN_EXPORT virtual void SetDepthTest(bool enabled) = 0;

    /**
    * @brief Adds mesh to the rendering system
    * @param [in] mesh mesh data
    * @return true if mesh was successfully added to the system
    */
    UNICORN_EXPORT virtual bool AddMesh(Mesh* mesh) = 0;

    /**
    * @brief Removes internal rendering mesh data from rendering system
    *
    * @param [in] pMesh pointer to mesh
    *
    * @return true if data was found and succesfully deleted
    */
    UNICORN_EXPORT virtual bool DeleteMesh(Mesh const* pMesh) = 0;

    //! Main view camera, must never be nullptr
    Camera const* camera;
protected:
    bool m_isInitialized;

    //! Reference to window manager
    system::Manager& m_systemManager;
    //! Pointer to associated window
    system::Window* m_pWindow;
    //! Background filling color
    std::array<float, 4> m_backgroundColor;
    //! Depth test
    bool m_depthTestEnabled;
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
