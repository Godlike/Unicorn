/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <vector>

namespace unicorn
{

namespace system
{
	class Manager;
	class Window;
}

namespace video
{
class Renderer
{
public:
    Renderer(system::Manager& manager,
        system::Window* pWindow) : m_systemManager(manager), m_pWindow(pWindow), m_isInitialized(false) 
	{};

    virtual ~Renderer() {};

    Renderer(const Renderer& other) = delete;
    Renderer(const Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(const Renderer&& other) = delete;

	virtual bool Init() = 0;
	virtual void Deinit() = 0;
	virtual bool Render() = 0;
    virtual bool RecreateSwapChain() = 0;
protected:
    bool m_isInitialized;

    //! Reference to window manager manager
    system::Manager& m_systemManager;
    //! Pointer to associated window
    system::Window* m_pWindow;
    // Callbacks for window events
	virtual void OnWindowDestroyed(system::Window* pWindow) = 0;
	virtual void OnWindowSizeChanged(system::Window* pWindow, std::pair<int32_t, int32_t> size) = 0;
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
