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
        Camera::Camera(glm::vec3 postion, glm::vec3 direction) : m_camSpeed(0),
                           m_camPosition(postion),
                           m_upVector(glm::vec3(0.0f, 1.0f, 0.0f)),
                           m_camDirection(direction),
                           m_fov(45.0),
                           m_znear(0.1),
                           m_zfar(100.0)
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

        
        void Camera::Translate(glm::vec3 delta)
        {
            m_camPosition += delta;
            UpdateViewMatrix();
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
                m_upVector
            );
        }

        void Camera::UpdateProjectionMatrix()
        {
            m_matrices.m_perspective = glm::perspective(
                m_fov,
                m_aspect,
                m_znear,
                m_zfar
            );
        }
    }
}
