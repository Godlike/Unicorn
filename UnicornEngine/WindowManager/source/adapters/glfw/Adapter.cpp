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

void unicornWindowPositionChanged(GLFWwindow* pHandle, int x, int y)
{
    Adapter::WindowPositionChanged.emit(
        static_cast<void*>(pHandle),
        std::pair<int32_t, int32_t>(x, y)
    );
}

void unicornWindowSizeChanged(GLFWwindow* pHandle, int width, int height)
{
    Adapter::WindowSizeChanged.emit(
        static_cast<void*>(pHandle),
        std::pair<int32_t, int32_t>(width, height)
    );
}

void unicornWindowClose(GLFWwindow* pHandle)
{
    Adapter::WindowClose.emit(
        static_cast<void*>(pHandle)
    );
}

void unicornWindowContentRefresh(GLFWwindow* pHandle)
{
    Adapter::WindowContentRefresh.emit(
        static_cast<void*>(pHandle)
    );
}

void unicornWindowFocused(GLFWwindow* pHandle, int focused)
{
    Adapter::WindowFocused.emit(
        static_cast<void*>(pHandle),
        GLFW_TRUE == focused ? true : false
    );
}

void unicornWindowMinimized(GLFWwindow* pHandle, int minimized)
{
    Adapter::WindowMinimized.emit(
        static_cast<void*>(pHandle),
        GLFW_TRUE == minimized ? true : false
    );
}

void unicornWindowMaximized(GLFWwindow* pHandle, int maximized)
{
    Adapter::WindowMaximized.emit(
        static_cast<void*>(pHandle),
        GLFW_TRUE == maximized ? true : false
    );
}

void unicornWindowFramebufferResized(GLFWwindow* pHandle, int width, int height)
{
    Adapter::WindowFramebufferResized.emit(
        static_cast<void*>(pHandle),
        std::pair<int32_t, int32_t>(width, height)
    );
}

void unicornMonitorStateChanged(GLFWmonitor* pHandle, int event)
{
    Adapter::MonitorStateChanged.emit(
        static_cast<void*>(pHandle),
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

void Adapter::SetWindowHint(WindowManager::WindowHint hint, int32_t value)
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
    void* pMonitorHandle,
    void* pSharedWindowHandle)
{
    GLFWwindow* pHandle = glfwCreateWindow(width,
        height,
        name,
        static_cast<GLFWmonitor*>(pMonitorHandle),
        static_cast<GLFWwindow*>(pSharedWindowHandle) );

    glfwSetWindowPosCallback(pHandle, unicornWindowPositionChanged);
    glfwSetWindowSizeCallback(pHandle, unicornWindowSizeChanged);
    glfwSetWindowCloseCallback(pHandle, unicornWindowClose);
    glfwSetWindowRefreshCallback(pHandle, unicornWindowContentRefresh);
    glfwSetWindowFocusCallback(pHandle, unicornWindowFocused);
    glfwSetWindowIconifyCallback(pHandle, unicornWindowMinimized);
    glfwSetWindowMaximizeCallback(pHandle, unicornWindowMaximized);
    glfwSetFramebufferSizeCallback(pHandle, unicornWindowFramebufferResized);

    return static_cast<void*>(pHandle);
}

void Adapter::DestroyWindow(void* pHandle)
{
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(pHandle), GLFW_TRUE);
    glfwDestroyWindow(static_cast<GLFWwindow*>(pHandle));
}

bool Adapter::IsVulkanSupported()
{
    return glfwVulkanSupported();
}

VkResult Adapter::CreateVulkanSurface(VkInstance instance,
    void* pHandle,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return glfwCreateWindowSurface(instance
        , static_cast<GLFWwindow*>(pHandle)
        , allocator
        , surface
    );
}

bool Adapter::WindowShouldClose(void* pHandle)
{
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(pHandle));
}

void Adapter::SetWindowShouldClose(void* pHandle, bool flag)
{
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(pHandle), flag ? GLFW_TRUE : GLFW_FALSE);
}

void Adapter::GetWindowPosition(void* pHandle, int* x, int* y)
{
    glfwGetWindowPos(static_cast<GLFWwindow*>(pHandle), x, y);
}

void Adapter::SetWindowPosition(void* pHandle, int x, int y)
{
    glfwSetWindowPos(static_cast<GLFWwindow*>(pHandle), x, y);
}

void Adapter::PollEvents()
{
    glfwPollEvents();
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
    GLFWmonitor* pMonitor = static_cast<GLFWmonitor*>(handle);

    result.name = glfwGetMonitorName(pMonitor);

    glfwGetMonitorPhysicalSize(pMonitor, &result.physicalSize.first, &result.physicalSize.second);
    glfwGetMonitorPos(pMonitor, &result.virtualPosition.first, &result.virtualPosition.second);

    {
        int modeCount = 0;
        const GLFWvidmode* currentMode = glfwGetVideoMode(pMonitor);
        const GLFWvidmode* modes = glfwGetVideoModes(pMonitor, &modeCount);

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
        const GLFWgammaramp* ramp = glfwGetGammaRamp(pMonitor);
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

    result.isPrimary = ( pMonitor == primary );
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

}
}
}
