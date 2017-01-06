#include <vorpal/video/Graphics.hpp>

#include <vorpal/video/Renderer.hpp>

#include <vorpal/utility/Logger.hpp>

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