/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera.hpp>
#include "unicorn/utility/Logger.hpp"

namespace unicorn
{
namespace video
{
Camera::Camera(glm::vec3 postion, glm::vec3 direction)
    : m_aspect(0), m_camPosition(postion), m_upVector(glm::vec3(0.0f, -1.0f, 0.0f)), m_camDirection(direction), m_fov(45.0f), m_znear(0.1f), m_zfar(1000.0f), m_projectionType(ProjectionType::Perspective)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float znear, float zfar)
{
    m_fov = fov;
    m_znear = znear;
    m_zfar = zfar;
    m_aspect = aspect;
    UpdateProjectionMatrix();
}

void Camera::UpdateAspectRatio(float aspect)
{
    m_aspect = aspect;
    UpdateProjectionMatrix();
}

void Camera::Translate(const glm::vec3& delta)
{
    m_camPosition += delta;
    UpdateViewMatrix();
}

void Camera::SetDirection(const glm::vec3& direction)
{
    m_camDirection = direction;
    UpdateViewMatrix();
}

void Camera::SetUpVector(const glm::vec3& upVector)
{
    m_upVector = upVector;
    UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_camPosition = position;
    UpdateViewMatrix();
}

void Camera::SetProjection(ProjectionType newType)
{
    m_projectionType = newType;
    UpdateProjectionMatrix();
}

glm::vec3 Camera::GetDirection() const
{
    return m_camDirection;
}

glm::vec3 Camera::GetUpVector() const
{
    return m_upVector;
}

glm::mat4 Camera::GetProjection() const
{
    return m_matrices.m_perspective;
}

glm::mat4 Camera::GetView() const
{
    return m_matrices.m_view;
}

void Camera::UpdateViewMatrix()
{
    m_matrices.m_view = glm::lookAt(
        m_camPosition,
        m_camPosition + m_camDirection,
        m_upVector);
}

void Camera::UpdateProjectionMatrix()
{
    switch ( m_projectionType )
    {
        case ProjectionType::Perspective:
            m_matrices.m_perspective = glm::perspective(
                m_fov,
                m_aspect,
                m_znear,
                m_zfar);
            break;
        case ProjectionType::Orthographic:
            m_matrices.m_perspective = glm::ortho(0.0f, 2.0f, 0.0f, 2.0f, 0.0f, 1000.0f);
            break;
    }

}
}
}
