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
        Camera::Camera(glm::vec3 postion, glm::vec3 direction) : m_camSpeed(0),
                           m_camPosition(postion),
                           m_upVector(glm::vec3(0.0f, 1.0f, 0.0f)),
                           m_camDirection(direction),
                           m_fov(45.0),
                           m_znear(0.1),
                           m_zfar(100.0),
                           m_sensitivity(1.5f),
                           m_lastX(0.0),
                           m_lastY(0.0),
                        m_yaw(0),
                        m_pitch(0)

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

        void Camera::UpdateMouseView(double posX, double posY)
        {
            LOG_ERROR("posX %f posY %f", posX, posY);
            double xoffset = posX - m_lastX;
            double yoffset = m_lastY - posY;
            m_lastX = posX;
            m_lastY = posY;

            xoffset *= m_sensitivity;
            yoffset *= m_sensitivity;

            m_yaw += xoffset;
            m_pitch += yoffset;

            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;

            glm::vec3 front;
            front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
            front.y = static_cast<float>(sin(glm::radians(m_pitch)));
            front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
            LOG_ERROR("x %f y %f z%f ", front.x, front.y, front.z);
            m_camDirection = glm::normalize(front);
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
