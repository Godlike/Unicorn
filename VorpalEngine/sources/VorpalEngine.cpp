/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <vorpal/VorpalEngine.hpp>
#include <vorpal/utility/Logger.hpp>
#include <vorpal/graphics/Graphics.hpp>
#include <vorpal/utility/asset/SimpleStorage.hpp>

namespace vp
{
VorpalEngine::VorpalEngine()
    : m_isInitialized(false)
    , m_pGraphics(nullptr)
{
}

VorpalEngine::~VorpalEngine()
{
    Deinit();
}

bool VorpalEngine::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    vp::utility::asset::SimpleStorage::Instance();
    LOG_INFO("Engine initialization started.");

    m_pGraphics = new graphics::Graphics();

    if (!m_pGraphics->Init())
    {
        Deinit();

        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Engine initialization finished.");

    return true;
}

void VorpalEngine::Deinit()
{
    if (m_pGraphics)
    {
        m_pGraphics->Deinit();

        delete m_pGraphics;

        m_pGraphics = nullptr;
    }

    if (m_isInitialized)
    {
        LOG_INFO("Engine shutdown correctly.");
    }

    m_isInitialized = false;
}

void VorpalEngine::Run()
{
    if (m_pGraphics)
    {
        m_pGraphics->Render();
    }
}
}
