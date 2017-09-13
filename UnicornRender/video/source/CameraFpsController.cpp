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
CameraFpsController::CameraFpsController(unicorn::video::Camera& camera)
    : sensitivity(0.1f), 
    speed(100.0f), 
    m_lastX(0.0), 
    m_lastY(0.0), 
    m_yaw(90.0),
    m_pitch(0.0), 
    m_dirty(false),
    m_camera(camera)
{
}

void CameraFpsController::MoveUp(float deltaTime) const
{
    m_camera.Translate({0.0f, -deltaTime * speed, 0.0f});
}

void CameraFpsController::MoveDown(float deltaTime) const
{
    m_camera.Translate({0.0f, speed * deltaTime, 0.0});
}

void CameraFpsController::MoveLeft(float deltaTime) const
{
    m_camera.Translate(-glm::normalize(glm::cross(m_camera.GetDirection(), m_camera.GetUpVector())) * speed * deltaTime);
}

void CameraFpsController::MoveRight(float deltaTime) const
{
    m_camera.Translate(glm::normalize(glm::cross(m_camera.GetDirection(), m_camera.GetUpVector())) * speed * deltaTime);
}

void CameraFpsController::MoveForward(float deltaTime) const
{
    m_camera.Translate(m_camera.GetDirection() * speed * deltaTime);
}

void CameraFpsController::MoveBackward(float deltaTime) const
{
    m_camera.Translate(-m_camera.GetDirection() * speed * deltaTime);
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
    m_camera.SetDirection(glm::normalize(front));
}

void CameraFpsController::Scroll(float yoffset) const
{
    //m_camera.SetFov(m_camera.GetFov() - yoffset);
}

}
}
