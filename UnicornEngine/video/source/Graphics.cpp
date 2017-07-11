/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/utility/Logger.hpp>

#include <unicorn/system/Manager.hpp>
#include <unicorn/system/Window.hpp>

#include <unicorn/video/vulkan/Context.hpp>
#include <unicorn/video/vulkan/Renderer.hpp>

namespace unicorn
{
namespace video
{
Graphics::Graphics(system::Manager& manager) : m_isInitialized(false)
                                             , m_systemManager(manager)
                                             , m_driver(DriverType::Vulkan)
{
}

Graphics::~Graphics()
{
    Deinit();
}

bool Graphics::Init(const DriverType& driver)
{
    if (m_isInitialized)
    {
        return false;
    }

    m_driver = driver;

    LOG_INFO("Graphics initialization started.");

    switch (m_driver)
    {
    case DriverType::Vulkan:
        if (!m_systemManager.IsVulkanSupported())
        {
            LOG_ERROR("Vulkan not supported!");
            return false;
        }
        if (!vulkan::Context::Instance().Initialize(m_systemManager))
        {
            LOG_ERROR("Vulkan context not initialized!");
            return false;
        }
        break;
    default:
        return false;
    }
    m_isInitialized = true;

    LOG_INFO("Graphics initialized correctly.");

    return true;
}

void Graphics::Deinit()
{
    m_expiredRenderers.insert(m_renderers.begin(), m_renderers.end());

    m_renderers.clear();

    ProcessExpiredRenderers();

    switch (m_driver)
    {
    case DriverType::Vulkan:
        vulkan::Context::Instance().Deinitialize();
        break;
    }

    if (m_isInitialized)
    {
        LOG_INFO("Graphics shutdown correctly.");
    }

    m_isInitialized = false;
}

bool Graphics::Render()
{
    if (m_isInitialized)
    {
        for (RendererWindowPairSet::const_iterator cit = m_renderers.cbegin(); cit != m_renderers.cend();)
        {
            if (!cit->second->ShouldClose() && cit->first->Render())
            {
                ++cit;
            }
            else
            {
                m_expiredRenderers.insert(*cit);
                cit = m_renderers.erase(cit);
            }
        }

        ProcessExpiredRenderers();

        return !m_renderers.empty();
    }

    return false;
}

system::Window* Graphics::SpawnWindow(int32_t width,
                                      int32_t height,
                                      const std::string& name,
                                      system::Monitor* pMonitor,
                                      system::Window* pSharedWindow)
{
    return m_systemManager.CreateWindow(width, height, name, pMonitor, pSharedWindow);
}

system::Window* Graphics::GetFocusedWindow() const
{
    return m_systemManager.GetFocusedWindow();
}

void Graphics::DepthTest(bool enabled)
{
    for(const auto& renderer : m_renderers)
    {
        renderer.first->DepthTest(enabled);
    }
}

const std::vector<system::Monitor*>& Graphics::GetMonitors() const
{
    return m_systemManager.GetMonitors();
}

void Graphics::BindWindowRenderer(system::Window* pWindow, video::Renderer* pRenderer)
{
    m_renderers.insert(RendererWindowPair(pRenderer, pWindow));
}

system::Monitor* Graphics::GetWindowMonitor(const system::Window& window) const
{
    return m_systemManager.GetWindowMonitor(window);
}

void Graphics::SetWindowMonitor(const system::Window& window,
                                system::Monitor* pMonitor,
                                const std::pair<int32_t, int32_t>& position,
                                const std::pair<int32_t, int32_t>& size,
                                int32_t refreshRate) const
{
    m_systemManager.SetWindowMonitor(window,
                                     pMonitor,
                                     position,
                                     size,
                                     refreshRate);
}

void Graphics::SetWindowCreationHint(system::WindowHint hint, int32_t value) const
{
    m_systemManager.SetWindowCreationHint(hint, value);
}

void Graphics::ProcessExpiredRenderers()
{
    if (!m_expiredRenderers.empty())
    {
        for (RendererWindowPairSet::const_iterator cit = m_expiredRenderers.cbegin(); cit != m_expiredRenderers.cend(); ++cit)
        {
            delete cit->first;

            if (!m_systemManager.DestroyWindow(cit->second))
            {
                LOG_WARNING("Failed to destroy window %s", cit->second->GetName().c_str());

                delete cit->second;
            }
        }

        m_expiredRenderers.clear();
    }
}

Renderer* Graphics::SpawnRenderer(system::Window* window)
{
    vulkan::Renderer* renderer = nullptr;
    switch (m_driver)
    {
    case DriverType::Vulkan:
        renderer = new vulkan::Renderer(m_systemManager, window);
        renderer->Init();
        BindWindowRenderer(window, renderer);
        break;
    default:
        LOG_ERROR("This render type not exist");
        break;
    }
    return renderer;
}
}
}
