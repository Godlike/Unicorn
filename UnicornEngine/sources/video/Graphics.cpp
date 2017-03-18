/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Graphics.hpp>
#include <unicorn/video/Renderer.hpp>
#include <unicorn/utility/Logger.hpp>

namespace vp
{
namespace video
{
Graphics::Graphics()
    : m_isInitialized(false)
    , m_pRenderer(nullptr)
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

    m_pRenderer = new Renderer();

    if (!m_pRenderer->Init())
    {
        Deinit();

        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Graphics initialized correctly.");

    return true;
}

void Graphics::Deinit()
{
    if (m_pRenderer)
    {
        m_pRenderer->Deinit();

        delete m_pRenderer;

        m_pRenderer = nullptr;
    }

    if (!m_isInitialized)
    {
        LOG_INFO("Graphics shutdown correctly.");
    }

    m_isInitialized = false;
}

void Graphics::Render()
{
    if (m_isInitialized && m_pRenderer)
    {
        m_pRenderer->Render();
    }
}
}
}
