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
#include <glm/gtx/norm.hpp>

namespace unicorn
{
namespace video
{

glm::quat RotationBetweenVectors(glm::vec3 v0, glm::vec3 v1)
{
    glm::quat q = glm::quat(glm::vec3(0));

    v0 = normalize(v0);
    v1 = normalize(v1);

    const float d = dot(v0, v1);
    const glm::vec3 c = cross(v0, v1);
    const float s = sqrt((1 + d) * 2);

    q.x = c.x / s;
    q.y = c.y / s;
    q.z = c.z / s;
    q.w = s / 2.0f;

    return q;
}

void CameraController::SetOrientation(glm::vec3 direction, glm::vec3 upvector)
{
    glm::mat4 mat = lookAt(m_position, m_position + direction, upvector);
    m_orientation = normalize(quat_cast(mat));

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
    Translate(m_worldX * distance);
}

void CameraController::TranslateWorldY(float distance)
{
    Translate(m_worldY * distance);
}

void CameraController::TranslateWorldZ(float distance)
{
    Translate(m_worldZ * distance);
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
        CalculateOrientation();
        UpdateViewMatrix();
        m_isDirty = false;
    }
}

void CameraController::ForceUpdate()
{
    UpdateViewMatrix();
}

CameraController::CameraController(glm::mat4& cameraView)
    : m_cameraView(cameraView)
    , m_rotation(0.0)
    , m_position(0.0)
    , m_upVector(0.0f, 1.0f, 0.0f)
    , m_direction(0.0f, 0.0f, 1.0f)
    , m_worldX(1.f, 0.f, 0.f)
    , m_worldY(0.f, 1.f, 0.f)
    , m_worldZ(0.f, 0.f, 1.f)
    , m_rightVector(1.0, 0.0f, 0.0f)
    , m_orientation(m_rotation)
    , m_isDirty(true)
{
}

/** @brief Recalculates view matrix */

void CameraController::UpdateViewMatrix()
{
    m_direction = glm::conjugate(m_orientation) * m_worldZ;
    m_upVector = glm::conjugate(m_orientation) * m_worldY;
    m_rightVector = glm::cross(m_upVector, m_direction);
    
    //m_orientation = glm::quat(glm::vec3(0));

    m_cameraView = glm::lookAt(m_position, m_position + m_direction, m_upVector);
}

void CameraController::CalculateOrientation()
{
    glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);
    glm::quat y = glm::angleAxis(m_rotation.y, m_worldY);
    glm::quat z = glm::angleAxis(m_rotation.z, m_worldZ);

    m_orientation = z * x * y * m_orientation;

    m_rotation = { 0 };
}

} // namespace video
} // namespace unicorn
