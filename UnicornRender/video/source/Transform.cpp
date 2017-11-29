/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Transform.hpp>
#include <unicorn/utility/Math.hpp>

namespace unicorn
{
namespace video
{

Transform::Transform()
    : m_rotation(0)
    , m_translation(0)
    , m_orientation(glm::vec3(0))
    , m_upVector({ 0.f, 1.f, 0.f })
    , m_direction({ 0.f, 0.f, 1.f })
    , m_rightVector({ 1., 0.f, 0.f })
    , m_worldX({ 1., 0.f, 0.f })
    , m_worldY({ 0.f, 1.f, 0.f })
    , m_worldZ({ 0.f, 0.f, 1.f })
    , m_transformMatrix(1.f)
    , m_isDirty(true)
{
}

void Transform::LookAtDirection(glm::vec3 direction)
{
    LookAtDirection(direction, m_upVector);
}

void Transform::LookAtDirection(glm::vec3 direction, glm::vec3 upVector)
{
    m_orientation = utility::math::LookAt(direction, upVector);

    m_isDirty = true;
}

void Transform::SetUp(glm::vec3 upVector)
{
    m_orientation = utility::math::LookAt(m_direction, upVector);

    m_isDirty = true;
}

void Transform::SetTranslation(glm::vec3 translate)
{
    m_translation = translate;

    m_isDirty = true;
}

void Transform::SetWorldCoordinates(glm::vec3 x, glm::vec3 y, glm::vec3 z)
{
    m_worldX = x;
    m_worldY = y;
    m_worldZ = z;
}

glm::vec3 Transform::GetDirection() const
{
    return m_direction;
}

glm::vec3 Transform::GetRight() const
{
    return m_rightVector;
}

glm::vec3 Transform::GetUp() const
{
    return m_upVector;
}

glm::vec3 Transform::GetTranslate() const
{
    return m_translation;
}

glm::mat4 const& Transform::GetModelMatrix() const
{
    return m_transformMatrix;
}

void Transform::TranslateLocalX(float distance)
{
    SetTranslation(GetTranslate() + m_rightVector * distance);
}

void Transform::TranslateLocalY(float distance)
{
    SetTranslation(GetTranslate() + m_upVector * distance);
}

void Transform::TranslateLocalZ(float distance)
{
    SetTranslation(GetTranslate() + m_direction * distance);
}

void Transform::TranslateWorldX(float distance)
{
    SetTranslation(GetTranslate() + m_worldX * distance);
}

void Transform::TranslateWorldY(float distance)
{
    SetTranslation(GetTranslate() + m_worldY * distance);
}

void Transform::TranslateWorldZ(float distance)
{
    SetTranslation(GetTranslate() + m_worldZ * distance);
}

void Transform::RotateX(float radians)
{
    m_rotation.x += radians;

    m_isDirty = true;
}

void Transform::RotateY(float radians)
{
    m_rotation.y += radians;

    m_isDirty = true;
}

void Transform::RotateZ(float radians)
{
    m_rotation.z += radians;

    m_isDirty = true;
}

void Transform::Rotate(glm::vec3 rotation)
{
    m_rotation += rotation;
    m_isDirty = true;
}

void Transform::Rotate(glm::quat rotation)
{
    m_orientation = rotation * m_orientation;

    m_isDirty = true;
}

void Transform::RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point)
{
    glm::vec3 dir = point - m_translation;
    SetTranslation(GetTranslate() + dir);
    glm::quat q = angleAxis(radians, axis);
    SetTranslation(GetTranslate() + q * -dir);
    m_isDirty = true;
}

void Transform::Rotate(float angleRadians, glm::vec3 axis) {
    glm::quat q = glm::angleAxis(angleRadians, axis);
    Rotate(q);
}

void Transform::CalculateOrientation()
{
    glm::quat z = glm::angleAxis(m_rotation.z, m_worldZ);
    glm::quat y = glm::angleAxis(m_rotation.y, m_worldY);
    glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);

    m_orientation = normalize(m_orientation * z * x * y);

    m_rotation = glm::vec3(0);
}

void Transform::Update()
{
    if (m_isDirty)
    {
        CalculateOrientation();

        m_direction = m_orientation * m_worldZ;
        m_upVector = m_orientation * m_worldY;
        m_rightVector = m_orientation * m_worldX;

        m_transformMatrix = glm::lookAt(m_translation, m_translation + m_direction, m_upVector);

        m_isDirty = false;
    }
}


} // namespace video
} // namespace unicorn
