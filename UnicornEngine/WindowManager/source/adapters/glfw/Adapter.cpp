/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/adapters/glfw/Adapter.hpp>

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

bool Adapter::s_isInitialized = false;

wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowPositionChanged = {};
wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowSizeChanged = {};
wink::signal< wink::slot<void(void*)> > Adapter::WindowClose = {};
wink::signal< wink::slot<void(void*)> > Adapter::WindowContentRefresh = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowFocused = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowMinimized = {};
wink::signal< wink::slot<void(void*, bool)> > Adapter::WindowMaximized = {};
wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > Adapter::WindowFramebufferResized = {};

wink::signal< wink::slot<void(void*, MonitorMemento::State)> > Adapter::MonitorStateChanged = {};

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

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwSetMonitorCallback(unicornMonitorStateChanged);

        s_isInitialized = true;
    }
}

void Adapter::Deinit()
{
    if (s_isInitialized)
    {
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

    for (unsigned int i = 0; i < glfwExtensionCount; ++i)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    return extensions;
}

std::vector<MonitorMemento> Adapter::GetMonitors()
{
    const GLFWmonitor* primary = glfwGetPrimaryMonitor();

    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<MonitorMemento> result;
    result.reserve(count);

    const char* name = nullptr;

    std::pair<int32_t, int32_t> physicalSize({0, 0});
    std::pair<int32_t, int32_t> virtualPosition({0, 0});

    const GLFWgammaramp* ramp = nullptr;
    GammaRamp gammaRamp;

    int modeCount = 0;
    const GLFWvidmode* modes = nullptr;
    const GLFWvidmode* currentMode = nullptr;
    std::vector<VideoMode> videoModes;

    for (int i = 0; i < count; ++i)
    {
        name = glfwGetMonitorName(monitors[i]);
        glfwGetMonitorPhysicalSize(monitors[i], &physicalSize.first, &physicalSize.second);
        glfwGetMonitorPos(monitors[i], &virtualPosition.first, &virtualPosition.second);

        currentMode = glfwGetVideoMode(monitors[i]);
        modes = glfwGetVideoModes(monitors[i], &modeCount);
        videoModes.clear();
        videoModes.reserve(modeCount);

        for (int m = 0; m < modeCount; ++m)
        {
            VideoMode mode({});

            mode.width = modes[m].width;
            mode.height = modes[m].height;
            mode.rgbBitDepth = {{ modes[m].redBits, modes[m].greenBits, modes[m].blueBits }};
            mode.refreshRate = modes[m].refreshRate;
            mode.isCurrent = ( &modes[m] == currentMode );

            videoModes.push_back(mode);
        }

        ramp = glfwGetGammaRamp(monitors[i]);
        if (ramp)
        {
            gammaRamp.size = ramp->size;
            gammaRamp.red = new uint16_t[gammaRamp.size];
            gammaRamp.green = new uint16_t[gammaRamp.size];
            gammaRamp.blue = new uint16_t[gammaRamp.size];

            for (uint32_t g = 0; g < gammaRamp.size; ++g)
            {
                gammaRamp.red[g] = ramp->red[g];
                gammaRamp.green[g] = ramp->green[g];
                gammaRamp.blue[g] = ramp->blue[g];
            }
        }
        else
        {
            gammaRamp = GammaRamp();
        }

        result.push_back(MonitorMemento({ static_cast<uint32_t>(i)
            , name
            , physicalSize
            , virtualPosition
            , videoModes
            , gammaRamp
            , ( monitors[i] == primary )
            , MonitorMemento::State::Connected
            , monitors[i]
        }));
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

}
}
}
