/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraFpsController.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>

namespace unicorn
{
namespace video
{

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView)
    , sensitivityX(0.005f)
    , sensitivityY(0.005f)
    , m_mousePosition(0)
    , m_quat(0.0, 0.0, 0.0, 0.0)
    , m_pitch(0)
    , m_roll(0)
    , m_yaw(0)
    , m_dirtyViewPosition(true)
{
}

void CameraFpsController::Roll(float degrees)
{
    m_roll += degrees;
    m_isDirty = true;
}

void CameraFpsController::MoveUp(float distance)
{
    m_position += glm::vec3(0.0f, distance, 0.0f);
    m_isDirty = true;
}

void CameraFpsController::MoveDown(float distance)
{
    m_position += glm::vec3(0.0f, -distance, 0.0);
    m_isDirty = true;
}

void CameraFpsController::MoveLeft(float distance)
{
    m_position -= m_rightVector * distance;
    m_isDirty = true;
}

void CameraFpsController::MoveRight(float distance)
{
    m_position +=m_rightVector * distance;
    m_isDirty = true;
}

void CameraFpsController::MoveForward(float distance)
{
    m_position += m_direction * distance;
    m_isDirty = true;
}

void CameraFpsController::MoveBackward(float distance)
{
    m_position -= m_direction * distance;
    m_isDirty = true;
}

void CameraFpsController::UpdateView(double x, double y)
{
    glm::vec2 mouseDelta = glm::vec2(x, y) - m_mousePosition;
    m_mousePosition = glm::vec2(x, y);

    if(m_dirtyViewPosition)
    {
        m_dirtyViewPosition = false;
        return;
    }

    m_yaw = mouseDelta.x * sensitivityX;
    m_pitch = mouseDelta.y * sensitivityY;

    m_isDirty = true;
}

void CameraFpsController::SetViewPositions(double x, double y)
{
    m_mousePosition = glm::vec2(x, y);
}

void CameraFpsController::UpdateViewMatrix()
{
    glm::quat quat = glm::quat(glm::vec3(m_pitch, m_yaw, m_roll));

    m_pitch = m_yaw = m_roll = 0;

    m_quat = quat * m_quat;
    m_quat = glm::normalize(m_quat);

    m_direction = m_direction * m_quat;

    glm::mat4 rotate = glm::mat4_cast(m_quat);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, m_position);

    m_cameraView = rotate * translate;
}

} // namespace video
} // namespace unicorn
