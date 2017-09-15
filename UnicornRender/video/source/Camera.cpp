/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace unicorn
{
namespace video
{

void CameraController::SetDirection(const glm::vec3 & direction)
{
    m_direction = direction;
    UpdateViewMatrix();
}

void CameraController::SetUpVector(const glm::vec3 & upVector)
{
    m_upVector = upVector;
    UpdateViewMatrix();
}

void CameraController::SetPosition(const glm::vec3 & position)
{
    m_position = position;
    UpdateViewMatrix();
}

CameraController::CameraController(glm::mat4 & cameraView) :
    m_cameraView(cameraView),
    m_position(0.0),
    m_direction(0.0),
    m_upVector(0.0f, -1.0f, 0.0f),
    speed(100.f)
{
}

void CameraController::UpdateViewMatrix()
{
    m_cameraView = glm::lookAt(m_position,
                               m_position + m_direction,
                               m_upVector);
}

CameraFpsController::CameraFpsController(glm::mat4& cameraView) 
    : CameraController(cameraView),
    sensitivity(0.1f),
    m_lastX(0.0),
    m_lastY(0.0),
    m_yaw(90.0),
    m_pitch(0.0),
    m_dirty(false)
{
}

void CameraFpsController::MoveUp(float deltaTime)
{
    m_position += glm::vec3(0.0f, -deltaTime * speed, 0.0f); 
    UpdateViewMatrix();
}

void CameraFpsController::MoveDown(float deltaTime)
{
    m_position += glm::vec3(0.0f, speed * deltaTime, 0.0);
    UpdateViewMatrix();
}

void CameraFpsController::MoveLeft(float deltaTime)
{
    m_position -= glm::normalize(glm::cross(m_direction, m_upVector)) * speed * deltaTime; //TODO: optimize crossproduct
    UpdateViewMatrix();
}

void CameraFpsController::MoveRight(float deltaTime)
{
    m_position += glm::normalize(glm::cross(m_direction, m_upVector)) * speed * deltaTime;
    UpdateViewMatrix();;
}

void CameraFpsController::MoveForward(float deltaTime)
{
    m_position += m_direction * speed * deltaTime;
    UpdateViewMatrix();
}

void CameraFpsController::MoveBackward(float deltaTime)
{
    m_position -= m_direction * speed * deltaTime;
    UpdateViewMatrix();
}

void CameraFpsController::UpdateView(double posX, double posY)
{
    if (!m_dirty)
    {
        m_lastX = posX;
        m_lastY = posY;
        m_dirty = true;
    }

    double xoffset = m_lastX - posX;
    double yoffset = posY - m_lastY;
    m_lastX = posX;
    m_lastY = posY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    m_pitch = std::max(std::min(m_pitch, 89.0), -89.0);

    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    front.y = static_cast<float>(sin(glm::radians(m_pitch)));
    front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_direction = glm::normalize(front);

    UpdateViewMatrix();
}

}
}
