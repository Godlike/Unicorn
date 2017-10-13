/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera2DController.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

Camera2DController::Camera2DController(glm::mat4& cameraView) :
    CameraController(cameraView)
{
}

void Camera2DController::MoveUp(float distance)
{
    m_position += glm::vec3(0.0f, -distance, 0.0f);
    m_isDirty = true;
}

void Camera2DController::MoveDown(float distance)
{
    m_position += glm::vec3(0.0f, distance, 0.0f);
    m_isDirty = true;
}

void Camera2DController::MoveLeft(float distance)
{
    m_position -= m_rightVector * distance;
    m_isDirty = true;
}

void Camera2DController::MoveRight(float distance)
{
    m_position += m_rightVector * distance;
    m_isDirty = true;
}

void Camera2DController::UpdateViewMatrix()
{    
    m_cameraView = glm::lookAt(m_position,
        m_position + m_direction,
        m_upVector);    
}

} // namespace video
} // namespace unicorn
