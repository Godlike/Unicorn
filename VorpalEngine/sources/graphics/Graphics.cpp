/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <vorpal/graphics/Graphics.hpp>
//#include <vorpal/graphics/Vulkan/VulkanRenderer.hpp>
#include <vorpal/graphics/vulkan/VulkanDevice.hpp>
#include <vorpal/utility/Logger.hpp>

namespace vp
{
namespace graphics
{
Graphics::Graphics()
    : m_isInitialized(false)
    //, m_pRenderer(nullptr)
{
}

Graphics::~Graphics()
{
    Deinit();
}

bool Graphics::Init(const system::Window& renderWindow)
{
    if (m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Graphics initialization started.");

    m_pVkDevice = new VulkanDevice;
    //m_pRenderer = new VulkanRenderer;
    // Init Subsystem -> OpenGL / Vulkan
    m_pVkDevice->Init(renderWindow);
    
    /*if (!m_pRenderer->Init())
    {
        Deinit();

        return false;
    }*/

    m_isInitialized = true;

    LOG_INFO("Graphics initialized correctly.");

    return true;
}

void Graphics::Deinit()
{
    /*if (m_pRenderer)
    {
        m_pRenderer->Deinit();

        delete m_pRenderer;

        m_pRenderer = nullptr;
    }
*/

    if (m_isInitialized)
    {
        LOG_INFO("Graphics shutdown correctly.");
    }

    m_isInitialized = false;
}

void Graphics::Render()
{
    /*if (m_isInitialized && m_pRenderer)
    {
        m_pRenderer->Render();
    }*/
}

void Graphics::WindowSizeChanged()
{
   /* if (m_pRenderer)
    {
        m_pRenderer->RecreateSwapChain();
    }*/
}
}
}
