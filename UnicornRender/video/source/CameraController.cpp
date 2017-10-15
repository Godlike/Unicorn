/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/CameraController.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{

glm::quat RotationBetweenVectors(glm::vec3 origin, glm::vec3 dest) 
{    
    origin = normalize(origin);
    dest = normalize(dest);
    
    float cosTheta = dot(origin, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        // opposite directions, so guess one
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), origin);
        if (length2(rotationAxis) < 0.01) // they parallel
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), origin);

        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = cross(origin, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return glm::quat(s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

void CameraController::SetDirection(glm::vec3 const& direction)
{
    auto q = RotationBetweenVectors(m_direction, direction);
    Rotate(q);
    m_isDirty = true;
}

void CameraController::SetUpVector(glm::vec3 const& upVector)
{
    auto q = RotationBetweenVectors(m_upVector, upVector);
    Rotate(q);
    m_isDirty = true;
}

void CameraController::SetPosition(glm::vec3 const& position)
{
    m_position = position;
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

void CameraController::LookAt(glm::vec3 dir, glm::vec3 up)
{
    if (length2(dir) < 0.0001f)
        return;

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    glm::vec3 right = cross(dir, up);
    up = cross(right, dir);

    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    glm::quat rot1 = RotationBetweenVectors(m_direction, dir);
    // Because of the 1rst rotation, the up is probably completely screwed up. 
    // Find the rotation between the "up" of the rotated object, and the desired up
    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rot2 = RotationBetweenVectors(newUp, up);
    Rotate(rot2 * rot1);
}

glm::mat4 CameraController::GetViewMatrix() const {
    glm::mat4 viewMatrix = glm::mat4_cast(m_orientation);
    viewMatrix = glm::translate(viewMatrix, m_position);
    return viewMatrix;
}

CameraController::CameraController(glm::mat4& cameraView)
    : m_cameraView(cameraView)
    , m_position(0.0)
    , m_upVector(0.0f, 1.0f, 0.0f)
    , m_direction(0.0f, 0.0f, 1.0f)
    , m_rightVector(1.0, 0.0f, 0.0f)
    , m_isDirty(true)
    , m_orientation(glm::vec3(0))
{
}

} // namespace video
} // namespace unicorn
