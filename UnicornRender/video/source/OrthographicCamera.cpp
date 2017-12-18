/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/OrthographicCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

OrthographicCamera::OrthographicCamera(system::Window& window, glm::mat4& cameraProj)
    : CameraProjection(window, cameraProj)
    , m_orthoScale(DEFAULT_ORTHO_BOX_SCALE)
    , m_scaledAspect(m_aspect / m_orthoScale)
    , m_right(static_cast<float>(m_pWindow->GetSize().first) / 2)
    , m_top(static_cast<float>(m_pWindow->GetSize().second) / 2)
    , m_back(BACK_ORTHO_FRUSTRUM_PLANE)
    , m_front(FRONT_ORTHO_FRUSTRUM_PLANE)
{
    UpdateProjection();
}

void OrthographicCamera::UpdateProjection()
{
    m_right = static_cast<float>(m_pWindow->GetSize().first) / 2;
    m_top = static_cast<float>(m_pWindow->GetSize().second) / 2;

    float const xScaled = m_right * m_scaledAspect;
    float const yScaled = m_top * m_scaledAspect;

    m_cameraProjection = glm::ortho(-xScaled, xScaled,
                                    -yScaled, yScaled,
                                    m_back, m_front);
}

void OrthographicCamera::SetScale(float scale)
{
    m_orthoScale = std::max(scale, 1.0f);
    m_scaledAspect = m_aspect / m_orthoScale;
    UpdateProjection();
}

float OrthographicCamera::GetScale() const
{
    return m_orthoScale;
}

} // namespace video
} // namespace unicorn
