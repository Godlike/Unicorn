/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Graphics.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

namespace unicorn
{
UnicornEngine::UnicornEngine()
    : m_isInitialized(false)
    , m_pGraphics(nullptr)
{
}

UnicornEngine::~UnicornEngine()
{
    Deinit();
}

bool UnicornEngine::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    unicorn::utility::asset::SimpleStorage::Instance();
    LOG_INFO("Engine initialization started.");

    m_pGraphics = new video::Graphics();

    if (!m_pGraphics->Init())
    {
        Deinit();

        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Engine initialization finished.");

    return true;
}

void UnicornEngine::Deinit()
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

void UnicornEngine::Run()
{
    if (m_pGraphics)
    {
        m_pGraphics->Render();
    }
}
}
