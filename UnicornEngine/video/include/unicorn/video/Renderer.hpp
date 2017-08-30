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
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    virtual bool Render() = 0;

    /**
     * @brief Returns reference to main camera
     * @return reference to main camera
     */
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
    UNICORN_EXPORT virtual void SetDepthTest(bool enabled) = 0;

    /** @brief  Creates new geometry mesh
    *
    *  Creates and subscribes to mesh.
    *  Mesh shall be deleted via DeleteMesh().
    *  
    *  @param [in] material describes mesh visual representation
    *  
    *  @attention  Mesh lifetime is bound by its renderer's lifetime.
    *              Using meshes after their renderer was destroyed is undefined behaviour.
    *              If you're storing mesh pointers, consider storing them with a reference
    *              to their renderer and listening for its Destroyed event for proper cleanup.
    *
    *  @return pointer to newly created geometry::Mesh
    *
    *  @sa DeleteMesh
    */
    UNICORN_EXPORT virtual Mesh* SpawnMesh(Material const& material) = 0;

    /**
    * @brief Removes mesh from rendering system
    * @param [in] mesh pointer to mesh
    * @return true if mesh was sucessfully deleted from system and false if not
    */
    UNICORN_EXPORT virtual bool DeleteMesh(Mesh const* mesh) = 0;
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
