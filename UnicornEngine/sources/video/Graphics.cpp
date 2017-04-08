/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/utility/Logger.hpp>

#include <unicorn/window_manager/Hub.hpp>
#include <unicorn/window_manager/Window.hpp>

namespace unicorn
{
namespace video
{
Graphics::Graphics(WindowManager::Hub& windowManagerHub)
    : m_isInitialized(false)
    , m_windowManagerHub(windowManagerHub)
{
}

Graphics::~Graphics()
{
    Deinit();
}

bool Graphics::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Graphics initialization started.");

    if (!m_windowManagerHub.IsVulkanSupported())
    {
        LOG_ERROR("Vulkan not supported!");

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
                m_windowManagerHub.PollEvents();
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

WindowManager::Window* Graphics::SpawnWindow(int32_t width,
    int32_t height,
    const std::string& name,
    WindowManager::Monitor* pMonitor,
    WindowManager::Window* pSharedWindow)
{
    WindowManager::Window* pWindow = m_windowManagerHub.CreateWindow(width, height, name, pMonitor, pSharedWindow);
    Renderer* pRenderer = new Renderer(m_windowManagerHub, pWindow);

    if (pRenderer->Init())
    {
        LOG_DEBUG("Created renderer for window %s", name.c_str());

        m_renderers.insert(RendererWindowPair(pRenderer, pWindow));
    }
    else
    {
        LOG_WARNING("Failed to initialize new renderer for window %s", name.c_str());

        if (!m_windowManagerHub.DestroyWindow(pWindow))
        {
            LOG_WARNING("Failed to destroy window %s", name.c_str());

            delete pWindow;
        }
    }

    LOG_INFO("Graphics initialization started.");

    return pWindow;
}

const std::vector<WindowManager::Monitor*>& Graphics::GetMonitors() const
{
    return m_windowManagerHub.GetMonitors();
}

void Graphics::ProcessExpiredRenderers()
{
    if (!m_expiredRenderers.empty())
    {
        for (RendererWindowPairSet::const_iterator cit = m_expiredRenderers.cbegin(); cit != m_expiredRenderers.cend(); ++cit)
        {
            delete cit->first;

            if (!m_windowManagerHub.DestroyWindow(cit->second))
            {
                LOG_WARNING("Failed to destroy window %s", cit->second->GetName().c_str());

                delete cit->second;
            }
        }

        m_expiredRenderers.clear();
    }
}

}
}
