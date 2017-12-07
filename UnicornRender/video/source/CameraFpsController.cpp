/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraFpsController.hpp>

namespace unicorn
{
namespace video
{

CameraFpsController::CameraFpsController(glm::mat4& cameraView) :
    m_mousePosition(0)
    , m_dirtyViewPosition(true)
    , m_cameraView(cameraView)
{
}

void CameraFpsController::UpdateView(float x, float y)
{
    const float xoffset = m_mousePosition.x - x;
    const float yoffset = y - m_mousePosition.y;

    m_mousePosition.x = x;
    m_mousePosition.y = y;

    if (m_dirtyViewPosition)
    {
        m_dirtyViewPosition = false;
        return;
    }

    m_rotation.x += glm::radians(yoffset);
    m_rotation.y += glm::radians(xoffset);

    m_isDirty = true;
}

void CameraFpsController::SetViewPositions(float x, float y)
{
    m_mousePosition = glm::vec2(x, y);
}

void CameraFpsController::ResetView()
{
    m_dirtyViewPosition = true;
}

void CameraFpsController::UpdateOrientation()
{
    glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);
    glm::quat y = glm::angleAxis(m_rotation.y, conjugate(m_orientation) * m_worldY);

    m_orientation = normalize(m_orientation * y * x);

    m_rotation = glm::vec3(0);
}

void CameraFpsController::Update()
{
    UpdateModelMatrix();
    m_cameraView = m_transformMatrix;
}

} // namespace video
} // namespace unicorn
