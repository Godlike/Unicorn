/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace unicorn
{
namespace video
{
Camera::Camera() :
    m_aspect(0)
    , m_position({0.0f, 0.0f, 0.0f})
    , m_upVector(0.0f, -1.0f, 0.0f)
    , m_direction({ 0.0f, 0.0f, 0.0f })
    , m_dirtyView(false)
{
    UpdateViewMatrix();
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

const glm::mat4& Camera::GetView() const
{
    return view;
}


void Camera::Frame()
{
    if (m_dirtyView)
    {
        UpdateViewMatrix();
        m_dirtyView = false;
    }
}

//void Camera::SetFov(float fov)
//{
//    m_fov = std::max(std::min(fov, m_fovUpperBound), m_fovLowerBound);
//    m_dirtyProjection = true;
//}

void Camera::UpdateViewMatrix()
{
    view = glm::lookAt(m_position,
                                    m_position + m_direction,
                                    m_upVector);
}
}
}
