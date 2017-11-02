/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtx/quaternion.hpp>

namespace unicorn
{
namespace video
{

using namespace glm;

// Returns a quaternion such that q*start = dest
quat RotationBetweenVectors0(vec3 start, vec3 dest){
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = dot(start, dest);
    vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f){
        // special case when vectors in opposite directions :
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        // This implementation favors a rotation around the Up axis,
        // since it's often what you want to do.
        rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if (length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(glm::radians(180.0f), rotationAxis);
    }

    // Implementation from Stan Melax's Game Programming Gems 1 article
    rotationAxis = cross(start, dest);

    float s = sqrt( (1 + cosTheta)*2 );
    float invs = 1 / s;

    return quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

quat LookAt(vec3 direction, vec3 desiredUp){

    if (length2(direction) < 0.0001f )
        return quat();

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);

    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    quat rot1 = RotationBetweenVectors0(vec3(0.0f, 0.0f, 1.0f), direction);
    // Because of the 1rst rotation, the up is probably completely screwed up.
    // Find the rotation between the "up" of the rotated object, and the desired up
    vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
    quat rot2 = RotationBetweenVectors0(newUp, desiredUp);

    // Apply them
    return rot2 * rot1; // remember, in reverse order.
}

glm::quat RotationBetweenVectors(glm::vec3 v1, glm::vec3 v2)
{
    glm::quat q;

    v1 = normalize(v1);
    v2 = normalize(v2);

    const auto cosTheta = dot(v1, v2);

    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f)
    {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), v1);

        if (glm::length2(rotationAxis) < 0.01)
        {
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), v1);
        }

        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = cross(v2, v1);

    q = glm::quat(rotationAxis);
    q.w = glm::sqrt(pow(length(v1), 2) * (length(v2), 2) + cosTheta);

    return normalize(q);
}

glm::quat RotationBetweenVectors2(glm::vec3 v0, glm::vec3 v1)
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

glm::quat RotationBetweenVectors3(glm::vec3 v0, glm::vec3 v1)
{
    v0 = normalize(v0);
    v1 = normalize(v1);

    return rotation(v0, v1);
}


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

void Transform::LookAtDirection(glm::vec3 direction, glm::vec3 upVector)
{
    m_orientation = RotationBetweenVectors0(m_direction, direction);

    m_isDirty = true;
}

void Transform::LookAtPoint(glm::vec3 point, glm::vec3 upVector) // Not Working
{
    glm::mat4 mat = lookAt(m_translation, point, m_upVector);
    //Rotate(quat_cast(mat));
    m_orientation = conjugate(quat_cast(mat));

    m_isDirty = true;
}

void Transform::SetUp(glm::vec3 upVector) // Not Working
{
    glm::mat4 mat = lookAt(m_translation, m_translation + m_direction, upVector);
    m_orientation = normalize(quat_cast(mat));

    m_isDirty = true;
}

void Transform::SetRight(glm::vec3 rightVector) // Not Working
{
    m_rightVector = rightVector;
    m_upVector = cross(m_direction, m_rightVector);

    m_isDirty = true;
}

void Transform::SetTranslate(glm::vec3 translate) // Works
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

void Transform::Translate(glm::vec3 translate)  // Works
{
    m_translation += translate;

    m_isDirty = true;
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

void Transform::TranslateLocalX(float distance)
{
    Translate(m_rightVector * distance);
}

void Transform::TranslateLocalY(float distance)
{
    Translate(m_upVector * distance);
}

void Transform::TranslateLocalZ(float distance)
{
    Translate(m_direction * distance);
}

void Transform::TranslateWorldX(float distance)
{
    Translate(m_worldX * distance);
}

void Transform::TranslateWorldY(float distance)
{
    Translate(m_worldY * distance);
}

void Transform::TranslateWorldZ(float distance)
{
    Translate(m_worldZ * distance);
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
    Translate(dir);
    glm::quat q = angleAxis(radians, axis);
    Translate(q * -dir);
    m_isDirty = true;
}

void Transform::Rotate(float angleRadians, glm::vec3 axis) {
    glm::quat q = glm::angleAxis(angleRadians, axis);
    Rotate(q);
}

void Transform::CalculateOrientation()
{
    glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);
    glm::quat y = glm::angleAxis(m_rotation.y, m_worldY);
    glm::quat z = glm::angleAxis(m_rotation.z, m_worldZ);

    m_orientation = z * x * y * m_orientation;

    m_rotation = glm::vec3(0);
}

void Transform::Update()
{
    if (m_isDirty)
    {
        CalculateOrientation();

        m_direction = conjugate(m_orientation) * m_worldZ;
        m_upVector = conjugate(m_orientation) * m_worldY;
        m_rightVector = cross(m_upVector, m_direction);

        m_transformMatrix = glm::lookAt(m_translation, m_translation + m_direction, m_upVector);

        m_isDirty = false;
    }
}


} // namespace video
} // namespace unicorn
