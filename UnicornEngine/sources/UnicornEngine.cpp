/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <unicorn/UnicornEngine.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/graphics/Graphics.hpp>
#include <unicorn/graphics/Camera.hpp>
#include <unicorn/graphics/SceneGraph.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>
#include <unicorn/system/Window.hpp>


uc::UnicornEngine::UnicornEngine()
        : m_pSceneGraph(nullptr),
        m_isInitialized(false), 
        m_pWindow(nullptr),
        m_pGraphics(nullptr),
        m_pCamera(nullptr)
    {
    }

uc::UnicornEngine::~UnicornEngine()
{
    Deinit();
}

bool uc::UnicornEngine::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    uc::utility::asset::SimpleStorage::Instance();
    LOG_INFO("Engine initialization started.");

    m_pWindow = new system::Window;
    m_pGraphics = new graphics::Graphics;
    m_pCamera = new graphics::Camera;
    m_pSceneGraph = new graphics::SceneGraph;

    m_pCamera->SetPerspective(45.0f, 45.0f, 0.1f, 1000.0f);
    m_pCamera->SetPosition({0.0f, 0.0f, 0.0f});
    
    if (!m_pWindow->Init())
    {
        Deinit();
        return false;
    }

    if (!m_pGraphics->Init(*m_pWindow))
    {
        Deinit();
        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Engine initialization finished.");

    return true;
}

void uc::UnicornEngine::Deinit()
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

void uc::UnicornEngine::Run()
{
    while (!m_pWindow->ShouldClose())
    {
        m_pWindow->RetrieveEvents();
        //m_pGraphics->Render(m_pCamera, m_pSceneGraph);
    }
}
