/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/adapters/glfw/Adapter.hpp>

#include <unicorn/system/CustomValue.hpp>
#include <unicorn/system/GammaRamp.hpp>
#include <unicorn/system/MonitorMemento.hpp>
#include <unicorn/system/VideoMode.hpp>

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

void Adapter::SetWindowCreationHint(system::WindowHint hint, int32_t value)
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

int32_t Adapter::GetWindowAttribute(void* handle, system::WindowAttribute attribute)
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

int Adapter::ConvertToGlfwHint(system::WindowHint hint)
{
    switch (hint)
    {
        case system::WindowHint::AutoMinimize:
        {
            return GLFW_AUTO_ICONIFY;
        }
        case system::WindowHint::Decorated:
        {
            return GLFW_DECORATED;
        }
        case system::WindowHint::Floating:
        {
            return GLFW_FLOATING;
        }
        case system::WindowHint::Focused:
        {
            return GLFW_FOCUSED;
        }
        case system::WindowHint::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case system::WindowHint::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case system::WindowHint::Visible:
        {
            return GLFW_VISIBLE;
        }
        case system::WindowHint::Doublebuffer:
        {
            return GLFW_DOUBLEBUFFER;
        }
        case system::WindowHint::Stereo:
        {
            return GLFW_STEREO;
        }
        case system::WindowHint::Samples:
        {
            return GLFW_SAMPLES;
        }
        case system::WindowHint::RefreshRate:
        {
            return GLFW_REFRESH_RATE;
        }
        case system::WindowHint::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case system::WindowHint::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case system::WindowHint::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case system::WindowHint::ContextVersionMinor:
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
    if (0 == (value & system::CustomValue::Mask))
    {
        return value;
    }

    switch (value)
    {
        case system::CustomValue::False:
        {
            return GLFW_FALSE;
        }
        case system::CustomValue::True:
        {
            return GLFW_TRUE;
        }
        case system::CustomValue::OpenGL_API:
        {
            return GLFW_OPENGL_API;
        }
        case system::CustomValue::OpenGL_ES_API:
        {
            return GLFW_OPENGL_ES_API;
        }
        case system::CustomValue::No_API:
        {
            return GLFW_NO_API;
        }
        case system::CustomValue::NativeContextAPI:
        {
            return GLFW_NATIVE_CONTEXT_API;
        }
        case system::CustomValue::EGLContextAPI:
        {
            return GLFW_EGL_CONTEXT_API;
        }
        case system::CustomValue::DontCare:
        {
            return GLFW_DONT_CARE;
        }
        default:
        {
            return value;
        }
    }
}

int Adapter::ConvertToGlfwAttribute(system::WindowAttribute attribute)
{
    switch (attribute)
    {
        case system::WindowAttribute::Decorated:
        {
            return GLFW_DECORATED;
        }
        case system::WindowAttribute::Floating:
        {
            return GLFW_FLOATING;
        }
        case system::WindowAttribute::Focused:
        {
            return GLFW_FOCUSED;
        }
        case system::WindowAttribute::Maximized:
        {
            return GLFW_MAXIMIZED;
        }
        case system::WindowAttribute::Minimized:
        {
            return GLFW_ICONIFIED;
        }
        case system::WindowAttribute::Resizable:
        {
            return GLFW_RESIZABLE;
        }
        case system::WindowAttribute::Visible:
        {
            return GLFW_VISIBLE;
        }
        case system::WindowAttribute::Samples:
        {
            return GLFW_SAMPLES;
        }
        case system::WindowAttribute::ClientAPI:
        {
            return GLFW_CLIENT_API;
        }
        case system::WindowAttribute::ContextCreationAPI:
        {
            return GLFW_CONTEXT_CREATION_API;
        }
        case system::WindowAttribute::ContextVersionMajor:
        {
            return GLFW_CONTEXT_VERSION_MAJOR;
        }
        case system::WindowAttribute::ContextVersionMinor:
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
