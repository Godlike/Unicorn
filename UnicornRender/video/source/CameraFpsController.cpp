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
    glm::quat y = glm::angleAxis(m_rotation.y, m_orientation * m_worldY);

    m_orientation = normalize(m_orientation * y * x);

    m_rotation = glm::vec3(0);
}

void CameraFpsController::Update()
{
    if (m_isDirty)
    {
        UpdateOrientation();

        m_direction = m_orientation * m_worldZ;
        m_upVector = m_orientation * m_worldY;
        m_rightVector = m_orientation * m_worldX;

        m_cameraView = glm::mat4(1.0);

        m_cameraView[0][0] = m_rightVector.x;
        m_cameraView[1][0] = m_rightVector.y;
        m_cameraView[2][0] = m_rightVector.z;
        m_cameraView[0][1] = m_upVector.x;
        m_cameraView[1][1] = m_upVector.y;
        m_cameraView[2][1] = m_upVector.z;
        m_cameraView[0][2] = m_direction.x;
        m_cameraView[1][2] = m_direction.y;
        m_cameraView[2][2] = m_direction.z;
        m_cameraView[3][0] = -dot(m_rightVector, m_translation);
        m_cameraView[3][1] = -dot(m_upVector, m_translation);
        m_cameraView[3][2] = -dot(m_direction, m_translation); // TODO: FIX CAMERA
    }
}

} // namespace video
} // namespace unicorn
