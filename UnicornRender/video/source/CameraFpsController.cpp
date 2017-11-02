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

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView)
    , m_mousePosition(0)
    , m_dirtyViewPosition(true)
{
}

void CameraFpsController::UpdateView(float x, float y)
{
    const float xoffset = x - m_mousePosition.x;
    const float yoffset = m_mousePosition.y - y;

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

void CameraFpsController::SetViewPositions(double x, double y)
{
    m_mousePosition = glm::vec2(x, y);
}

void CameraFpsController::ResetView()
{
    m_dirtyViewPosition = false;
}

// void CameraFpsController::CalculateOrientation()
// {
//     glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);
//     glm::quat y = glm::angleAxis(m_rotation.y, m_orientation * m_worldY);

//     m_orientation = x * y * m_orientation;

//     m_rotation = glm::vec3(0);
// }


} // namespace video
} // namespace unicorn
