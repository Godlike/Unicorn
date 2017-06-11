/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera.hpp>

namespace unicorn
{
namespace video
{
Camera::Camera(const glm::vec3& position, const glm::vec3& direction) : m_aspect(0)
                                                                      , m_position(position)
                                                                      , m_upVector(0.0f, -1.0f, 0.0f)
                                                                      , m_direction(direction)
                                                                      , m_fov(45.0f)
                                                                      , m_znear(0.1f)
                                                                      , m_zfar(1000.0f)
                                                                      , m_dirtyView(false)
                                                                      , m_dirtyProjection(false)
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
    m_dirtyProjection = true;
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspect = aspect;
    m_dirtyProjection = true;
}

void Camera::Translate(const glm::vec3& delta)
{
    m_position += delta;
    m_dirtyView = true;
}

void Camera::SetDirection(const glm::vec3& direction)
{
    m_direction = direction;
    m_dirtyView = true;
}

void Camera::SetUpVector(const glm::vec3& upVector)
{
    m_upVector = upVector;
    m_dirtyView = true;
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    m_dirtyView = true;
}

const glm::vec3& Camera::GetDirection() const
{
    return m_direction;
}

const glm::vec3& Camera::GetUpVector() const
{
    return m_upVector;
}

const glm::mat4& Camera::GetProjection() const
{
    return m_matrices.m_projection;
}

const glm::mat4& Camera::GetView() const
{
    return m_matrices.m_view;
}

float Camera::GetFov() const
{
    return m_fov;
}

void Camera::Frame()
{
    if (m_dirtyView)
    {
        UpdateViewMatrix();
        m_dirtyView = false;
    }
    if (m_dirtyProjection)
    {
        UpdateProjectionMatrix();
        m_dirtyProjection = false;
    }
}

void Camera::SetFov(float fov)
{
    m_fov = fov;
    m_dirtyProjection = true;
}

void Camera::UpdateViewMatrix()
{
    m_matrices.m_view = glm::lookAt(m_position,
                                    m_position + m_direction,
                                    m_upVector);
}

void Camera::UpdateProjectionMatrix()
{
    m_matrices.m_projection = glm::perspective(m_fov,
                                               m_aspect,
                                               m_znear,
                                               m_zfar);
}
}
}
