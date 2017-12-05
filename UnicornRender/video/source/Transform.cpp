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
    , m_scale(glm::vec3(1.f))
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
    m_orientation = utility::math::CalculateOrientationQuaternion(direction, upVector);

    m_isDirty = true;
}

void Transform::SetUp(glm::vec3 upVector)
{
    LookAtDirection(m_direction, upVector);
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

glm::vec3 Transform::GetTranslation() const
{
    return m_translation;
}

glm::mat4 const& Transform::GetModelMatrix() const
{
    return m_transformMatrix;
}

void Transform::Scale(glm::vec3 scale)
{
    m_scale = scale;
}

void Transform::TranslateLocal(glm::vec3 distance)
{
    glm::vec3 const rightTranslation = m_rightVector * distance.x;
    glm::vec3 const upTranslation = m_upVector * distance.y;
    glm::vec3 const forwardTranslation = m_direction * distance.z;
    SetTranslation(GetTranslation() + rightTranslation + upTranslation + forwardTranslation);
}

void Transform::TranslateWorld(glm::vec3 distance)
{
    glm::vec3 const xTranslation = m_worldX * distance.x;
    glm::vec3 const yTranslation = m_worldY * distance.y;
    glm::vec3 const zTranslation = m_worldZ * distance.z;
    SetTranslation(GetTranslation() + xTranslation + yTranslation + zTranslation);
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

void Transform::Rotate(float angleRadians, glm::vec3 axis) {
    glm::quat const q = glm::angleAxis(angleRadians, axis);
    Rotate(q);
}

void Transform::SetOrientation(glm::quat quat)
{
    m_orientation = quat;
    m_isDirty = true;
}

void Transform::SetRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
    m_isDirty = true;
}

void Transform::UpdateOrientation()
{
    glm::quat const z = glm::angleAxis(m_rotation.z, m_worldZ);
    glm::quat const y = glm::angleAxis(m_rotation.y, m_worldY);
    glm::quat const x = glm::angleAxis(m_rotation.x, m_worldX);

    m_orientation = normalize(m_orientation * z * x * y);

    m_rotation = glm::vec3(0);
}

void Transform::UpdateModelMatrix()
{
    if (m_isDirty)
    {
        UpdateOrientation();

        m_direction = m_orientation * m_worldZ;
        m_upVector = m_orientation * m_worldY;
        m_rightVector = m_orientation * m_worldX;

        auto T = glm::translate(glm::mat4(1.0), m_translation);
        auto R = glm::mat4_cast(m_orientation) * glm::mat4(1.0);
        auto S = glm::scale(glm::mat4(1.0), { m_scale });

        m_transformMatrix = T * R * S;

        m_isDirty = false;
    }
}


} // namespace video
} // namespace unicorn
