/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraFpsController.hpp>

#include <unicorn/utility/Logger.hpp>

#include <algorithm>

namespace unicorn
{
namespace video
{

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView)
    , m_mousePosition(0)
    , m_pitch(0.0f)
    , m_roll(0.0f)
    , m_yaw(0.0f)
    , m_dirtyViewPosition(true)
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

    m_yaw += xoffset;
    m_pitch += yoffset;

    m_isDirty = true;
}

void CameraFpsController::SetViewPositions(double x, double y)
{
    m_mousePosition = glm::vec2(x, y);
}

void CameraFpsController::UpdateViewMatrix()
{    
    Rotate(m_pitch, m_rightVector);
    Rotate(m_yaw, m_upVector);
    Rotate(m_roll, m_direction);

    m_pitch = m_yaw = m_roll = 0;

    m_cameraView = GetViewMatrix();
}

} // namespace video
} // namespace unicorn
