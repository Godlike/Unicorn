/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/IRenderer.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/video/Camera.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace video
{
IRenderer::IRenderer(system::Manager& manager, system::Window* window, Camera const& camera)
    : camera(&camera)
    , m_isInitialized(false)
    , m_fontAtlasRes(1024, 1024)
    , m_fontOversample(2, 2)
    , m_firstChar(' ')
    , m_charCount('~' - ' ')
    , m_fontSize(30)
    , m_systemManager(manager)
    , m_pWindow(window)
    , m_backgroundColor({ {0.0f, 0.0f, 0.0f, 0.0f} })
    , m_depthTestEnabled(true)
{
    if(m_pWindow == nullptr)
    {
        LOG_VIDEO->Error("Window pointer in nullptr!");
    }
}

IRenderer::~IRenderer()
{
    Destroyed.emit(this);
    Destroyed.clear();
}

void IRenderer::SetBackgroundColor(const glm::vec3& backgroundColor)
{
    m_backgroundColor[0] = backgroundColor.r;
    m_backgroundColor[1] = backgroundColor.g;
    m_backgroundColor[2] = backgroundColor.b;
    m_backgroundColor[3] = 1.0f;
}
}
}
