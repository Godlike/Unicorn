/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraFpsController.hpp>

#include <algorithm>

namespace unicorn
{
namespace video
{

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView),
    sensitivity(0.1f),
    m_lastX(0.f),
    m_lastY(0.f),
    m_yaw(90.0),
    m_pitch(0.0),
    m_dirty(true)
{
}

void CameraFpsController::MoveUp(float deltaTime)
{
    m_position += glm::vec3(0.0f, -deltaTime, 0.0f);
    m_isDirty = true;
}

void CameraFpsController::MoveDown(float deltaTime)
{
    m_position += glm::vec3(0.0f, deltaTime, 0.0);
    m_isDirty = true;
}

void CameraFpsController::MoveLeft(float deltaTime)
{
    m_position -= m_rightVector * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveRight(float deltaTime)
{
    m_position +=m_rightVector * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveForward(float deltaTime)
{
    m_position += m_direction * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::MoveBackward(float deltaTime)
{
    m_position -= m_direction * deltaTime;
    m_isDirty = true;
}

void CameraFpsController::UpdateView(double posX, double posY)
{
    const double xoffset = (m_lastX - posX) * sensitivity;
    const double yoffset = (posY - m_lastY) * sensitivity;

    m_lastX = posX;
    m_lastY = posY;
    if(m_dirty)
    {
        m_dirty = false;
        return;
    }
    m_yaw += xoffset;
    m_pitch += yoffset;

    m_pitch = std::max(std::min(m_pitch, 89.0), -89.0);

    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    front.y = static_cast<float>(sin(glm::radians(m_pitch)));
    front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_direction = glm::normalize(front);
    m_rightVector = glm::normalize(glm::cross(m_direction, m_upVector));

    m_isDirty = true;
}

void CameraFpsController::UpdateViewPositions(double posX, double posY)
{
    m_lastX = posX;
    m_lastY = posY;
}

} // namespace video
} // namespace unicorn
