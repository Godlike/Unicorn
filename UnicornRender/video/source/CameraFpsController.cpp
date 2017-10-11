/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraFpsController.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <unicorn/utility/Logger.hpp>

#include <algorithm>

namespace unicorn
{
namespace video
{

CameraFpsController::CameraFpsController(glm::mat4& cameraView)
    : CameraController(cameraView)
    , sensitivityX(0.005f)
    , sensitivityY(0.005f)
    , sensitivityZ(0.005f)
    , m_mousePosition(0)
    , m_pitch(0.0f)
    , m_roll(0.0f)
    , m_yaw(0.0f)
    , m_quat({ m_pitch, m_yaw, m_roll })
    , m_dirtyViewPosition(true)
    , m_lastX(0)
    , m_lastY(0)
{
}

void CameraFpsController::Roll(float degrees)
{
    m_roll += degrees * sensitivityZ;
    LOG_ERROR( "ROLL degress %f" , m_roll);
    m_isDirty = true;
}

void CameraFpsController::Yaw(float degrees)
{
    m_yaw += degrees * sensitivityY;
    LOG_ERROR("YAW degress %f", m_yaw);
    m_isDirty = true;
}

void CameraFpsController::Pitch(float degrees)
{
    m_pitch += degrees * sensitivityX;
    LOG_ERROR("PITCH degress %f", m_pitch);
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
    m_position += m_rightVector * distance;
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
    double xoffset = m_lastX - x;
    double yoffset = y - m_lastY;

    m_lastX = x;
    m_lastY = y;

    if (!m_dirtyViewPosition)
    {
        m_isDirty = true;
    }

    xoffset *= sensitivityX;
    yoffset *= sensitivityY;

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
    glm::quat p = glm::angleAxis(m_pitch, m_rightVector);
    glm::quat y = glm::angleAxis(m_yaw, m_upVector);
    glm::quat r = glm::angleAxis(m_roll, m_direction);

    m_pitch = m_yaw = m_roll = 0;

    m_quat = glm::normalize(p * y * r * m_quat);

    m_direction = conjugate(m_quat) * glm::vec3(0, 0, 1);
    m_upVector = conjugate(m_quat) * glm::vec3(0, -1, 0);
    m_rightVector = cross(m_direction, m_upVector);

    LOG_ERROR("QUATERNION w %f x %f y %f z %f ", m_quat.w, m_quat.x, m_quat.y, m_quat.z);
    LOG_ERROR("DIRECTION x %f y %f z %f ", m_direction.x, m_direction.y, m_direction.z);
    //LOG_ERROR("UPVECTOR x %f y %f z %f ", m_upVector.x, m_upVector.y, m_upVector.z);
    //LOG_ERROR("RIGHT  x %f y %f z %f ", m_rightVector.x, m_rightVector.y, m_rightVector.z);

    m_cameraView = mat4_cast(m_quat) * glm::translate(glm::mat4(1.0f), m_position);
}

} // namespace video
} // namespace unicorn
