/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_WINDOW_HPP
#define UNICORN_WINDOW_MANAGER_WINDOW_HPP

#include <unicorn/window_manager/Monitor.hpp>

#include <unicorn/utility/SharedMacros.hpp>

#include <wink/signal.hpp>

#include <cstdint>
#include <string>
#include <utility>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Provides interface to system window
 *
 *  Proxies calls to window manager adapter
 */
class Window
{
public:
    /** @brief  Constructs a window object
     *
     *  @param  id              window id within application
     *  @param  width           desired width
     *  @param  height          desired height
     *  @param  name            desired name
     *  @param  pMonitor        pointer to a monitor where window
     *                          should be created
     *  @param  pSharedWindow   pointer to another window that
     *                          should share all resources with
     *                          created window
     */
    Window(uint32_t m_id,
        int32_t width,
        int32_t height,
        const std::string& name,
        Monitor* pMonitor = nullptr,
        Window* pSharedWindow = nullptr);

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    /** @brief  Destructs a window object */
    ~Window();

    /** @brief  Checks if window should be closed */
    UNICORN_EXPORT bool ShouldClose() const;

    /** @brief  Sets flag if window should be closed
     *
     *  Can be used to override user input when close event was emitted
     *  or just to set the flag if window should be closed
     *
     *  @param  flag    new value
     */
    UNICORN_EXPORT void SetShouldClose(bool flag) const;

    /** @brief  Returns the id of the window */
    UNICORN_EXPORT uint32_t GetId() const { return m_id; }

    /** @brief  Returns the name of the window */
    UNICORN_EXPORT const std::string& GetName() const { return m_name; }

    /** @brief  Sets the name of the window
     *
     *  @param  name    new name of the window
     */
    UNICORN_EXPORT void SetName(const std::string& name);

    /** @brief  Returns the size of the window */
    UNICORN_EXPORT const std::pair<int32_t, int32_t>& GetSize() const { return m_size; }

    /** @brief  Sets window size
     *
     *  @param  size    pair of new values as (width, height)
     */
    UNICORN_EXPORT void SetSize(std::pair<int32_t, int32_t> size);

    /** @brief  Sets window size limits
     *
     *  @param  minSize     pair of lower boundaries as (width, height)
     *  @param  maxSize     pair of higher boundaries as (width, height)
     */
    UNICORN_EXPORT void SetSizeLimits(std::pair<int32_t, int32_t> minSize, std::pair<int32_t, int32_t> maxSize);

    /** @brief  Sets aspect ratio for window
     *
     *  @param  ratio   pair of values as (width, height)
     */
    UNICORN_EXPORT void SetAspectRatio(std::pair<int32_t, int32_t> ratio);

    /** @brief  Returns the position of the window */
    UNICORN_EXPORT const std::pair<int32_t, int32_t>& GetPosition() const { return m_position; }

    /** @brief  Sets window position
     *
     *  @param  position    pair of new values as (x, y)
     */
    UNICORN_EXPORT void SetPosition(std::pair<int32_t, int32_t> position);

    /** @brief  Minimizes window */
    UNICORN_EXPORT void Minimize();

    /** @brief  Restores window from minimize/maximize */
    UNICORN_EXPORT void Restore();

    /** @brief  Maximizes window */
    UNICORN_EXPORT void Maximize();

    /** @brief  Brings window to front and sets input focus */
    UNICORN_EXPORT void Focus();

    /** @brief  Makes window invisible */
    UNICORN_EXPORT void Hide();

    /** @brief  Makes window visible */
    UNICORN_EXPORT void Show();

    /** @brief  Returns a handle provided by window manager adapter */
    UNICORN_EXPORT void* GetHandle() const { return m_handle; }

    //! Signal that is emitted from destructor before the actual window is destroyed
    wink::signal< wink::slot<void(Window*)> > Destroyed;

    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > PositionChanged;
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > SizeChanged;
    wink::signal< wink::slot<void(Window*)> > Close;
    wink::signal< wink::slot<void(Window*)> > ContentRefresh;
    wink::signal< wink::slot<void(Window*, bool)> > Focused;
    wink::signal< wink::slot<void(Window*, bool)> > Minimized;
    wink::signal< wink::slot<void(Window*, bool)> > Maximized;
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > FramebufferResized;

private:
    void OnWindowPositionChanged(void* handle, std::pair<int32_t, int32_t> position);
    void OnWindowSizeChanged(void* handle, std::pair<int32_t, int32_t> size);
    void OnWindowClose(void* handle);
    void OnWindowContentRefresh(void* handle);
    void OnWindowFocused(void* handle, bool flag);
    void OnWindowMinimized(void* handle, bool flag);
    void OnWindowMaximized(void* handle, bool flag);
    void OnWindowFramebufferResized(void* handle, std::pair<int32_t, int32_t> size);

    //! Window id within application
    uint32_t m_id;

    //! Window size in pixels as (width, height)
    std::pair<int32_t, int32_t> m_size;

    //! Window position in pixels as (x, y)
    std::pair<int32_t, int32_t> m_position;

    //! Window title name
    std::string m_name;

    //! Pointer to a monitor associated with the window
    Monitor* m_pMonitor;

    //! Pointer to another window that shares resources with this window
    Window* m_pSharedWindow;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_WINDOW_MANAGER_WINDOW_HPP
