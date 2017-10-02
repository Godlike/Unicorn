/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraProjection.hpp>

namespace unicorn
{
namespace video
{

CameraProjection::CameraProjection(system::Window* window, glm::mat4& cameraProj) :
    m_aspect(static_cast<float>(window->GetSize().first) / window->GetSize().second),
    m_cameraProjection(cameraProj),
    m_pWindow(window)
{
    m_pWindow->Destroyed.connect(this, &CameraProjection::OnWindowDestroyed);
    m_pWindow->SizeChanged.connect(this, &CameraProjection::OnWindowSizeChanged);
}

CameraProjection::~CameraProjection()
{
    if(m_pWindow != nullptr)
    {
        m_pWindow->SizeChanged.disconnect(this, &CameraProjection::OnWindowSizeChanged);
        m_pWindow->Destroyed.disconnect(this, &CameraProjection::OnWindowDestroyed);
    }
}

void CameraProjection::OnWindowDestroyed(system::Window*)
{
    m_pWindow = nullptr;
}

void CameraProjection::OnWindowSizeChanged(system::Window*, std::pair<int32_t, int32_t> size)
{
    m_aspect = static_cast<float>(size.first) / static_cast<float>(size.second);
}

} // namespace video
} // namespace unicorn
