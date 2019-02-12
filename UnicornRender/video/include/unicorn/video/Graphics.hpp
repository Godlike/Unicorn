/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GRAPHICS_HPP
#define UNICORN_VIDEO_GRAPHICS_HPP

#include <unicorn/system/WindowHint.hpp>

#include <wink/signal.hpp>
#include <wink/event_queue.hpp>

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace unicorn
{
namespace system
{
class Manager;
class Monitor;
class Window;
}

namespace video
{
class Renderer;
struct Camera;

enum class DriverType
{
    Vulkan
};

/** @brief Abstract graphics renderer
 *
 *  Initializes graphics system and proxies render calls to the renderer
 */
class Graphics
{
public:
    /** @brief Constructs an empty graphics system */
    Graphics(system::Manager& manager);

    /** @brief  Destructs a graphics system
    *
    *  Also calls Deinit() to release all resources
    *
    *  @sa Deinit()
    */
    ~Graphics();

    Graphics(Graphics const& other) = delete;
    Graphics(Graphics&& other) = delete;
    Graphics& operator=(Graphics& other) = delete;
    Graphics& operator=(Graphics&& other) = delete;

    /** @brief  Initializes the graphics system
    *
    *  Ensures that window management system supports DriverType
    *
    *  @param  driver Driver type
    *
    *  @return @c true if initialization was successful, @c false otherwise
    */
    bool Init(DriverType const& driver);

    /** @brief  Deinitializes the graphics system
    *
    *  Deinitializes all Renderer-Window pairs
    */
    void Deinit();

    /** @brief  Graphics renderer loop */
    bool Render();

    /** @brief  Spawns new window
    *
    *  Also creates renderer context for created window
    *
    *  @param  width           desired width
    *  @param  height          desired height
    *  @param  name            desired name
    *  @param  pMonitor        pointer to a monitor where window
    *                          should be created
    *  @param  pSharedWindow   pointer to another window that should
    *                          share all resources with created window
    *
    *  @return a pointer to newly created window object
    */
    system::Window* SpawnWindow(int32_t width,
                                               int32_t height,
                                               std::string const& name,
                                               system::Monitor* pMonitor = nullptr,
                                               system::Window* pSharedWindow = nullptr);

    /** @brief  Returns a pointer to currently focused window
    *
    *  @return pointer to currently focused window or @c nullptr if there is
    *          no focused window controlled by the program
    */
    system::Window* GetFocusedWindow() const;

    /** @brief Enable or disable depth test
    *   @param enabled Enabled if true and disabled if false
    */
    void SetDepthTest(bool enabled);

    /** @brief  Returns the list of known monitors */
    const std::vector<system::Monitor*>& GetMonitors() const;

    /** @brief  Returns monitor associated with given @p window
    *
    *  @param  window  window object
    *
    *  @return a pointer to monitor object associated with given @p window
    *          or @c nullptr if something went wrong or the window is not in
    *          fullscreen mode
    */
    system::Monitor* GetWindowMonitor(system::Window const& window) const;

    /** @brief  Sets monitor to be used by @p  window for fullscreen mode
    *
    *  @param  window      window
    *  @param  pMonitor    monitor object or @c nullptr for windowed mode
    *  @param  position    desired position as (x, y)
    *  @param  size        desired size as (width, height)
    *  @param  refreshRate desired refresh rate
    */
    void SetWindowMonitor(system::Window const& window,
                                         system::Monitor* pMonitor,
                                         std::pair<int32_t, int32_t> const& position,
                                         std::pair<int32_t, int32_t> const& size,
                                         int32_t refreshRate) const;

    /** @brief  Sets window creation hints
    *
    *  @param  hint    window creation hint
    *  @param  value   new value
    */
    void SetWindowCreationHint(system::WindowHint hint, int32_t value) const;

    /** @brief  Spawn new DriverType based Renderer
    *  @param  window output window
    *  @param  camera main camera
    *  @return Pointer to newly created Renderer, nullptr if any error occured.
    */
    Renderer* SpawnRenderer(system::Window* window, Camera& camera);

    /** @brief Binds renderer to window. */
    void BindWindowRenderer(system::Window* pWindow, Renderer* pRenderer);

    /** @brief  Event triggered every time new window is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created window pointer
     */
    wink::signal< wink::slot<void(system::Window*)> >& WindowCreated;

    /** @brief  Event queue triggered every time new batch of monitors is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created monitor pointer
     */
    wink::event_queue<system::Monitor*>& MonitorCreated;

private:
    //! Renderer-Window pair type
    typedef std::pair<Renderer*, system::Window*> RendererWindowPair;

    /** @brief  Helper class generating hash for Renderer-Window pair */
    struct RendererWindowPairHash
    {
        /** @brief  Calculates hash */
        std::size_t operator()(RendererWindowPair const& pair) const
        {
            return std::hash<Renderer*>()(pair.first) ^
                    std::hash<system::Window*>()(pair.second);
        }
    };

    //! Set of Renderer-Window pairs
    typedef std::unordered_set<RendererWindowPair, RendererWindowPairHash> RendererWindowPairSet;

    /** @brief  Checks if there are expired renderers and destroys them */
    void ProcessExpiredRenderers();

    //! Flag describing if graphics were initialized
    bool m_isInitialized;

    //! Reference to window and monitor manager
    system::Manager& m_systemManager;

    //! Set of Renderer-Window pairs
    RendererWindowPairSet m_renderers;

    //! Set of expired Renderer-Window pairs that need to be deinitialized
    RendererWindowPairSet m_expiredRenderers;

    //! Current driver
    DriverType m_driver;
};
}
}

#endif // UNICORN_VIDEO_GRAPHICS_HPP
