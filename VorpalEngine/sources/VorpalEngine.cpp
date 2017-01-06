#include <vorpal/VorpalEngine.hpp>

#include <vorpal/utility/Logger.hpp>

#include <vorpal/video/Graphics.hpp>

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
    if (m_isInitialized) return false;

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

void VorpalEngine::Deinit()
{
    if (m_pGraphics)
    {
        m_pGraphics->Deinit();

        delete m_pGraphics;

        m_pGraphics = nullptr;
    }

    m_isInitialized = false;

    LOG_INFO("Engine shutdown correctly.");
}

void VorpalEngine::Run()
{
    if (m_pGraphics)
    {
        m_pGraphics->Render();
    }
}

}
