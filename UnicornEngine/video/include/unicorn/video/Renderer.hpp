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

namespace unicorn
{

namespace system
{
	class Manager;
	class Window;
}

namespace video
{
class Camera;
class Renderer
{
public:
	Renderer(system::Manager& manager, system::Window* window);

	virtual ~Renderer() {};

	Renderer(const Renderer& other) = delete;
	Renderer(const Renderer&& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(const Renderer&& other) = delete;

	virtual bool Init() = 0;
	virtual void Deinit() = 0;
	virtual bool Render() = 0;
	virtual std::shared_ptr<geometry::Mesh> SpawnMesh() = 0;
protected:
	bool m_isInitialized;

	//! Reference to window manager manager
	system::Manager& m_systemManager;
	//! Pointer to associated window
	system::Window* m_pWindow;
	//! Main view camera
	Camera* m_pCamera;
	//! Array of renderer meshes
	std::vector<std::shared_ptr<geometry::Mesh>> m_meshes;
	// Callbacks for window events
	void OnWindowDestroyed(system::Window* pWindow);
	void OnWindowSizeChanged(system::Window* pWindow, std::pair<std::int32_t, std::int32_t> size);
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
