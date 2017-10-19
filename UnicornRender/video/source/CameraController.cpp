/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraController.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace unicorn
{
namespace video
{

void CameraController::SetDirection(glm::vec3 direction)
{
    m_direction = direction;
    m_isDirty = true;
}

void CameraController::SetUpVector(glm::vec3 upVector)
{
    m_upVector = upVector;
    m_isDirty = true;
}

void CameraController::SetPosition(glm::vec3 position)
{
    m_position = position;
    m_isDirty = true;
}

glm::vec3 CameraController::GetDirection() const
{
    return m_direction;
}

glm::vec3 CameraController::GetRight() const
{
    return m_rightVector;
}

glm::vec3 CameraController::GetUp() const
{
    return m_upVector;
}

glm::vec3 CameraController::GetPosition() const
{
    return m_position;
}

void CameraController::Translate(glm::vec3 translate)
{
    m_position += translate;
    m_isDirty = true;
}

void CameraController::TranslateLocalX(float distance)
{
    Translate(m_rightVector * distance);
}

void CameraController::TranslateLocalY(float distance)
{
    Translate(m_upVector * distance);
}

void CameraController::TranslateLocalZ(float distance)
{
    Translate(m_direction * distance);
}

void CameraController::TranslateWorldX(float distance)
{
    glm::vec3 worldX = { 1.0, 0.0, 0.0 };
    Translate(worldX * distance);
}

void CameraController::TranslateWorldY(float distance)
{
    glm::vec3 worldY = { 0.0, 1.0, 0.0 };
    Translate(worldY * distance);
}

void CameraController::TranslateWorldZ(float distance)
{
    glm::vec3 worldZ = { 0.0, 0.0, 1.0 };
    Translate(worldZ * distance);
}

void CameraController::RotateX(float radians)
{
    m_rotation.x += radians;
    m_isDirty = true;
}

void CameraController::RotateY(float radians)
{
    m_rotation.y += radians;
    m_isDirty = true;
}

void CameraController::RotateZ(float radians)
{
    m_rotation.z += radians;
    m_isDirty = true;
}

void CameraController::Rotate(glm::vec3 rotation)
{
    m_rotation += rotation;
    m_isDirty = true;
}

void CameraController::RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point)
{
    glm::vec3 dir = point - m_position;
    Translate(dir);
    glm::quat q = angleAxis(radians, axis);
    Translate(q * -dir);
    m_isDirty = true;
}

void CameraController::Update()
{
    if (m_isDirty)
    {
        UpdateViewMatrix();
        m_isDirty = false;
    }
}

void CameraController::UpdateForce()
{
    UpdateViewMatrix();
}

CameraController::CameraController(glm::mat4& cameraView)
    : m_cameraView(cameraView)
    , m_rotation(0.0)
    , m_position(0.0)
    , m_upVector(0.0f, 1.0f, 0.0f)
    , m_direction(0.0f, 0.0f, 1.0f)
    , m_rightVector(1.0, 0.0f, 0.0f)
    , m_orientation(m_rotation)
    , m_isDirty(true)
{
}

/** @brief Recalculates view matrix */

void CameraController::UpdateViewMatrix()
{   
    glm::quat x = glm::angleAxis(m_rotation.y, glm::vec3(1.0, 0.0, 0.0));
    glm::quat y = glm::angleAxis(m_rotation.x, glm::vec3(0.0, 1.0, 0.0));
    glm::quat z = glm::angleAxis(m_rotation.z, glm::vec3(0.0, 0.0, 1.0));

    m_orientation = x * y * z * m_orientation;

    m_rotation.x = 0;
    m_rotation.y = 0;
    m_rotation.z = 0;

    m_direction = m_orientation * glm::vec3(0.0, 0.0, 1.0);
    m_rightVector = m_orientation * glm::vec3(1.0, 0.0, 0.0);
    m_upVector = glm::cross(m_direction, m_rightVector);

    m_cameraView = glm::mat4_cast(m_orientation) * glm::translate(glm::mat4(1.0), -m_position); // but why

}

} // namespace video
} // namespace unicorn
