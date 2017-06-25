/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_WINDOW_HPP
#define UNICORN_SYSTEM_WINDOW_HPP

#include <unicorn/system/CursorShape.hpp>
#include <unicorn/system/Monitor.hpp>
#include <unicorn/system/MouseMode.hpp>

#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/system/input/MouseButton.hpp>

#include <unicorn/utility/SharedMacros.hpp>

#include <wink/signal.hpp>
#include <wink/event_queue.hpp>

#include <cstdint>
#include <set>
#include <string>
#include <utility>

namespace unicorn
{
namespace system
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
    Window(uint32_t id,
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

    /** @brief  Returns @c true if window has focus, @c false otherwise */
    UNICORN_EXPORT bool IsFocused() const { return m_focus; }

    /** @brief  Brings window to front and sets input focus */
    UNICORN_EXPORT void Focus();

    /** @brief  Makes window invisible */
    UNICORN_EXPORT void Hide();

    /** @brief  Makes window visible */
    UNICORN_EXPORT void Show();

    /** @brief  Sets a cursor shape for a window
     *
     *  @param  shape   cursor shape
     */
    UNICORN_EXPORT void SetCursorShape(CursorShape shape);

    /** @brief  Returns a mouse mode for a window
     *
     *  @return mouse mode
     */
    UNICORN_EXPORT MouseMode GetMouseMode() const;

    /** @brief  Sets mouse mode for a window
     *
     *  @param  mode    mouse mode
     */
    UNICORN_EXPORT void SetMouseMode(MouseMode mode);

    /** @brief  Returns a set of pressed keys */
    UNICORN_EXPORT const std::set<input::MouseButton>& GetPressedMouseButtons() const { return m_pressedMouseButtons; }

    /** @brief  Returns a set of pressed keys */
    UNICORN_EXPORT const std::set<input::Key>& GetPressedKeys() const { return m_pressedKeys; }

    /** @brief  Returns a handle provided by window manager adapter */
    UNICORN_EXPORT void* GetHandle() const { return m_handle; }

    /** @brief  Clears @ref MouseButton and @ref Keyboard event queues
     *
     *  Also clears @p m_skipMouseButtons and @p m_skipKeys
     */
    void ClearInputEvents();

    /** @brief  Updates input modifier keys */
    void UpdateInputModifiers();

    /** @brief  Emits @ref MouseButton signal for all processed mouse buttons events */
    void TriggerMouseButtonEvents();

    /** @brief  Emits @ref Keyboard signal for all processed keyboard events */
    void TriggerKeyboardEvents();

    /** @brief  Event triggered from destructor before the window is destroyed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     */
    wink::signal< wink::slot<void(Window*)> > Destroyed;

    /** @brief  Event triggered when window position is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# pair of values as (x, y)
     */
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > PositionChanged;

    /** @brief  Event triggered when window size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# pair of values as (width, height)
     */
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > SizeChanged;

    /** @brief  Event triggered when window is going to be closed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     */
    wink::signal< wink::slot<void(Window*)> > Close;

    /** @brief  Event triggered when window content should be refreshed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     */
    wink::signal< wink::slot<void(Window*)> > ContentRefresh;

    /** @brief  Event triggered when window focus is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# boolean flag
     */
    wink::signal< wink::slot<void(Window*, bool)> > Focused;

    /** @brief  Event triggered when window minimized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# boolean flag
     */
    wink::signal< wink::slot<void(Window*, bool)> > Minimized;

    /** @brief  Event triggered when window maximized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# boolean flag
     */
    wink::signal< wink::slot<void(Window*, bool)> > Maximized;

    /** @brief  Event triggered when cursor shape for a window is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# cursor shape
     */
    wink::signal< wink::slot<void(Window*, CursorShape)> > CursorShapeChanged;

    /** @brief  Event triggered when mouse interaction mode of window is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# mouse interaction mode
     */
    wink::signal< wink::slot<void(Window*, MouseMode)> > MouseModeChanged;

    /** @brief  Event triggered when window framebuffer size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# pair of values as (width, height)
     */
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > FramebufferResized;

    /** @name   Input events */
    //! @{

    /** @brief  Mouse button event information */
    struct MouseButtonEvent
    {
        Window* const pWindow;
        input::MouseButton button;
        input::Action action;
        input::Modifier::Mask modifiers;
    };

    /** @brief  Event triggered when window receives mouse button input */
    wink::event_queue<MouseButtonEvent> MouseButton;

    /** @brief  Event triggered when window receives mouse position update
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# pair of values as (x, y)
     */
    wink::signal< wink::slot<void(Window*, std::pair<double, double>)> > MousePosition;

    /** @brief  Event triggered when window receives/loses mouse
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# boolean flag
     */
    wink::signal< wink::slot<void(Window*, bool)> > MouseEnter;

    /** @brief  Event triggered when window receives scroll input
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# pair of values as (x, y)
     */
    wink::signal< wink::slot<void(Window*, std::pair<double, double>)> > Scroll;

    /** @brief  Keyboard event information */
    struct KeyboardEvent
    {
        Window * const pWindow;
        input::Key key;
        uint32_t scancode;
        input::Action action;
        input::Modifier::Mask modifiers;
    };

    /** @brief  Event triggered when window receives keyboard input */
    wink::event_queue<KeyboardEvent> Keyboard;

    /** @brief  Event triggered when window receives unicode input with modifiers
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# unicode character
     *  -# modifiers mask
     */
    wink::signal< wink::slot<void(Window*, uint32_t, input::Modifier::Mask)> > Unicode;

    /** @brief  Event triggered when window receives file drop
     *
     *  Event is emitted with the following signature:
     *  -# window pointer
     *  -# vector of filepaths
     */
    wink::signal< wink::slot<void(Window*, std::vector<std::string>)> > FileDrop;

    //! @}

private:
    /** @brief  Slot invoked when window position is changed
     *
     *  Bound to Adapter::WindowPositionChanged
     *
     *  @param  handle      window handle to be checked against @ref m_handle
     *  @param  position    pair of values as (x, y)
     */
    void OnWindowPositionChanged(void* handle, std::pair<int32_t, int32_t> position);

    /** @brief  Slot invoked when window size is changed
     *
     *  Bound to Adapter::WindowSizeChanged
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  size    pair of values as (width, height)
     */
    void OnWindowSizeChanged(void* handle, std::pair<int32_t, int32_t> size);

    /** @brief  Slot invoked when window is about to be closed
     *
     *  Bound to Adapter::WindowClose
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     */
    void OnWindowClose(void* handle);

    /** @brief  Slot invoked when window content should be refreshed
     *
     *  Bound to Adapter::WindowContentRefresh
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     */
    void OnWindowContentRefresh(void* handle);

    /** @brief  Slot invoked when window focus is changed
     *
     *  Bound to Adapter::WindowFocused
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  flag    boolean flag
     */
    void OnWindowFocused(void* handle, bool flag);

    /** @brief  Slot invoked when window minimized stae is changed
     *
     *  Bound to Adapter::WindowMinimized
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  flag    boolean flag
     */
    void OnWindowMinimized(void* handle, bool flag);

    /** @brief  Slot invoked when window maximized state is changed
     *
     *  Bound to Adapter::WindowMaximized
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  flag    boolean flag
     */
    void OnWindowMaximized(void* handle, bool flag);

    /** @brief  Slot invoked when window framebuffer size is changed
     *
     *  Bound to Adapter::WindowFramebufferResized
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  size    pair of values as (width, height)
     */
    void OnWindowFramebufferResized(void* handle, std::pair<int32_t, int32_t> size);

    /** @brief  Slot invoked when window receives mouse button input
     *
     *  Bound to Adapter::WindowMouseButton
     *
     *  @param  handle      window handle to be checked against @ref m_handle
     *  @param  button      mouse button
     *  @param  action      mouse action (press/release)
     *  @param  modifiers   currently active modifiers
     */
    void OnWindowMouseButton(void* handle, input::MouseButton button, input::Action action, input::Modifier::Mask modifiers);

    /** @brief  Slot invoked when mouse position is changed within a window
     *
     *  Bound to Adapter::WindowMousePosition
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  coords  pair of values as (x, y)
     */
    void OnWindowMousePosition(void* handle, std::pair<double, double> coords);

    /** @brief  Slot invoked when mouse enters or leaves window
     *
     *  Bound to Adapter::WindowMouseEnter
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  entered boolean flag
     */
    void OnWindowMouseEnter(void* handle, bool entered);

    /** @brief  Slot invoked when window receives scroll event
     *
     *  Bound to Adapter::WindowScroll
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  coords  pair of values as (x, y)
     */
    void OnWindowScroll(void* handle, std::pair<double, double> coords);

    /** @brief  Slot invoked when window receives keyboard input
     *
     *  Bound to Adapter::WindowKeyboard
     *
     *  @param  handle      window handle to be checked against @ref m_handle
     *  @param  key         key associated with the event
     *  @param  scancode    scancode for key if @p key is input::Key::Unknown
     *  @param  action      key action (press/release/repeat)
     *  @param  modifiers   currently active modifiers
     */
    void OnWindowKeyboard(void* handle, input::Key key, uint32_t scancode, input::Action action, input::Modifier::Mask modifiers);

    /** @brief  Slot invoked when window receives unicode input
     *
     *  Bound to Adapter::WindowUnicode
     *
     *  @param  handle      window handle to be checked against @ref m_handle
     *  @param  unicode     character code
     *  @param  modifiers   currently active modifiers
     */
    void OnWindowUnicode(void* handle, uint32_t unicode, input::Modifier::Mask modifiers);

    /** @brief  Slot invoked when files are dropped in a window
     *
     *  Bound to Adapter::WindowFileDrop
     *
     *  @param  handle  window handle to be checked against @ref m_handle
     *  @param  paths   vector of file paths
     */
    void OnWindowFileDrop(void* handle, std::vector<std::string> paths);

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

    //! Flag describing if window is focused
    bool m_focus;

    //! A set of pressed buttons
    std::set<input::Key> m_pressedKeys;

    //! A set of polled keys
    std::set<input::Key> m_pollKeys;

    //! A set of pressed buttons
    std::set<input::MouseButton> m_pressedMouseButtons;

    //! A set of polled mouse buttons
    std::set<input::MouseButton> m_pollMouseButtons;

    //! Last reported set of input modifiers
    input::Modifier::Mask m_modifiers;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_SYSTEM_WINDOW_HPP
