/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/graphics/Graphics.hpp>
//#include <vorpal/graphics/Vulkan/VulkanRenderer.hpp>
#include <unicorn/graphics/vulkan/VulkanDevice.hpp>
#include <unicorn/utility/Logger.hpp>

uc::graphics::Graphics::Graphics()
    : m_isInitialized(false)
    , m_pRenderer(nullptr)
{
}

uc::graphics::Graphics::~Graphics()
{
    Deinit();
}

bool uc::graphics::Graphics::Init(const system::Window& renderWindow)
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

void uc::graphics::Graphics::Deinit()
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

void uc::graphics::Graphics::Render()
{
    /*if (m_isInitialized && m_pRenderer)
    {
        m_pRenderer->Render();
    }*/
}

void uc::graphics::Graphics::WindowSizeChanged()
{
   /* if (m_pRenderer)
    {
        m_pRenderer->RecreateSwapChain();
    }*/
}
