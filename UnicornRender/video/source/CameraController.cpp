/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraController.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

void CameraController::SetDirection(const glm::vec3& direction)
{
    m_direction = direction;
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));
    m_isDirty = true;
}

void CameraController::SetUpVector(const glm::vec3& upVector)
{
    m_upVector = upVector;
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));
    m_isDirty = true;
}

void CameraController::SetPosition(const glm::vec3& position)
{
    m_position = position;
    m_isDirty = true;
}

glm::vec3 const& CameraController::GetDirection() const
{
    return m_direction;
}

glm::vec3 const& CameraController::GetUpVector() const
{
    return m_upVector;
}

glm::vec3 const& CameraController::GetPosition() const
{
    return m_position;
}

void CameraController::Recalculate()
{
    if (m_isDirty)
    {
        UpdateViewMatrix();
        m_isDirty = false;
    }
}

CameraController::CameraController(glm::mat4& cameraView) :
    m_cameraView(cameraView),
    m_position(0.0),
    m_upVector(0.0f, -1.0f, 0.0f),
    m_direction(0.0),
    m_rightVector(glm::cross(m_direction, m_upVector)),
    m_isDirty(true)
{
}

void CameraController::UpdateViewMatrix()
{
    m_cameraView = glm::lookAt(m_position,
                               m_position + m_direction,
                               m_upVector);
}

} // namespace video
} // namespace unicorn
