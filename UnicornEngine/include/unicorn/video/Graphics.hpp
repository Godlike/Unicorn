/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GRAPHICS_HPP
#define UNICORN_VIDEO_GRAPHICS_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <cstdint>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace unicorn
{

namespace WindowManager
{
class Hub;
class Monitor;
class Window;
}

namespace video
{
class Renderer;

/** @brief Abstract graphics renderer
 *
 *  Initializes graphics system and proxies render calls to the renderer
 */
class Graphics
{
public:
    /** @brief Constructs an empty graphics system */
    Graphics(WindowManager::Hub& windowManagerHub);

    /** @brief  Destructs a graphics system
     *
     *  Also calls Deinit() to release all resources
     *
     *  @sa Deinit()
     */
    ~Graphics();

    Graphics(const Graphics& other) = delete;
    Graphics(const Graphics&& other) = delete;
    Graphics& operator=(const Renderer& other) = delete;
    Graphics& operator=(const Graphics&& other) = delete;

    /** @brief  Initializes the graphics system
     *
     *  Initializes @ref m_pRenderer
     *
     *  @return @c true if initialization was successful, @c false otherwise
     */
    bool Init();

    /** @brief  Deinitializes the graphics system
     *
     *  Deinitializes @ref m_pRenderer
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
    UNICORN_EXPORT WindowManager::Window* SpawnWindow(int32_t width,
        int32_t height,
        const std::string& name,
        WindowManager::Monitor* pMonitor = nullptr,
        WindowManager::Window* pSharedWindow = nullptr);

    /** @brief  Returns the list of known monitors */
    UNICORN_EXPORT const std::vector<WindowManager::Monitor*>& GetMonitors() const;

private:
    typedef std::pair<Renderer*, WindowManager::Window*> RendererWindowPair;

    struct RendererWindowPairHash
    {
        std::size_t operator()(const RendererWindowPair& pair) const
        {
            return std::hash<Renderer*>()(pair.first) ^
                std::hash<WindowManager::Window*>()(pair.second);
        }
    };

    typedef std::unordered_set<RendererWindowPair, RendererWindowPairHash> RendererWindowPairSet;

    void ProcessExpiredRenderers();

    //! Flag describing if graphics were initialized
    bool m_isInitialized;

    //! Reference to window and monitor managing hub
    WindowManager::Hub& m_windowManagerHub;

    //! Set of renderer-window pairs
    RendererWindowPairSet m_renderers;

    //! Set of expired renderer-window pairs that need to be deinitialized
    RendererWindowPairSet m_expiredRenderers;
};
}
}

#endif // UNICORN_VIDEO_GRAPHICS_HPP
