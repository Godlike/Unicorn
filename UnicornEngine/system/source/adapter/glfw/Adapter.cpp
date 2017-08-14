/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/adapter/glfw/Adapter.hpp>

#include <unicorn/system/CustomValue.hpp>
#include <unicorn/system/GammaRamp.hpp>
#include <unicorn/system/MonitorMemento.hpp>
#include <unicorn/system/VideoMode.hpp>

#include <unicorn/system/input/Modifier.hpp>

#include <GLFW/glfw3.h>

namespace unicorn
{
namespace system
{
namespace glfw
{

bool operator==(const GLFWvidmode& lhs, const GLFWvidmode& rhs)
{
    return lhs.width == rhs.width
        && lhs.height == rhs.height
        && lhs.redBits == rhs.redBits
        && lhs.greenBits == rhs.greenBits
        && lhs.blueBits == rhs.blueBits
        && lhs.refreshRate == rhs.refreshRate;
}

wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowPositionChanged = {};
wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowSizeChanged = {};
wink::signal< wink::slot<void(void*)> > Adapter::WindowClose = {};
wink::signal< wink::slot<void(void*)> > Adapter::WindowContentRefresh = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowFocused = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowMinimized = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowMaximized = {};
wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowFramebufferResized = {};

wink::signal< wink::slot<void(void*, input::MouseButton, input::Action, input::Modifier::Mask)> > Adapter::WindowMouseButton = {};
wink::signal< wink::slot<void(void*, std::pair<double, double>)> > Adapter::WindowMousePosition = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowMouseEnter = {};
wink::signal< wink::slot<void(void*, std::pair<double, double>)> > Adapter::WindowScroll = {};
wink::signal< wink::slot<void(void*, input::Key, uint32_t, input::Action, input::Modifier::Mask)> > Adapter::WindowKeyboard = {};
wink::signal< wink::slot<void(void*, uint32_t, input::Modifier::Mask)> > Adapter::WindowUnicode = {};
wink::signal< wink::slot<void(void*, std::vector<std::string>)> > Adapter::WindowFileDrop = {};
wink::signal< wink::slot<void(void*, input::GamepadState)> > Adapter::GamepadStateChanged = {};

wink::signal< wink::slot<void(void*, MonitorMemento::State)> > Adapter::MonitorStateChanged = {};

bool Adapter::s_isInitialized = false;

namespace
{

void unicornWindowPositionChanged(GLFWwindow* handle, int x, int y)
{
    Adapter::WindowPositionChanged.emit(
        static_cast<void*>(handle),
        std::pair<int32_t, int32_t>(x, y)
    );
}

void unicornWindowSizeChanged(GLFWwindow* handle, int width, int height)
{
    Adapter::WindowSizeChanged.emit(
        static_cast<void*>(handle),
        std::pair<int32_t, int32_t>(width, height)
    );
}

void unicornWindowClose(GLFWwindow* handle)
{
    Adapter::WindowClose.emit(
        static_cast<void*>(handle)
    );
}

void unicornWindowContentRefresh(GLFWwindow* handle)
{
    Adapter::WindowContentRefresh.emit(
        static_cast<void*>(handle)
    );
}

void unicornWindowFocused(GLFWwindow* handle, int focused)
{
    Adapter::WindowFocused.emit(
        static_cast<void*>(handle),
        GLFW_TRUE == focused ? true : false
    );
}

void unicornWindowMinimized(GLFWwindow* handle, int minimized)
{
    Adapter::WindowMinimized.emit(
        static_cast<void*>(handle),
        GLFW_TRUE == minimized ? true : false
    );
}

void unicornWindowMaximized(GLFWwindow* handle, int maximized)
{
    Adapter::WindowMaximized.emit(
        static_cast<void*>(handle),
        GLFW_TRUE == maximized ? true : false
    );
}

void unicornWindowFramebufferResized(GLFWwindow* handle, int width, int height)
{
    Adapter::WindowFramebufferResized.emit(
        static_cast<void*>(handle),
        std::pair<int32_t, int32_t>(width, height)
    );
}

void unicornWindowMouseButton(GLFWwindow* handle, int button, int action, int modifiers)
{
    const input::MouseButton unicornButton = Adapter::ConvertToUnicornMouseButton(button);
    const input::Action unicornAction = Adapter::ConvertToUnicornActionType(action);

    if (input::MouseButton::Unknown != unicornButton &&
        input::Action::Unknown != unicornAction)
    {
        Adapter::WindowMouseButton.emit(
            static_cast<void*>(handle),
            unicornButton,
            unicornAction,
            Adapter::ConvertToUnicornModifiers(modifiers)
        );
    }
}

void unicornWindowMousePosition(GLFWwindow* handle, double x, double y)
{
    Adapter::WindowMousePosition.emit(
        static_cast<void*>(handle),
        std::pair<double, double>(x, y)
    );
}

void unicornWindowMouseEnter(GLFWwindow* handle, int entered)
{
    Adapter::WindowMouseEnter.emit(
        static_cast<void*>(handle),
        GLFW_TRUE == entered ? true : false
    );
}

void unicornWindowScroll(GLFWwindow* handle, double x, double y)
{
    Adapter::WindowScroll.emit(
        static_cast<void*>(handle),
        std::pair<double, double>(x, y)
    );
}

void unicornWindowKeyboard(GLFWwindow* handle, int key, int scancode, int action, int modifiers)
{
    const input::Key unicornKey = Adapter::ConvertToUnicornKey(key);
    const input::Action unicornAction = Adapter::ConvertToUnicornActionType(action);

    if (input::Key::Unknown != unicornKey &&
        input::Action::Unknown != unicornAction)
    {
        Adapter::WindowKeyboard.emit(
            static_cast<void*>(handle),
            unicornKey,
            static_cast<uint32_t>(scancode),
            unicornAction,
            Adapter::ConvertToUnicornModifiers(modifiers)
        );
    }
}

void unicornWindowUnicode(GLFWwindow* handle, uint32_t unicode)
{
    Adapter::WindowUnicode.emit(
        static_cast<void*>(handle),
        unicode,
        Adapter::ConvertToUnicornModifiers(0)
    );
}

void unicornWindowUnicodeModifiers(GLFWwindow* handle, uint32_t unicode, int modifiers)
{
    Adapter::WindowUnicode.emit(
        static_cast<void*>(handle),
        unicode,
        Adapter::ConvertToUnicornModifiers(modifiers)
    );
}

void unicornWindowFileDrop(GLFWwindow* handle, int count, const char** rawPaths)
{
    std::vector<std::string> paths(rawPaths, rawPaths + count);

    Adapter::WindowFileDrop.emit(
        static_cast<void*>(handle),
        paths
    );
}

void unicornGamepadStateChanged(int gamepad, int event)
{
    Adapter::GamepadStateChanged.emit(
        reinterpret_cast<void*>(static_cast<intptr_t>(gamepad)),
        GLFW_CONNECTED == event ? input::GamepadState::Connected : input::GamepadState::Disconnected
    );
}

void unicornMonitorStateChanged(GLFWmonitor* handle, int event)
{
    Adapter::MonitorStateChanged.emit(
        static_cast<void*>(handle),
        GLFW_CONNECTED == event ? MonitorMemento::State::Connected : MonitorMemento::State::Disconnected
    );
}

static GLFWcursor* s_pArrowCursor = nullptr;
static GLFWcursor* s_pIBeamCursor = nullptr;
static GLFWcursor* s_pCrosshairCursor = nullptr;
static GLFWcursor* s_pHandCursor = nullptr;
static GLFWcursor* s_pHResizeCursor = nullptr;
static GLFWcursor* s_pVResizeCursor = nullptr;

}

void Adapter::Init()
{
    if (!s_isInitialized)
    {
        glfwInit();

        glfwSetMonitorCallback(unicornMonitorStateChanged);
        glfwSetJoystickCallback(unicornGamepadStateChanged);

        s_isInitialized = true;
    }
}

void Adapter::Deinit()
{
    if (s_isInitialized)
    {
        glfwSetMonitorCallback(NULL);
        glfwSetJoystickCallback(NULL);

        if (s_pArrowCursor)
        {
            glfwDestroyCursor(s_pArrowCursor);
            s_pArrowCursor = nullptr;
        }

        if (s_pIBeamCursor)
        {
            glfwDestroyCursor(s_pIBeamCursor);
            s_pIBeamCursor = nullptr;
        }

        if (s_pCrosshairCursor)
        {
            glfwDestroyCursor(s_pCrosshairCursor);
            s_pCrosshairCursor = nullptr;
        }

        if (s_pHandCursor)
        {
            glfwDestroyCursor(s_pHandCursor);
            s_pHandCursor = nullptr;
        }

        if (s_pHResizeCursor)
        {
            glfwDestroyCursor(s_pHResizeCursor);
            s_pHResizeCursor = nullptr;
        }

        if (s_pVResizeCursor)
        {
            glfwDestroyCursor(s_pVResizeCursor);
            s_pVResizeCursor = nullptr;
        }

        glfwTerminate();

        WindowPositionChanged.clear();
        WindowSizeChanged.clear();
        WindowClose.clear();
        WindowContentRefresh.clear();
        WindowFocused.clear();
        WindowMinimized.clear();
        WindowMaximized.clear();
        WindowFramebufferResized.clear();

        WindowMouseButton.clear();
        WindowMousePosition.clear();
        WindowMouseEnter.clear();
        WindowScroll.clear();
        WindowKeyboard.clear();
        WindowUnicode.clear();
        WindowUnicode.clear();
        WindowFileDrop.clear();
        GamepadStateChanged.clear();

        MonitorStateChanged.clear();

        s_isInitialized = false;
    }
}

void Adapter::SetWindowCreationHint(WindowHint hint, int32_t value)
{
    glfwWindowHint(ConvertToGlfwHint(hint), ConvertToGlfwValue(value));
}

void Adapter::ResetWindowHints()
{
    glfwDefaultWindowHints();
}

void* Adapter::CreateWindow(int32_t width,
    int32_t height,
    const char* name,
    void* monitorHandle,
    void* sharedWindowHandle)
{
    GLFWwindow* handle = glfwCreateWindow(width,
        height,
        name,
        static_cast<GLFWmonitor*>(monitorHandle),
        static_cast<GLFWwindow*>(sharedWindowHandle) );

    glfwSetWindowPosCallback(handle, unicornWindowPositionChanged);
    glfwSetWindowSizeCallback(handle, unicornWindowSizeChanged);
    glfwSetWindowCloseCallback(handle, unicornWindowClose);
    glfwSetWindowRefreshCallback(handle, unicornWindowContentRefresh);
    glfwSetWindowFocusCallback(handle, unicornWindowFocused);
    glfwSetWindowIconifyCallback(handle, unicornWindowMinimized);
    glfwSetWindowMaximizeCallback(handle, unicornWindowMaximized);
    glfwSetFramebufferSizeCallback(handle, unicornWindowFramebufferResized);

    glfwSetMouseButtonCallback(handle, unicornWindowMouseButton);
    glfwSetCursorPosCallback(handle, unicornWindowMousePosition);
    glfwSetCursorEnterCallback(handle, unicornWindowMouseEnter);
    glfwSetScrollCallback(handle, unicornWindowScroll);
    glfwSetKeyCallback(handle, unicornWindowKeyboard);
    glfwSetCharCallback(handle, unicornWindowUnicode);
    glfwSetCharModsCallback(handle, unicornWindowUnicodeModifiers);
    glfwSetDropCallback(handle, unicornWindowFileDrop);

    return static_cast<void*>(handle);
}

void Adapter::DestroyWindow(void* handle)
{
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(handle), GLFW_TRUE);
    glfwDestroyWindow(static_cast<GLFWwindow*>(handle));
}

bool Adapter::IsVulkanSupported()
{
    return glfwVulkanSupported();
}

VkResult Adapter::CreateVulkanSurface(VkInstance instance,
    void* handle,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return glfwCreateWindowSurface(instance
        , static_cast<GLFWwindow*>(handle)
        , allocator
        , surface
    );
}

bool Adapter::WindowShouldClose(void* handle)
{
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(handle));
}

void Adapter::SetWindowShouldClose(void* handle, bool flag)
{
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(handle), flag ? GLFW_TRUE : GLFW_FALSE);
}

void Adapter::GetWindowPosition(void* handle, int* x, int* y)
{
    glfwGetWindowPos(static_cast<GLFWwindow*>(handle), x, y);
}

void Adapter::SetWindowPosition(void* handle, int x, int y)
{
    glfwSetWindowPos(static_cast<GLFWwindow*>(handle), x, y);
}

void Adapter::GetWindowSize(void* handle, int* x, int* y)
{
    glfwGetWindowSize(static_cast<GLFWwindow*>(handle), x, y);
}

void Adapter::SetWindowSize(void* handle, int x, int y)
{
    glfwSetWindowSize(static_cast<GLFWwindow*>(handle), x, y);
}

void Adapter::SetWindowSizeLimits(void* handle, std::pair<int32_t, int32_t> minSize, std::pair<int32_t, int32_t> maxSize)
{
    glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(handle), minSize.first, minSize.second, maxSize.first, maxSize.second);
}

void Adapter::SetWindowAspectRatio(void* handle, std::pair<int32_t, int32_t> ratio)
{
    glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(handle), ratio.first, ratio.second);
}

void Adapter::MinimizeWindow(void* handle)
{
    glfwIconifyWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::RestoreWindow(void* handle)
{
    glfwRestoreWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::MaximizeWindow(void* handle)
{
    glfwMaximizeWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::FocusWindow(void* handle)
{
    glfwFocusWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::HideWindow(void* handle)
{
    glfwHideWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::ShowWindow(void* handle)
{
    glfwShowWindow(static_cast<GLFWwindow*>(handle));
}

void Adapter::SetWindowName(void* handle, const char* name)
{
    glfwSetWindowTitle(static_cast<GLFWwindow*>(handle), name);
}

void* Adapter::GetWindowMonitor(void* handle)
{
    return glfwGetWindowMonitor(static_cast<GLFWwindow*>(handle));
}

void Adapter::SetWindowMonitor(void* windowHandle,
    void* monitorHandle,
    std::pair<int32_t, int32_t> position,
    std::pair<int32_t, int32_t> size,
    int32_t refreshRate)
{
    glfwSetWindowMonitor(static_cast<GLFWwindow*>(windowHandle),
        static_cast<GLFWmonitor*>(monitorHandle),
        position.first,
        position.second,
        size.first,
        size.second,
        refreshRate);
}

int32_t Adapter::GetWindowAttribute(void* handle, WindowAttribute attribute)
{
    return glfwGetWindowAttrib(static_cast<GLFWwindow*>(handle), ConvertToGlfwAttribute(attribute));
}

void Adapter::SetWindowCursorShape(void* handle, CursorShape shape)
{
    GLFWcursor* cursor = nullptr;

    switch (shape)
    {
        case CursorShape::Arrow:
        {
            if (!s_pArrowCursor)
            {
                s_pArrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            }

            cursor = s_pArrowCursor;

            break;
        }
        case CursorShape::TextBeam:
        {
            if (!s_pIBeamCursor)
            {
                s_pIBeamCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
            }

            cursor = s_pIBeamCursor;

            break;
        }
        case CursorShape::Crosshair:
        {
            if (!s_pCrosshairCursor)
            {
                s_pCrosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
            }

            cursor = s_pCrosshairCursor;

            break;
        }
        case CursorShape::Hand:
        {
            if (!s_pHandCursor)
            {
                s_pHandCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
            }

            cursor = s_pHandCursor;

            break;
        }
        case CursorShape::HorizontalResize:
        {
            if (!s_pHResizeCursor)
            {
                s_pHResizeCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
            }

            cursor = s_pHResizeCursor;

            break;
        }
        case CursorShape::VerticalResize:
        {
            if (!s_pVResizeCursor)
            {
                s_pVResizeCursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
            }

            cursor = s_pVResizeCursor;

            break;
        }
        default:
        {
            cursor = nullptr;
            break;
        }
    }

    glfwSetCursor(static_cast<GLFWwindow*>(handle), cursor);
}

MouseMode Adapter::GetWindowMouseMode(void* handle)
{
    MouseMode result = MouseMode::Normal;

    switch (glfwGetInputMode(static_cast<GLFWwindow*>(handle), GLFW_CURSOR))
    {
        case GLFW_CURSOR_HIDDEN:
        {
            result = MouseMode::Hidden;
            break;
        }
        case GLFW_CURSOR_DISABLED:
        {
            result = MouseMode::Captured;
            break;
        }
        default:
        {
            break;
        }
    }

    return result;
}

void Adapter::SetWindowMouseMode(void* handle, MouseMode mode)
{
    int value = GLFW_CURSOR_NORMAL;

    switch (mode)
    {
        case MouseMode::Hidden:
        {
            value = GLFW_CURSOR_HIDDEN;
            break;
        }
        case MouseMode::Captured:
        {
            value = GLFW_CURSOR_DISABLED;
            break;
        }
        default:
        {
            break;
        }
    }

    glfwSetInputMode(static_cast<GLFWwindow*>(handle), GLFW_CURSOR, value);
}

bool Adapter::GetWindowStickyMouse(void* handle)
{
    return glfwGetInputMode(static_cast<GLFWwindow*>(handle), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

void Adapter::SetWindowStickyMouse(void* handle, bool flag)
{
    glfwSetInputMode(static_cast<GLFWwindow*>(handle), GLFW_STICKY_MOUSE_BUTTONS, flag ? GLFW_TRUE : GLFW_FALSE);
}

bool Adapter::GetWindowStickyKeys(void* handle)
{
    return glfwGetInputMode(static_cast<GLFWwindow*>(handle), GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void Adapter::SetWindowStickyKeys(void* handle, bool flag)
{
    glfwSetInputMode(static_cast<GLFWwindow*>(handle), GLFW_STICKY_KEYS, flag ? GLFW_TRUE : GLFW_FALSE);
}

input::Action Adapter::GetWindowMouseButton(void* handle, input::MouseButton button)
{
    return ConvertToUnicornActionType(glfwGetMouseButton(static_cast<GLFWwindow*>(handle), ConvertToGlfwMouseButton(button)));
}

input::Action Adapter::GetWindowKey(void* handle, input::Key key)
{
    return ConvertToUnicornActionType(glfwGetKey(static_cast<GLFWwindow*>(handle), ConvertToGlfwKey(key)));
}

std::string Adapter::GetClipboard(void* handle)
{
    const char* data = glfwGetClipboardString(static_cast<GLFWwindow*>(handle));

    return nullptr != data ? std::string(data) : std::string();
}

void Adapter::SetClipboard(void* handle, const std::string& data)
{
    return glfwSetClipboardString(static_cast<GLFWwindow*>(handle), data.c_str());
}

uint32_t Adapter::GetKeyScancode(input::Key key)
{
    return static_cast<uint32_t>(glfwGetKeyScancode(ConvertToGlfwKey(key)));
}

void Adapter::PollEvents()
{
    glfwPollEvents();
}

void Adapter::WaitEvents(double timeoutSeconds)
{
    if (std::isnan(timeoutSeconds))
    {
        glfwWaitEvents();
    }
    else
    {
        glfwWaitEventsTimeout(timeoutSeconds);
    }
}

std::vector<const char*> Adapter::GetRequiredVulkanExtensions()
{
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

std::vector<MonitorMemento> Adapter::GetMonitors()
{
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<MonitorMemento> result;
    result.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        result.push_back(GetMonitor(monitors[i]));
    }

    return result;
}

MonitorMemento Adapter::GetMonitor(void* handle)
{
    MonitorMemento result = {};

    const GLFWmonitor* primary = glfwGetPrimaryMonitor();
    GLFWmonitor* monitorHandle = static_cast<GLFWmonitor*>(handle);

    result.name = glfwGetMonitorName(monitorHandle);

    glfwGetMonitorPhysicalSize(monitorHandle, &result.physicalSize.first, &result.physicalSize.second);

    {
        int modeCount = 0;
        const GLFWvidmode* modes = glfwGetVideoModes(monitorHandle, &modeCount);

        result.modes.reserve(modeCount);

        for (int m = 0; m < modeCount; ++m)
        {
            result.modes.push_back(PrepareVideoMode(&modes[m]));
        }
    }

    result.isPrimary = ( monitorHandle == primary );
    result.id = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    result.state = MonitorMemento::State::Connected;
    result.handle = handle;

    return result;
}

void* Adapter::GetPrimaryMonitor()
{
    return glfwGetPrimaryMonitor();
}

VideoMode Adapter::GetActiveVideoMode(void* handle)
{
    return PrepareVideoMode(glfwGetVideoMode(static_cast<GLFWmonitor*>(handle)));
}

std::pair<int32_t, int32_t> Adapter::GetVirtualPosition(void* handle)
{
    std::pair<int32_t, int32_t> result;

    glfwGetMonitorPos(static_cast<GLFWmonitor*>(handle), &result.first, &result.second);

    return result;
}

GammaRamp Adapter::GetGammaRamp(void* handle)
{
    const GLFWgammaramp* ramp = glfwGetGammaRamp(static_cast<GLFWmonitor*>(handle));

    GammaRamp result = {};

    if (ramp)
    {
        result.size = ramp->size;
        result.red = new uint16_t[result.size];
        result.green = new uint16_t[result.size];
        result.blue = new uint16_t[result.size];

        for (uint32_t g = 0; g < result.size; ++g)
        {
            result.red[g] = ramp->red[g];
            result.green[g] = ramp->green[g];
            result.blue[g] = ramp->blue[g];
        }
    }
    else
    {
        result = GammaRamp();
    }

    return result;
}

void Adapter::SetGammaRamp(void* handle, const GammaRamp& gammaRamp)
{
    GLFWgammaramp ramp;

    ramp.size = gammaRamp.size;
    ramp.red = gammaRamp.red;
    ramp.green = gammaRamp.green;
    ramp.blue = gammaRamp.blue;

    glfwSetGammaRamp(static_cast<GLFWmonitor*>(handle), &ramp);
}

std::vector<void*> Adapter::GetConnectedGamepads()
{
    std::vector<void*> result;

    result.reserve(4);

    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i)
    {
        if (GLFW_TRUE == glfwJoystickPresent(i))
        {
            result.push_back(reinterpret_cast<void*>(static_cast<intptr_t>(i)));
        }
    }

    return result;
}

const char* Adapter::GetGamepadName(void* handle)
{
    return glfwGetJoystickName(static_cast<int>(reinterpret_cast<intptr_t>(handle)));
}

std::vector<float> Adapter::GetGamepadAxes(void* handle)
{
    const int gamepad = static_cast<int>(reinterpret_cast<intptr_t>(handle));
    int count = 0;

    const float* axes = glfwGetJoystickAxes(gamepad, &count);

    std::vector<float> result(count);

    for (int i = 0; i < count; ++i)
    {
        result[i] = axes[i];
    }

    return result;
}

std::vector<bool> Adapter::GetGamepadButtons(void* handle)
{
    const int gamepad = static_cast<int>(reinterpret_cast<intptr_t>(handle));
    int count = 0;

    const unsigned char* buttons = glfwGetJoystickButtons(gamepad, &count);

    std::vector<bool> result(count);

    for (int i = 0; i < count; ++i)
    {
        result[i] = (GLFW_PRESS == buttons[i]) ? true : false;
    }

    return result;
}

VideoMode Adapter::PrepareVideoMode(const void* pMode)
{
    VideoMode result = {};

    if (pMode)
    {
        const GLFWvidmode& mode = *static_cast<const GLFWvidmode*>(pMode);
        result.width = mode.width;
        result.height = mode.height;
        result.rgbBitDepth = {{ mode.redBits, mode.greenBits, mode.blueBits }};
        result.refreshRate = mode.refreshRate;
    }

    return result;
}

int Adapter::ConvertToGlfwHint(WindowHint hint)
{
    switch (hint)
    {
        case WindowHint::AutoMinimize:
        {
            return GLFW_AUTO_ICONIFY;
        }
        case WindowHint::Decorated:
        {
            return GLFW_DECORATED;
        }
        case WindowHint::Floating:
        {
            return GLFW_FLOATING;
        }
        case WindowHint::Focused:
        {
            return GLFW_FOCUSED;
        }
        case WindowHint::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case WindowHint::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case WindowHint::Visible:
        {
            return GLFW_VISIBLE;
        }
        case WindowHint::Doublebuffer:
        {
            return GLFW_DOUBLEBUFFER;
        }
        case WindowHint::Stereo:
        {
            return GLFW_STEREO;
        }
        case WindowHint::Samples:
        {
            return GLFW_SAMPLES;
        }
        case WindowHint::RefreshRate:
        {
            return GLFW_REFRESH_RATE;
        }
        case WindowHint::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case WindowHint::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case WindowHint::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case WindowHint::ContextVersionMinor:
        {
            return GLFW_CONTEXT_VERSION_MINOR;
        }
        default:
        {
            return GLFW_INVALID_ENUM;
        }
    }
}

int Adapter::ConvertToGlfwAttribute(WindowAttribute attribute)
{
    switch (attribute)
    {
        case WindowAttribute::Decorated:
        {
            return GLFW_DECORATED;
        }
        case WindowAttribute::Floating:
        {
            return GLFW_FLOATING;
        }
        case WindowAttribute::Focused:
        {
            return GLFW_FOCUSED;
        }
        case WindowAttribute::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case WindowAttribute::Minimized:
        {
            return GLFW_ICONIFIED;
        }
        case WindowAttribute::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case WindowAttribute::Visible:
        {
            return GLFW_VISIBLE;
        }
        case WindowAttribute::Samples:
        {
            return GLFW_SAMPLES;
        }
        case WindowAttribute::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case WindowAttribute::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case WindowAttribute::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case WindowAttribute::ContextVersionMinor:
        {
            return GLFW_CONTEXT_VERSION_MINOR;
        }
        default:
        {
            return GLFW_INVALID_ENUM;
        }
    }
}

int Adapter::ConvertToGlfwValue(int32_t value)
{
    if (0 == (value & CustomValue::Mask))
    {
        return value;
    }

    switch (value)
    {
        case CustomValue::False:
        {
            return GLFW_FALSE;
        }
        case CustomValue::True:
        {
            return GLFW_TRUE;
        }
        case CustomValue::OpenGL_API:
        {
            return GLFW_OPENGL_API;
        }
        case CustomValue::OpenGL_ES_API:
        {
            return GLFW_OPENGL_ES_API;
        }
        case CustomValue::No_API:
        {
            return GLFW_NO_API;
        }
        case CustomValue::NativeContextAPI:
        {
            return GLFW_NATIVE_CONTEXT_API;
        }
        case CustomValue::EGLContextAPI:
        {
            return GLFW_EGL_CONTEXT_API;
        }
        case CustomValue::DontCare:
        {
            return GLFW_DONT_CARE;
        }
        default:
        {
            return value;
        }
    }
}

input::MouseButton Adapter::ConvertToUnicornMouseButton(int32_t button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_1:
        {
            return input::MouseButton::Mouse1;
        }
        case GLFW_MOUSE_BUTTON_2:
        {
            return input::MouseButton::Mouse2;
        }
        case GLFW_MOUSE_BUTTON_3:
        {
            return input::MouseButton::Mouse3;
        }
        case GLFW_MOUSE_BUTTON_4:
        {
            return input::MouseButton::Mouse4;
        }
        case GLFW_MOUSE_BUTTON_5:
        {
            return input::MouseButton::Mouse5;
        }
        case GLFW_MOUSE_BUTTON_6:
        {
            return input::MouseButton::Mouse6;
        }
        case GLFW_MOUSE_BUTTON_7:
        {
            return input::MouseButton::Mouse7;
        }
        case GLFW_MOUSE_BUTTON_8:
        {
            return input::MouseButton::Mouse8;
        }
        default:
        {
            return input::MouseButton::Unknown;
        }
    }
}

int32_t Adapter::ConvertToGlfwMouseButton(input::MouseButton button)
{
    switch (button)
    {
        case input::MouseButton::Mouse1:
        {
            return GLFW_MOUSE_BUTTON_1;
        }
        case input::MouseButton::Mouse2:
        {
            return GLFW_MOUSE_BUTTON_2;
        }
        case input::MouseButton::Mouse3:
        {
            return GLFW_MOUSE_BUTTON_3;
        }
        case input::MouseButton::Mouse4:
        {
            return GLFW_MOUSE_BUTTON_4;
        }
        case input::MouseButton::Mouse5:
        {
            return GLFW_MOUSE_BUTTON_5;
        }
        case input::MouseButton::Mouse6:
        {
            return GLFW_MOUSE_BUTTON_6;
        }
        case input::MouseButton::Mouse7:
        {
            return GLFW_MOUSE_BUTTON_7;
        }
        case input::MouseButton::Mouse8:
        {
            return GLFW_MOUSE_BUTTON_8;
        }
        default:
        {
            return GLFW_KEY_UNKNOWN;
        }
    }
}

input::Action Adapter::ConvertToUnicornActionType(int32_t action)
{
    switch (action)
    {
        case GLFW_RELEASE:
        {
            return input::Action::Release;
        }
        case GLFW_PRESS:
        {
            return input::Action::Press;
        }
        case GLFW_REPEAT:
        {
            return input::Action::Repeat;
        }
        default:
        {
            return input::Action::Unknown;
        }
    }
}

input::Modifier::Mask Adapter::ConvertToUnicornModifiers(int32_t modifiers)
{
    input::Modifier::Mask result = input::Modifier::None;

    if (modifiers & GLFW_MOD_SHIFT)
    {
        result |= input::Modifier::Shift;
    }

    if (modifiers & GLFW_MOD_CONTROL)
    {
        result |= input::Modifier::Ctrl;
    }

    if (modifiers & GLFW_MOD_ALT)
    {
        result |= input::Modifier::Alt;
    }

    if (modifiers & GLFW_MOD_SUPER)
    {
        result |= input::Modifier::Super;
    }

    if (modifiers & ~(GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT |GLFW_MOD_SUPER))
    {
        result |= input::Modifier::Unknown;
    }

    return modifiers;
}

input::Key Adapter::ConvertToUnicornKey(int32_t key)
{
    switch (key)
    {
        case GLFW_KEY_UNKNOWN:
        {
            return input::Key::Unrecognized;
        }
        case GLFW_KEY_SPACE:
        {
            return input::Key::Space;
        }
        case GLFW_KEY_APOSTROPHE:
        {
            return input::Key::Apostrophe;
        }
        case GLFW_KEY_COMMA:
        {
            return input::Key::Comma;
        }
        case GLFW_KEY_MINUS:
        {
            return input::Key::Minus;
        }
        case GLFW_KEY_PERIOD:
        {
            return input::Key::Period;
        }
        case GLFW_KEY_SLASH:
        {
            return input::Key::Slash;
        }
        case GLFW_KEY_0:
        {
            return input::Key::Normal_0;
        }
        case GLFW_KEY_1:
        {
            return input::Key::Normal_1;
        }
        case GLFW_KEY_2:
        {
            return input::Key::Normal_2;
        }
        case GLFW_KEY_3:
        {
            return input::Key::Normal_3;
        }
        case GLFW_KEY_4:
        {
            return input::Key::Normal_4;
        }
        case GLFW_KEY_5:
        {
            return input::Key::Normal_5;
        }
        case GLFW_KEY_6:
        {
            return input::Key::Normal_6;
        }
        case GLFW_KEY_7:
        {
            return input::Key::Normal_7;
        }
        case GLFW_KEY_8:
        {
            return input::Key::Normal_8;
        }
        case GLFW_KEY_9:
        {
            return input::Key::Normal_9;
        }
        case GLFW_KEY_SEMICOLON:
        {
            return input::Key::Semicolon;
        }
        case GLFW_KEY_EQUAL:
        {
            return input::Key::Equal;
        }
        case GLFW_KEY_A:
        {
            return input::Key::A;
        }
        case GLFW_KEY_B:
        {
            return input::Key::B;
        }
        case GLFW_KEY_C:
        {
            return input::Key::C;
        }
        case GLFW_KEY_D:
        {
            return input::Key::D;
        }
        case GLFW_KEY_E:
        {
            return input::Key::E;
        }
        case GLFW_KEY_F:
        {
            return input::Key::F;
        }
        case GLFW_KEY_G:
        {
            return input::Key::G;
        }
        case GLFW_KEY_H:
        {
            return input::Key::H;
        }
        case GLFW_KEY_I:
        {
            return input::Key::I;
        }
        case GLFW_KEY_J:
        {
            return input::Key::J;
        }
        case GLFW_KEY_K:
        {
            return input::Key::K;
        }
        case GLFW_KEY_L:
        {
            return input::Key::L;
        }
        case GLFW_KEY_M:
        {
            return input::Key::M;
        }
        case GLFW_KEY_N:
        {
            return input::Key::N;
        }
        case GLFW_KEY_O:
        {
            return input::Key::O;
        }
        case GLFW_KEY_P:
        {
            return input::Key::P;
        }
        case GLFW_KEY_Q:
        {
            return input::Key::Q;
        }
        case GLFW_KEY_R:
        {
            return input::Key::R;
        }
        case GLFW_KEY_S:
        {
            return input::Key::S;
        }
        case GLFW_KEY_T:
        {
            return input::Key::T;
        }
        case GLFW_KEY_U:
        {
            return input::Key::U;
        }
        case GLFW_KEY_V:
        {
            return input::Key::V;
        }
        case GLFW_KEY_W:
        {
            return input::Key::W;
        }
        case GLFW_KEY_X:
        {
            return input::Key::X;
        }
        case GLFW_KEY_Y:
        {
            return input::Key::Y;
        }
        case GLFW_KEY_Z:
        {
            return input::Key::Z;
        }
        case GLFW_KEY_LEFT_BRACKET:
        {
            return input::Key::LeftBracket;
        }
        case GLFW_KEY_BACKSLASH:
        {
            return input::Key::Backslash;
        }
        case GLFW_KEY_RIGHT_BRACKET:
        {
            return input::Key::RightBracket;
        }
        case GLFW_KEY_GRAVE_ACCENT:
        {
            return input::Key::GraveAccent;
        }
        case GLFW_KEY_WORLD_1:
        {
            return input::Key::World_1;
        }
        case GLFW_KEY_WORLD_2:
        {
            return input::Key::World_2;
        }
        case GLFW_KEY_ESCAPE:
        {
            return input::Key::Escape;
        }
        case GLFW_KEY_ENTER:
        {
            return input::Key::Enter;
        }
        case GLFW_KEY_TAB:
        {
            return input::Key::Tab;
        }
        case GLFW_KEY_BACKSPACE:
        {
            return input::Key::Backspace;
        }
        case GLFW_KEY_INSERT:
        {
            return input::Key::Insert;
        }
        case GLFW_KEY_DELETE:
        {
            return input::Key::Delete;
        }
        case GLFW_KEY_RIGHT:
        {
            return input::Key::Right;
        }
        case GLFW_KEY_LEFT:
        {
            return input::Key::Left;
        }
        case GLFW_KEY_DOWN:
        {
            return input::Key::Down;
        }
        case GLFW_KEY_UP:
        {
            return input::Key::Up;
        }
        case GLFW_KEY_PAGE_UP:
        {
            return input::Key::PageUp;
        }
        case GLFW_KEY_PAGE_DOWN:
        {
            return input::Key::PageDown;
        }
        case GLFW_KEY_HOME:
        {
            return input::Key::Home;
        }
        case GLFW_KEY_END:
        {
            return input::Key::End;
        }
        case GLFW_KEY_CAPS_LOCK:
        {
            return input::Key::CapsLock;
        }
        case GLFW_KEY_SCROLL_LOCK:
        {
            return input::Key::ScrollLock;
        }
        case GLFW_KEY_NUM_LOCK:
        {
            return input::Key::NumLock;
        }
        case GLFW_KEY_PRINT_SCREEN:
        {
            return input::Key::PrintScreen;
        }
        case GLFW_KEY_PAUSE:
        {
            return input::Key::Pause;
        }
        case GLFW_KEY_F1:
        {
            return input::Key::F1;
        }
        case GLFW_KEY_F2:
        {
            return input::Key::F2;
        }
        case GLFW_KEY_F3:
        {
            return input::Key::F3;
        }
        case GLFW_KEY_F4:
        {
            return input::Key::F4;
        }
        case GLFW_KEY_F5:
        {
            return input::Key::F5;
        }
        case GLFW_KEY_F6:
        {
            return input::Key::F6;
        }
        case GLFW_KEY_F7:
        {
            return input::Key::F7;
        }
        case GLFW_KEY_F8:
        {
            return input::Key::F8;
        }
        case GLFW_KEY_F9:
        {
            return input::Key::F9;
        }
        case GLFW_KEY_F10:
        {
            return input::Key::F10;
        }
        case GLFW_KEY_F11:
        {
            return input::Key::F11;
        }
        case GLFW_KEY_F12:
        {
            return input::Key::F12;
        }
        case GLFW_KEY_F13:
        {
            return input::Key::F13;
        }
        case GLFW_KEY_F14:
        {
            return input::Key::F14;
        }
        case GLFW_KEY_F15:
        {
            return input::Key::F15;
        }
        case GLFW_KEY_F16:
        {
            return input::Key::F16;
        }
        case GLFW_KEY_F17:
        {
            return input::Key::F17;
        }
        case GLFW_KEY_F18:
        {
            return input::Key::F18;
        }
        case GLFW_KEY_F19:
        {
            return input::Key::F19;
        }
        case GLFW_KEY_F20:
        {
            return input::Key::F20;
        }
        case GLFW_KEY_F21:
        {
            return input::Key::F21;
        }
        case GLFW_KEY_F22:
        {
            return input::Key::F22;
        }
        case GLFW_KEY_F23:
        {
            return input::Key::F23;
        }
        case GLFW_KEY_F24:
        {
            return input::Key::F24;
        }
        case GLFW_KEY_F25:
        {
            return input::Key::F25;
        }
        case GLFW_KEY_KP_0:
        {
            return input::Key::Num_0;
        }
        case GLFW_KEY_KP_1:
        {
            return input::Key::Num_1;
        }
        case GLFW_KEY_KP_2:
        {
            return input::Key::Num_2;
        }
        case GLFW_KEY_KP_3:
        {
            return input::Key::Num_3;
        }
        case GLFW_KEY_KP_4:
        {
            return input::Key::Num_4;
        }
        case GLFW_KEY_KP_5:
        {
            return input::Key::Num_5;
        }
        case GLFW_KEY_KP_6:
        {
            return input::Key::Num_6;
        }
        case GLFW_KEY_KP_7:
        {
            return input::Key::Num_7;
        }
        case GLFW_KEY_KP_8:
        {
            return input::Key::Num_8;
        }
        case GLFW_KEY_KP_9:
        {
            return input::Key::Num_9;
        }
        case GLFW_KEY_KP_DECIMAL:
        {
            return input::Key::Num_Decimal;
        }
        case GLFW_KEY_KP_DIVIDE:
        {
            return input::Key::Num_Divide;
        }
        case GLFW_KEY_KP_MULTIPLY:
        {
            return input::Key::Num_Multiply;
        }
        case GLFW_KEY_KP_SUBTRACT:
        {
            return input::Key::Num_Subtract;
        }
        case GLFW_KEY_KP_ADD:
        {
            return input::Key::Num_Add;
        }
        case GLFW_KEY_KP_ENTER:
        {
            return input::Key::Num_Enter;
        }
        case GLFW_KEY_KP_EQUAL:
        {
            return input::Key::Num_Equal;
        }
        case GLFW_KEY_LEFT_SHIFT:
        {
            return input::Key::LeftShift;
        }
        case GLFW_KEY_LEFT_CONTROL:
        {
            return input::Key::LeftControl;
        }
        case GLFW_KEY_LEFT_ALT:
        {
            return input::Key::LeftAlt;
        }
        case GLFW_KEY_LEFT_SUPER:
        {
            return input::Key::LeftSuper;
        }
        case GLFW_KEY_RIGHT_SHIFT:
        {
            return input::Key::RightShift;
        }
        case GLFW_KEY_RIGHT_CONTROL:
        {
            return input::Key::RightControl;
        }
        case GLFW_KEY_RIGHT_ALT:
        {
            return input::Key::RightAlt;
        }
        case GLFW_KEY_RIGHT_SUPER:
        {
            return input::Key::RightSuper;
        }
        case GLFW_KEY_MENU:
        {
            return input::Key::Menu;
        }
        default:
        {
            return input::Key::Unknown;
        }
    }
}

int32_t Adapter::ConvertToGlfwKey(input::Key key)
{
    switch (key)
    {
        case input::Key::Space:
        {
            return GLFW_KEY_SPACE;
        }
        case input::Key::Apostrophe:
        {
            return GLFW_KEY_APOSTROPHE;
        }
        case input::Key::Comma:
        {
            return GLFW_KEY_COMMA;
        }
        case input::Key::Minus:
        {
            return GLFW_KEY_MINUS;
        }
        case input::Key::Period:
        {
            return GLFW_KEY_PERIOD;
        }
        case input::Key::Slash:
        {
            return GLFW_KEY_SLASH;
        }
        case input::Key::Normal_0:
        {
            return GLFW_KEY_0;
        }
        case input::Key::Normal_1:
        {
            return GLFW_KEY_1;
        }
        case input::Key::Normal_2:
        {
            return GLFW_KEY_2;
        }
        case input::Key::Normal_3:
        {
            return GLFW_KEY_3;
        }
        case input::Key::Normal_4:
        {
            return GLFW_KEY_4;
        }
        case input::Key::Normal_5:
        {
            return GLFW_KEY_5;
        }
        case input::Key::Normal_6:
        {
            return GLFW_KEY_6;
        }
        case input::Key::Normal_7:
        {
            return GLFW_KEY_7;
        }
        case input::Key::Normal_8:
        {
            return GLFW_KEY_8;
        }
        case input::Key::Normal_9:
        {
            return GLFW_KEY_9;
        }
        case input::Key::Semicolon:
        {
            return GLFW_KEY_SEMICOLON;
        }
        case input::Key::Equal:
        {
            return GLFW_KEY_EQUAL;
        }
        case input::Key::A:
        {
            return GLFW_KEY_A;
        }
        case input::Key::B:
        {
            return GLFW_KEY_B;
        }
        case input::Key::C:
        {
            return GLFW_KEY_C;
        }
        case input::Key::D:
        {
            return GLFW_KEY_D;
        }
        case input::Key::E:
        {
            return GLFW_KEY_E;
        }
        case input::Key::F:
        {
            return GLFW_KEY_F;
        }
        case input::Key::G:
        {
            return GLFW_KEY_G;
        }
        case input::Key::H:
        {
            return GLFW_KEY_H;
        }
        case input::Key::I:
        {
            return GLFW_KEY_I;
        }
        case input::Key::J:
        {
            return GLFW_KEY_J;
        }
        case input::Key::K:
        {
            return GLFW_KEY_K;
        }
        case input::Key::L:
        {
            return GLFW_KEY_L;
        }
        case input::Key::M:
        {
            return GLFW_KEY_M;
        }
        case input::Key::N:
        {
            return GLFW_KEY_N;
        }
        case input::Key::O:
        {
            return GLFW_KEY_O;
        }
        case input::Key::P:
        {
            return GLFW_KEY_P;
        }
        case input::Key::Q:
        {
            return GLFW_KEY_Q;
        }
        case input::Key::R:
        {
            return GLFW_KEY_R;
        }
        case input::Key::S:
        {
            return GLFW_KEY_S;
        }
        case input::Key::T:
        {
            return GLFW_KEY_T;
        }
        case input::Key::U:
        {
            return GLFW_KEY_U;
        }
        case input::Key::V:
        {
            return GLFW_KEY_V;
        }
        case input::Key::W:
        {
            return GLFW_KEY_W;
        }
        case input::Key::X:
        {
            return GLFW_KEY_X;
        }
        case input::Key::Y:
        {
            return GLFW_KEY_Y;
        }
        case input::Key::Z:
        {
            return GLFW_KEY_Z;
        }
        case input::Key::LeftBracket:
        {
            return GLFW_KEY_LEFT_BRACKET;
        }
        case input::Key::Backslash:
        {
            return GLFW_KEY_BACKSLASH;
        }
        case input::Key::RightBracket:
        {
            return GLFW_KEY_RIGHT_BRACKET;
        }
        case input::Key::GraveAccent:
        {
            return GLFW_KEY_GRAVE_ACCENT;
        }
        case input::Key::World_1:
        {
            return GLFW_KEY_WORLD_1;
        }
        case input::Key::World_2:
        {
            return GLFW_KEY_WORLD_2;
        }
        case input::Key::Escape:
        {
            return GLFW_KEY_ESCAPE;
        }
        case input::Key::Enter:
        {
            return GLFW_KEY_ENTER;
        }
        case input::Key::Tab:
        {
            return GLFW_KEY_TAB;
        }
        case input::Key::Backspace:
        {
            return GLFW_KEY_BACKSPACE;
        }
        case input::Key::Insert:
        {
            return GLFW_KEY_INSERT;
        }
        case input::Key::Delete:
        {
            return GLFW_KEY_DELETE;
        }
        case input::Key::Right:
        {
            return GLFW_KEY_RIGHT;
        }
        case input::Key::Left:
        {
            return GLFW_KEY_LEFT;
        }
        case input::Key::Down:
        {
            return GLFW_KEY_DOWN;
        }
        case input::Key::Up:
        {
            return GLFW_KEY_UP;
        }
        case input::Key::PageUp:
        {
            return GLFW_KEY_PAGE_UP;
        }
        case input::Key::PageDown:
        {
            return GLFW_KEY_PAGE_DOWN;
        }
        case input::Key::Home:
        {
            return GLFW_KEY_HOME;
        }
        case input::Key::End:
        {
            return GLFW_KEY_END;
        }
        case input::Key::CapsLock:
        {
            return GLFW_KEY_CAPS_LOCK;
        }
        case input::Key::ScrollLock:
        {
            return GLFW_KEY_SCROLL_LOCK;
        }
        case input::Key::NumLock:
        {
            return GLFW_KEY_NUM_LOCK;
        }
        case input::Key::PrintScreen:
        {
            return GLFW_KEY_PRINT_SCREEN;
        }
        case input::Key::Pause:
        {
            return GLFW_KEY_PAUSE;
        }
        case input::Key::F1:
        {
            return GLFW_KEY_F1;
        }
        case input::Key::F2:
        {
            return GLFW_KEY_F2;
        }
        case input::Key::F3:
        {
            return GLFW_KEY_F3;
        }
        case input::Key::F4:
        {
            return GLFW_KEY_F4;
        }
        case input::Key::F5:
        {
            return GLFW_KEY_F5;
        }
        case input::Key::F6:
        {
            return GLFW_KEY_F6;
        }
        case input::Key::F7:
        {
            return GLFW_KEY_F7;
        }
        case input::Key::F8:
        {
            return GLFW_KEY_F8;
        }
        case input::Key::F9:
        {
            return GLFW_KEY_F9;
        }
        case input::Key::F10:
        {
            return GLFW_KEY_F10;
        }
        case input::Key::F11:
        {
            return GLFW_KEY_F11;
        }
        case input::Key::F12:
        {
            return GLFW_KEY_F12;
        }
        case input::Key::F13:
        {
            return GLFW_KEY_F13;
        }
        case input::Key::F14:
        {
            return GLFW_KEY_F14;
        }
        case input::Key::F15:
        {
            return GLFW_KEY_F15;
        }
        case input::Key::F16:
        {
            return GLFW_KEY_F16;
        }
        case input::Key::F17:
        {
            return GLFW_KEY_F17;
        }
        case input::Key::F18:
        {
            return GLFW_KEY_F18;
        }
        case input::Key::F19:
        {
            return GLFW_KEY_F19;
        }
        case input::Key::F20:
        {
            return GLFW_KEY_F20;
        }
        case input::Key::F21:
        {
            return GLFW_KEY_F21;
        }
        case input::Key::F22:
        {
            return GLFW_KEY_F22;
        }
        case input::Key::F23:
        {
            return GLFW_KEY_F23;
        }
        case input::Key::F24:
        {
            return GLFW_KEY_F24;
        }
        case input::Key::F25:
        {
            return GLFW_KEY_F25;
        }
        case input::Key::Num_0:
        {
            return GLFW_KEY_KP_0;
        }
        case input::Key::Num_1:
        {
            return GLFW_KEY_KP_1;
        }
        case input::Key::Num_2:
        {
            return GLFW_KEY_KP_2;
        }
        case input::Key::Num_3:
        {
            return GLFW_KEY_KP_3;
        }
        case input::Key::Num_4:
        {
            return GLFW_KEY_KP_4;
        }
        case input::Key::Num_5:
        {
            return GLFW_KEY_KP_5;
        }
        case input::Key::Num_6:
        {
            return GLFW_KEY_KP_6;
        }
        case input::Key::Num_7:
        {
            return GLFW_KEY_KP_7;
        }
        case input::Key::Num_8:
        {
            return GLFW_KEY_KP_8;
        }
        case input::Key::Num_9:
        {
            return GLFW_KEY_KP_9;
        }
        case input::Key::Num_Decimal:
        {
            return GLFW_KEY_KP_DECIMAL;
        }
        case input::Key::Num_Divide:
        {
            return GLFW_KEY_KP_DIVIDE;
        }
        case input::Key::Num_Multiply:
        {
            return GLFW_KEY_KP_MULTIPLY;
        }
        case input::Key::Num_Subtract:
        {
            return GLFW_KEY_KP_SUBTRACT;
        }
        case input::Key::Num_Add:
        {
            return GLFW_KEY_KP_ADD;
        }
        case input::Key::Num_Enter:
        {
            return GLFW_KEY_KP_ENTER;
        }
        case input::Key::Num_Equal:
        {
            return GLFW_KEY_KP_EQUAL;
        }
        case input::Key::LeftShift:
        {
            return GLFW_KEY_LEFT_SHIFT;
        }
        case input::Key::LeftControl:
        {
            return GLFW_KEY_LEFT_CONTROL;
        }
        case input::Key::LeftAlt:
        {
            return GLFW_KEY_LEFT_ALT;
        }
        case input::Key::LeftSuper:
        {
            return GLFW_KEY_LEFT_SUPER;
        }
        case input::Key::RightShift:
        {
            return GLFW_KEY_RIGHT_SHIFT;
        }
        case input::Key::RightControl:
        {
            return GLFW_KEY_RIGHT_CONTROL;
        }
        case input::Key::RightAlt:
        {
            return GLFW_KEY_RIGHT_ALT;
        }
        case input::Key::RightSuper:
        {
            return GLFW_KEY_RIGHT_SUPER;
        }
        case input::Key::Menu:
        {
            return GLFW_KEY_MENU;
        }
        case input::Key::Unrecognized:
        default:
        {
            return GLFW_KEY_UNKNOWN;
        }
    }
}

}
}
}
