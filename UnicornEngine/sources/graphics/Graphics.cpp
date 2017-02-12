/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/graphics/Graphics.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/graphics/opengl/OpenGLGraphicsSubsystem.hpp>
#include <unicorn/graphics/vulkan/VulkanGraphicsSubsystem.hpp>

uc::graphics::Graphics::Graphics()
    : m_isInitialized(false)
    , m_pRenderer(nullptr)
{
}

uc::graphics::Graphics::~Graphics()
{
    Deinit();
}

bool uc::graphics::Graphics::Init(const system::Window& renderWindow, GraphicsSubsystemType renderType)
{
    if (m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Graphics initialization started.");

    switch (renderType)
    {
    case uc::graphics::GraphicsSubsystemType::OPENGL:
        m_pGraphicsSubsystem = new OpenGLGraphicsSubsystem;
        break;
    case uc::graphics::GraphicsSubsystemType::VULKAN:
        m_pGraphicsSubsystem = new VulkanGraphicsSubsystem;
        break;
    default:
        break;
    }    
    
    m_isInitialized = true;

    LOG_INFO("Graphics initialized correctly.");

    return true;
}

void uc::graphics::Graphics::Deinit()
{
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
