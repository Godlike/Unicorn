/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <vector>
#include <cstdint>
#include <memory>

#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/Color.hpp>
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
class Camera;
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
    UNICORN_EXPORT std::shared_ptr<Camera> GetCamera() const;
    virtual std::shared_ptr<geometry::Mesh> SpawnMesh() = 0;
    UNICORN_EXPORT void SetBackgroundColor(const glm::vec3& backgroundColor);

protected:
    bool m_isInitialized;

    //! Reference to window manager manager
    system::Manager& m_systemManager;
    //! Pointer to associated window
    system::Window* m_pWindow;
    //! Main view camera
    std::shared_ptr<Camera> m_camera;
    //! Array of renderer meshes
    std::vector<std::shared_ptr<geometry::Mesh>> m_meshes;
    //! Background filling color
    std::array<float, 4> m_backgroundColor;
    // Callbacks for window events
    void OnWindowDestroyed(system::Window* pWindow);
    void OnWindowSizeChanged(system::Window* pWindow, std::pair<std::int32_t, std::int32_t> size);
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
