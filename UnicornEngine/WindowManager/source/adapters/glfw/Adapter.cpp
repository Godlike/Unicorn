/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/adapters/glfw/Adapter.hpp>

#include <unicorn/window_manager/CustomValue.hpp>
#include <unicorn/window_manager/GammaRamp.hpp>
#include <unicorn/window_manager/MonitorMemento.hpp>
#include <unicorn/window_manager/VideoMode.hpp>

#include <GLFW/glfw3.h>

namespace unicorn
{
namespace WindowManager
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

void unicornMonitorStateChanged(GLFWmonitor* handle, int event)
{
    Adapter::MonitorStateChanged.emit(
        static_cast<void*>(handle),
        GLFW_CONNECTED == event ? MonitorMemento::State::Connected : MonitorMemento::State::Disconnected
    );
}

}

void Adapter::Init()
{
    if (!s_isInitialized)
    {
        glfwInit();

        glfwSetMonitorCallback(unicornMonitorStateChanged);

        s_isInitialized = true;
    }
}

void Adapter::Deinit()
{
    if (s_isInitialized)
    {
        glfwSetMonitorCallback(NULL);

        glfwTerminate();

        WindowPositionChanged.clear();
        WindowSizeChanged.clear();
        WindowClose.clear();
        WindowContentRefresh.clear();
        WindowFocused.clear();
        WindowMinimized.clear();
        WindowMaximized.clear();
        WindowFramebufferResized.clear();

        MonitorStateChanged.clear();

        s_isInitialized = false;
    }
}

void Adapter::SetWindowCreationHint(WindowManager::WindowHint hint, int32_t value)
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

int32_t Adapter::GetWindowAttribute(void* handle, WindowManager::WindowAttribute attribute)
{
    return glfwGetWindowAttrib(static_cast<GLFWwindow*>(handle), ConvertToGlfwAttribute(attribute));
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
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    extensions.reserve(glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; ++i)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    return extensions;
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
    glfwGetMonitorPos(monitorHandle, &result.virtualPosition.first, &result.virtualPosition.second);

    {
        int modeCount = 0;
        const GLFWvidmode* currentMode = glfwGetVideoMode(monitorHandle);
        const GLFWvidmode* modes = glfwGetVideoModes(monitorHandle, &modeCount);

        result.modes.reserve(modeCount);

        for (int m = 0; m < modeCount; ++m)
        {
            VideoMode mode = {};

            mode.width = modes[m].width;
            mode.height = modes[m].height;
            mode.rgbBitDepth = {{ modes[m].redBits, modes[m].greenBits, modes[m].blueBits }};
            mode.refreshRate = modes[m].refreshRate;
            mode.isCurrent = ( modes[m] == *currentMode );

            result.modes.push_back(mode);
        }
    }

    {
        const GLFWgammaramp* ramp = glfwGetGammaRamp(monitorHandle);
        if (ramp)
        {
            result.gammaRamp.size = ramp->size;
            result.gammaRamp.red = new uint16_t[result.gammaRamp.size];
            result.gammaRamp.green = new uint16_t[result.gammaRamp.size];
            result.gammaRamp.blue = new uint16_t[result.gammaRamp.size];

            for (uint32_t g = 0; g < result.gammaRamp.size; ++g)
            {
                result.gammaRamp.red[g] = ramp->red[g];
                result.gammaRamp.green[g] = ramp->green[g];
                result.gammaRamp.blue[g] = ramp->blue[g];
            }
        }
        else
        {
            result.gammaRamp = GammaRamp();
        }
    }

    result.isPrimary = ( monitorHandle == primary );
    result.id = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));
    result.state = MonitorMemento::State::Connected;
    result.handle = handle;

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

int Adapter::ConvertToGlfwHint(WindowManager::WindowHint hint)
{
    switch (hint)
    {
        case WindowManager::WindowHint::AutoMinimize:
        {
            return GLFW_AUTO_ICONIFY;
        }
        case WindowManager::WindowHint::Decorated:
        {
            return GLFW_DECORATED;
        }
        case WindowManager::WindowHint::Floating:
        {
            return GLFW_FLOATING;
        }
        case WindowManager::WindowHint::Focused:
        {
            return GLFW_FOCUSED;
        }
        case WindowManager::WindowHint::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case WindowManager::WindowHint::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case WindowManager::WindowHint::Visible:
        {
            return GLFW_VISIBLE;
        }
        case WindowManager::WindowHint::Doublebuffer:
        {
            return GLFW_DOUBLEBUFFER;
        }
        case WindowManager::WindowHint::Stereo:
        {
            return GLFW_STEREO;
        }
        case WindowManager::WindowHint::Samples:
        {
            return GLFW_SAMPLES;
        }
        case WindowManager::WindowHint::RefreshRate:
        {
            return GLFW_REFRESH_RATE;
        }
        case WindowManager::WindowHint::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case WindowManager::WindowHint::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case WindowManager::WindowHint::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case WindowManager::WindowHint::ContextVersionMinor:
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
    if (0 == (value & WindowManager::CustomValue::Mask))
    {
        return value;
    }

    switch (value)
    {
        case WindowManager::CustomValue::False:
        {
            return GLFW_FALSE;
        }
        case WindowManager::CustomValue::True:
        {
            return GLFW_TRUE;
        }
        case WindowManager::CustomValue::OpenGL_API:
        {
            return GLFW_OPENGL_API;
        }
        case WindowManager::CustomValue::OpenGL_ES_API:
        {
            return GLFW_OPENGL_ES_API;
        }
        case WindowManager::CustomValue::No_API:
        {
            return GLFW_NO_API;
        }
        case WindowManager::CustomValue::NativeContextAPI:
        {
            return GLFW_NATIVE_CONTEXT_API;
        }
        case WindowManager::CustomValue::EGLContextAPI:
        {
            return GLFW_EGL_CONTEXT_API;
        }
        case WindowManager::CustomValue::DontCare:
        {
            return GLFW_DONT_CARE;
        }
        default:
        {
            return value;
        }
    }
}

int Adapter::ConvertToGlfwAttribute(WindowManager::WindowAttribute attribute)
{
    switch (attribute)
    {
        case WindowManager::WindowAttribute::Decorated:
        {
            return GLFW_DECORATED;
        }
        case WindowManager::WindowAttribute::Floating:
        {
            return GLFW_FLOATING;
        }
        case WindowManager::WindowAttribute::Focused:
        {
            return GLFW_FOCUSED;
        }
        case WindowManager::WindowAttribute::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case WindowManager::WindowAttribute::Minimized:
        {
            return GLFW_ICONIFIED;
        }
        case WindowManager::WindowAttribute::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case WindowManager::WindowAttribute::Visible:
        {
            return GLFW_VISIBLE;
        }
        case WindowManager::WindowAttribute::Samples:
        {
            return GLFW_SAMPLES;
        }
        case WindowManager::WindowAttribute::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case WindowManager::WindowAttribute::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case WindowManager::WindowAttribute::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case WindowManager::WindowAttribute::ContextVersionMinor:
        {
            return GLFW_CONTEXT_VERSION_MINOR;
        }
        default:
        {
            return GLFW_INVALID_ENUM;
        }
    }
}

}
}
}
