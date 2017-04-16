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

#include <unicorn/video/vulkan/VulkanInstance.hpp>
#include <unicorn/video/vulkan/VulkanRenderer.hpp>

namespace unicorn
{
namespace video
{
Graphics::Graphics(system::Manager& manager)
    : m_isInitialized(false)
    , m_systemManager(manager)
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

    if (!m_systemManager.IsVulkanSupported())
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
                m_systemManager.PollEvents();
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
    system::Window* pWindow = m_systemManager.CreateWindow(width, height, name, pMonitor, pSharedWindow);
    Renderer* pRenderer = new Renderer(m_systemManager, pWindow);

    if (pRenderer->Init())
    {
        LOG_DEBUG("Created renderer for window %s", name.c_str());

        m_renderers.insert(RendererWindowPair(pRenderer, pWindow));
    }
    else
    {
        LOG_WARNING("Failed to initialize new renderer for window %s", name.c_str());

        if (!m_systemManager.DestroyWindow(pWindow))
        {
            LOG_WARNING("Failed to destroy window %s", name.c_str());

            delete pWindow;
        }
    }

    LOG_INFO("Graphics initialization started.");

    return pWindow;
}

const std::vector<system::Monitor*>& Graphics::GetMonitors() const
{
    return m_systemManager.GetMonitors();
}

system::Monitor* Graphics::GetWindowMonitor(const system::Window& window) const
{
    return m_systemManager.GetWindowMonitor(window);
}

void Graphics::SetWindowMonitor(const system::Window& window,
    system::Monitor* pMonitor,
    std::pair<int32_t, int32_t> position,
    std::pair<int32_t, int32_t> size,
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

Renderer* Graphics::SpawnVulkanRenderer()
{
    return nullptr;
}

bool Graphics::CreateVulkanContext()
{
    VulkanInstance& vulkanInstance = VulkanInstance::Instance(m_systemManager);
    return vulkanInstance.Initialize();
}
}
}
