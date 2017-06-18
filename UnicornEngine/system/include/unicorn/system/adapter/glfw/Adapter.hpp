/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_ADAPTER_GLFW_ADAPTER_HPP
#define UNICORN_SYSTEM_ADAPTER_GLFW_ADAPTER_HPP

#include <unicorn/system/CursorShape.hpp>
#include <unicorn/system/MonitorMemento.hpp>
#include <unicorn/system/MouseMode.hpp>
#include <unicorn/system/VideoMode.hpp>
#include <unicorn/system/WindowAttribute.hpp>
#include <unicorn/system/WindowHint.hpp>

#include <unicorn/system/input/Action.hpp>
#include <unicorn/system/input/GamepadState.hpp>
#include <unicorn/system/input/Key.hpp>
#include <unicorn/system/input/Modifier.hpp>
#include <unicorn/system/input/MouseButton.hpp>

#include <wink/signal.hpp>

#include <vulkan/vulkan.hpp>

#include <cmath>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace unicorn
{
namespace system
{

struct GammaRamp;

namespace glfw
{

/** @brief  Proxies calls to glfw */
class Adapter
{
public:
    /** @brief  Initializes glfw subsystem */
    static void Init();

    /** @brief  Deinitializes glfw subsystem */
    static void Deinit();

    /** @brief  Sets window creation hints
     *
     *  @param  hint    window creation hint
     *  @param  value   new value
     */
    static void SetWindowCreationHint(WindowHint hint, int32_t value);

    /** @brief  Resets window creation hints */
    static void ResetWindowHints();

    /** @brief  Creates a window with given properties
     *
     *  @param  width               desired width
     *  @param  height              desired height
     *  @param  name                window name
     *  @param  monitorHandle       monitor handle if fullscreen, @c nullptr
     *                              if windowed
     *  @param  sharedWindowHandle  pointer to a window handle to share
     *                              context and resources with
     *
     *  @return handle to newly created window
     */
    static void* CreateWindow(int32_t width,
        int32_t height,
        const char* name,
        void* monitorHandle,
        void* sharedWindowHandle);

    /** @brief  Destroys a window
     *
     *  @param  handle  window to be destroyed
     */
    static void DestroyWindow(void* handle);

    /** @brief  Returns @c true if vulkan can be used */
    static bool IsVulkanSupported();

    /** @brief  Creates a vulkan surface
     *
     *  @param[in]  instance    vulkan instance
     *  @param[in]  handle      window handle
     *  @param[in]  allocator   vulkan allocator
     *  @param[out] surface     stores created vulkan surface
     *
     *  @return vulkan result flag
     */
    static VkResult CreateVulkanSurface(VkInstance instance,
        void* handle,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface);

    /** @brief  Returns @c true if given window should be closed
     *
     *  @param  handle  window handle
     *
     *  @return @c true if given window should be closed, @c false otherwise
     */
    static bool WindowShouldClose(void* handle);

    /** @brief  Sets the flag indicating if given window should be closed
     *
     *  Can be used to override user input when close event was emitted
     *  or just to set the flag if window should be closed
     *
     *  @param  handle  window handle
     *  @param  flag    new value
     */
    static void SetWindowShouldClose(void* handle, bool flag);

    /** @brief  Gets window position
     *
     *  @param[in]  handle  window handle
     *  @param[out] x       stores x coordinate
     *  @param[out] y       stores y coordinate
     */
    static void GetWindowPosition(void* handle, int* x, int* y);

    /** @brief  Sets window position
     *
     *  @param  handle  window handle
     *  @param  x       x coordinate
     *  @param  y       y coordinate
     */
    static void SetWindowPosition(void* handle, int x, int y);

    /** @brief  Gets window size
     *
     *  @param[in]  handle  window handle
     *  @param[out] width   stores width
     *  @param[out] height  stores height
     */
    static void GetWindowSize(void* handle, int* width, int* height);

    /** @brief  Sets window size
     *
     *  @param  handle  window handle
     *  @param  width   width
     *  @param  height  height
     */
    static void SetWindowSize(void* handle, int width, int height);

    /** @brief  Sets window size limits
     *
     *  @param  handle  window handle
     *  @param  minSize pair of lower boundaries as (width, height)
     *  @param  maxSize pair of higher boundaries as (width, height)
     */
    static void SetWindowSizeLimits(void* handle, std::pair<int32_t, int32_t> minSize, std::pair<int32_t, int32_t> maxSize);

    /** @brief  Sets window aspect ration
     *
     *  @param  handle  window handle
     *  @param  ratio   pair of values as (width, height)
     */
    static void SetWindowAspectRatio(void* handle, std::pair<int32_t, int32_t> ratio);

    /** @brief  Minimizes window
     *
     *  @param  handle  window handle
     */
    static void MinimizeWindow(void* handle);

    /** @brief  Restores window from minimize/maximize
     *
     *  @param  handle  window handle
     */
    static void RestoreWindow(void* handle);

    /** @brief  Maximizes window
     *
     *  @param  handle  window handle
     */
    static void MaximizeWindow(void* handle);

    /** @brief  Brings window to front and sets input focus
     *
     *  @param  handle  window handle
     */
    static void FocusWindow(void* handle);

    /** @brief  Makes window invisible
     *
     *  @param  handle  window handle
     */
    static void HideWindow(void* handle);

    /** @brief  Makes window visible
     *
     *  @param  handle  window handle
     */
    static void ShowWindow(void* handle);

    /** @brief  Sets the name of the window
     *
     *  @param  handle  window handle
     *  @param  name    new name of the window
     */
    static void SetWindowName(void* handle, const char* name);

    /** @brief  Returns monitor associated with given window
     *
     *  @param  handle  window handle
     *
     *  @return a handle to monitor associated with given window or @c nullptr
     *          if something went wrong or window is not in fullscreen mode
     */
    static void* GetWindowMonitor(void* handle);

    /** @brief  Sets monitor to be used by window for fullscreen mode
     *
     *  @param  windowHandle    window handle
     *  @param  monitorHandle   monitor handle or @c nullptr for windowed mode
     *  @param  position        desired position as (x, y)
     *  @param  size            desired size as (width, height)
     *  @param  refreshRate     desired refresh rate
     */
    static void SetWindowMonitor(void* windowHandle,
        void* monitorHandle,
        std::pair<int32_t, int32_t> position,
        std::pair<int32_t, int32_t> size,
        int32_t refreshRate);

    /** @brief  Returns a value for window attribute
     *
     *  @param  handle      window handle
     *  @param  attribute   attribute
     *
     *  @return attribute value
     */
    static int32_t GetWindowAttribute(void* handle, WindowAttribute attribute);

    /** @brief  Sets a cursor shape for a window
     *
     *  @param  handle  window handle
     *  @param  shape   cursor shape
     */
    static void SetWindowCursorShape(void* handle, CursorShape shape);

    /** @brief  Returns a mouse mode for a window
     *
     *  @param  handle  window handle
     *
     *  @return mouse mode
     */
    static MouseMode GetWindowMouseMode(void* handle);

    /** @brief  Sets mouse mode for a window
     *
     *  @param  handle  window handle
     *  @param  mode    mouse mode
     */
    static void SetWindowMouseMode(void* handle, MouseMode mode);

    /** @brief  Checks if window has sticky mouse buttons
     *
     *  @param  handle  window handle
     *
     *  @return @c true if window has sticky mouse buttons enabled,
     *          @c false otherwise
     */
    static bool GetWindowStickyMouse(void* handle);

    /** @brief  Sets mouse button sticky mode for a window
     *
     *  @param  handle  window handle
     *  @param  flag    new value
     */
    static void SetWindowStickyMouse(void* handle, bool flag);

    /** @brief  Checks if window has sticky keys
     *
     *  @param  handle  window handle
     *
     *  @return @c true if window has sticky keys enabled,
     *          @c false otherwise
     */
    static bool GetWindowStickyKeys(void* handle);

    /** @brief  Sets sticky keys mode for a window
     *
     *  @param  handle  window handle
     *  @param  flag    new value
     */
    static void SetWindowStickyKeys(void* handle, bool flag);

    /** @brief  Returns mouse button state
     *
     *  @param  handle  window handle
     *  @param  button  mouse button
     */
    static input::Action GetWindowMouseButton(void* handle, input::MouseButton button);

    /** @brief  Returns key state
     *
     *  @param  handle  window handle
     *  @param  key     key
     */
    static input::Action GetWindowKey(void* handle, input::Key key);

    /** @brief  Returns key scancode
     *
     *  @param  key unicorn key code
     *
     *  @return scancode value
     */
    static uint32_t GetKeyScancode(input::Key key);

    /** @brief  Processes events that are in event queue */
    static void PollEvents();

    /** @brief  Waits for events to be processed and processes them
     *
     *  Puts thread into sleep until at least one event is available in the
     *  event queue or the specified timeout @p timeoutSeconds is reached.
     *
     *  @param  timeoutSeconds  sleep limit
     */
    static void WaitEvents(double timeoutSeconds = NAN);

    /** @brief  Returns a vector of vulkan extensions required by glfw */
    static std::vector<const char*> GetRequiredVulkanExtensions();

    /** @brief  Returns a vector of monitor descriptions */
    static std::vector<MonitorMemento> GetMonitors();

    /** @brief  Returns a monitor description
     *
     *  @param  handle  monitor handle
     *
     *  @return monitor description
     */
    static MonitorMemento GetMonitor(void* handle);

    /** @brief  Returns a primary monitor handle
     *
     *  @return monitor handle
     */
    static void* GetPrimaryMonitor();

    /** @brief  Get currently active video mode for monitor
     *
     *  @param  handle  monitor handle
     *
     *  @return video mode
     */
    static VideoMode GetActiveVideoMode(void* handle);

    /** @brief  Get virtual position of monitor
     *
     *  @param  handle  monitor handle
     *
     *  @return pair of values as (x, y)
     */
    static std::pair<int32_t, int32_t> GetVirtualPosition(void* handle);

    /** @brief  Gets gamma ram for monitor
     *
     *  @param  handle  monitor handle
     *
     *  @return gamma ramp
     */
    static GammaRamp GetGammaRamp(void* handle);

    /** @brief  Sets gamma ramp for monitor
     *
     *  @param  handle      monitor handle
     *  @param  gammaRamp   gammaRamp description
     */
    static void SetGammaRamp(void* handle, const GammaRamp& gammaRamp);

    /** @brief  Gets handles to connected gamepads */
    static std::vector<void*> GetConnectedGamepads();

    /** @brief  Gets name for gamepad
     *
     *  @param  handle  gamepad handle
     *
     *  @return gamepad name
     */
    static const char* GetGamepadName(void* handle);

    /** @brief  Gets axes for gamepad
     *
     *  @param  handle  gamepad handle
     *
     *  @return vector of axis values
     */
    static std::vector<float> GetGamepadAxes(void* handle);

    /** @brief  Gets buttons for gamepad
     *
     *  @param  handle  gamepad handle
     *
     *  @return vector of button states
     */
    static std::vector<bool> GetGamepadButtons(void* handle);

    /** @name   Window events */
    //! @{

    /** @brief  Event triggered when window position is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (x, y)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowPositionChanged;

    /** @brief  Event triggered when window size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (width, height)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowSizeChanged;

    /** @brief  Event triggered when window is going to be closed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     */
    static wink::signal< wink::slot<void(void*)> > WindowClose;

    /** @brief  Event triggered when window content should be refreshed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     */
    static wink::signal< wink::slot<void(void*)> > WindowContentRefresh;

    /** @brief  Event triggered when window focus is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowFocused;

    /** @brief  Event triggered when window minimized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowMinimized;

    /** @brief  Event triggered when window maximized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowMaximized;

    /** @brief  Event triggered when window framebuffer size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (width, height)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowFramebufferResized;

    //! @}

    /** @name   Input events */
    //! @{

    /** @brief  Event triggered when window receives mouse button input
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# mouse button input
     *  -# button action type
     *  -# modifiers mask
     */
    static wink::signal< wink::slot<void(void*, input::MouseButton, input::Action, input::Modifier::Mask)> > WindowMouseButton;

    /** @brief  Event triggered when window receives mouse position update
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (x, y)
     */
    static wink::signal< wink::slot<void(void*, std::pair<double, double>)> > WindowMousePosition;

    /** @brief  Event triggered when window receives/loses mouse
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowMouseEnter;

    /** @brief  Event triggered when window receives scroll input
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (x, y)
     */
    static wink::signal< wink::slot<void(void*, std::pair<double, double>)> > WindowScroll;

    /** @brief  Event triggered when window receives keyboard input
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# key indicator
     *  -# raw scancode value
     *  -# key action type
     *  -# modifiers mask
     */
    static wink::signal< wink::slot<void(void*, input::Key, uint32_t, input::Action, input::Modifier::Mask)> > WindowKeyboard;

    /** @brief  Event triggered when window receives unicode input with modifiers
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# unicode character
     *  -# modifiers mask
     */
    static wink::signal< wink::slot<void(void*, uint32_t, input::Modifier::Mask)> > WindowUnicode;

    /** @brief  Event triggered when window receives file drop
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# vector of filepaths
     */
    static wink::signal< wink::slot<void(void*, std::vector<std::string>)> > WindowFileDrop;

    /** @brief  Event triggered when gamepad state is changed (connected/disconnected)
     *
     *  Event is emitted with the following signature:
     *  -# gamepad handle
     *  -# gamepad state
     */
    static wink::signal< wink::slot<void(void*, input::GamepadState)> > GamepadStateChanged;

    //! @}

    /** @name   Monitor events */
    //! @{

    /** @brief  Event triggered when monitor state is changed (connected/disconnected)
     *
     *  Event is emitted with the following signature:
     *  -# monitor handle
     *  -# monitor state
     */
    static wink::signal< wink::slot<void(void*, MonitorMemento::State)> > MonitorStateChanged;

    //! @}

    /** @name   Value conversion methods between glfw and unicorn */
    //! @{

    /** @brief  Converts WindowManager hint to glfw hint */
    static int ConvertToGlfwHint(WindowHint hint);

    /** @brief  Converts WindowManager attribute to glfw attribute */
    static int ConvertToGlfwAttribute(WindowAttribute attribute);

    /** @brief  Tries to convert WindowManager value to glfw value */
    static int ConvertToGlfwValue(int32_t value);

    /** @brief  Converts mouse button from glfw to unicorn */
    static input::MouseButton ConvertToUnicornMouseButton(int32_t button);

    /** @brief  Converts mouse button from unicorn to glfw */
    static int32_t ConvertToGlfwMouseButton(input::MouseButton button);

    /** @brief  Converts action type from glfw to unicorn */
    static input::Action ConvertToUnicornActionType(int32_t action);

    /** @brief  Converts modifiers from glfw to unicorn */
    static input::Modifier::Mask ConvertToUnicornModifiers(int32_t modifiers);

    /** @brief  Converts key code from glfw to unicorn */
    static input::Key ConvertToUnicornKey(int32_t key);

    /** @brief  Converts key code from unicorn to glfw */
    static int32_t ConvertToGlfwKey(input::Key key);

    //!@}

private:
    /** @brief  Fills in video mode structure */
    static VideoMode PrepareVideoMode(const void* pMode);

    //! Flag indicating if glfw was initialized
    static bool s_isInitialized;
};

}
}
}

#endif /* UNICORN_SYSTEM_ADAPTER_GLFW_ADAPTER_HPP */
