/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GRAPHICS_HPP
#define UNICORN_VIDEO_GRAPHICS_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/system/WindowHint.hpp>

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

    Graphics(const Graphics& other) = delete;
    Graphics(Graphics&& other) = delete;
    Graphics& operator=(Graphics& other) = delete;
    Graphics& operator=(Graphics&& other) = delete;

    /** @brief  Initializes the graphics system
    *
    *  Ensures that window management system supports Vulkan
    *
    *  @param  driver Driver, which will render our all graphics.
    *
    *  @return @c true if initialization was successful, @c false otherwise
    */
    bool Init(const DriverType& driver);

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
    UNICORN_EXPORT system::Window* SpawnWindow(int32_t width,
                                               int32_t height,
                                               const std::string& name,
                                               system::Monitor* pMonitor = nullptr,
                                               system::Window* pSharedWindow = nullptr);

    /** @brief  Returns a pointer to currently focused window
    *
    *  @return pointer to currently focused window or @c nullptr if there is
    *          no focused window controlled by the program
    */
    UNICORN_EXPORT system::Window* GetFocusedWindow() const;

    /** @brief  Returns the list of known monitors */
    UNICORN_EXPORT const std::vector<system::Monitor*>& GetMonitors() const;

    /** @brief  Returns monitor associated with given @p window
    *
    *  @param  window  window object
    *
    *  @return a pointer to monitor object associated with given @p window
    *          or @c nullptr if something went wrong or the window is not in
    *          fullscreen mode
    */
    UNICORN_EXPORT system::Monitor* GetWindowMonitor(const system::Window& window) const;

    /** @brief  Sets monitor to be used by @p  window for fullscreen mode
    *
    *  @param  window      window
    *  @param  pMonitor    monitor object or @c nullptr for windowed mode
    *  @param  position    desired position as (x, y)
    *  @param  size        desired size as (width, height)
    *  @param  refreshRate desired refresh rate
    */
    UNICORN_EXPORT void SetWindowMonitor(const system::Window& window,
                                         system::Monitor* pMonitor,
                                         std::pair<int32_t, int32_t> position,
                                         std::pair<int32_t, int32_t> size,
                                         int32_t refreshRate) const;

    /** @brief  Sets window creation hints
    *
    *  @param  hint    window creation hint
    *  @param  value   new value
    */
    UNICORN_EXPORT void SetWindowCreationHint(system::WindowHint hint, int32_t value) const;

    /** @brief  Spawn new Vulkan based Renderer, which contain VulkanDevice
    *  @param  window Output window.
    *  @return Pointer to newly created Renderer.
    */
    UNICORN_EXPORT Renderer* SpawnVulkanRenderer(system::Window* window);

    /** @brief Binds renderer to window. */
    UNICORN_EXPORT void BindWindowRenderer(system::Window* pWindow, video::Renderer* pRenderer);

private:
    //! Renderer-Window pair type
    typedef std::pair<Renderer*, system::Window*> RendererWindowPair;

    /** @brief  Helper class generating hash for Renderer-Window pair */
    struct RendererWindowPairHash
    {
        /** @brief  Calculates hash */
        std::size_t operator()(const RendererWindowPair& pair) const
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

    //! Current in use driver type
    DriverType m_driver;
};
}
}

#endif // UNICORN_VIDEO_GRAPHICS_HPP
