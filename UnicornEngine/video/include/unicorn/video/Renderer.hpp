/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <unicorn/video/geometry/Mesh.hpp>
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

    /** @brief  Creates new geometry mesh
     *
     *  Creates and subscribes to mesh.
     *  Mesh shall be deleted via DeleteMesh().
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
    virtual geometry::Mesh* SpawnMesh() = 0;

    /** @brief  Deletes geometry mesh
     *
     *  Checks if given mesh is associated with this renderer and deletes it cleaning up
     *  all associated resources within Renderer.
     *  Does nothing if given mesh is not associated with this renderer.
     *
     *  @return @c true if mesh was deleted, @c false otherwise
     */
    virtual bool DeleteMesh(const geometry::Mesh* pMesh) = 0;

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
