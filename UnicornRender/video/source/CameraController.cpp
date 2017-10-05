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

glm::quat RotationBetweenVectors(glm::vec3 v1, glm::vec3 v2)
{
    glm::quat q;
    const auto cosTheta = dot(v1, v2);
    //const auto a = cross(v2, v1);

    glm::vec3 rotationAxis;
    
    if (cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), v1);
            
        if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again! 
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), v1);
    
        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }
    
    rotationAxis = cross(v2, v1);

    q = rotationAxis;
    q.w = glm::sqrt(pow(length(v1), 2) * (length(v2), 2) + cosTheta);
    return normalize(q);
}

//glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
//    start = normalize(start);
//    dest = normalize(dest);
//
//    float cosTheta = dot(start, dest);
//    glm::vec3 rotationAxis;
//
//    if (cosTheta < -1 + 0.001f) {
//        // special case when vectors in opposite directions:
//        // there is no "ideal" rotation axis
//        // So guess one; any will do as long as it's perpendicular to start
//        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
//        
//        if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again! 
//            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
//
//        rotationAxis = normalize(rotationAxis);
//        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
//    }
//
//    rotationAxis = cross(start, dest);
//
//    float s = sqrt((1 + cosTheta) * 2);
//    float invs = 1 / s;
//
//    return glm::quat(
//        s * 0.5f,
//        rotationAxis.x * invs,
//        rotationAxis.y * invs,
//        rotationAxis.z * invs
//    );
//
//}

void CameraController::SetDirection(glm::vec3 direction)
{
    m_orientation = RotationBetweenVectors(m_direction, direction) * m_orientation;
    m_isDirty = true;
}

void CameraController::SetUpVector(glm::vec3 upVector)
{
    m_orientation = RotationBetweenVectors(m_upVector, upVector) * m_orientation;
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
    m_direction = glm::normalize(glm::conjugate(m_orientation) * m_worldZ);
    m_upVector = glm::normalize(glm::conjugate(m_orientation) * m_worldY);
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
