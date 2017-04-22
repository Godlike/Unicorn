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
        Camera::Camera() : m_rotation({ 0.0, 0.0, 0.0 }),
            m_position({ 0.0, 0.0, 0.0 }),
            m_rotationSpeed(0.0),
            m_movementSpeed(0.0),
            m_fov(0.0),
            m_znear(0.0),
            m_zfar(0.0)
        {
        }

        void Camera::SetPerspective(float fov, float aspect, float znear, float zfar)
        {
            m_fov = fov;
            m_znear = znear;
            m_zfar = zfar;
            m_matrices.m_perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
        }

        void Camera::UpdateAspectRatio(float aspect)
        {
            m_matrices.m_perspective = glm::perspective(glm::radians(m_fov), aspect, m_znear, m_zfar);
        }

        void Camera::SetPosition(glm::vec3 position)
        {
            m_rotation = position;
            UpdateViewMatrix();
        }

        void Camera::SetRotation(glm::vec3 rotation)
        {
            m_rotation = rotation;
            UpdateViewMatrix();
        }

        void Camera::Rotate(glm::vec3 delta)
        {
            m_rotation += delta;
            UpdateViewMatrix();
        }

        void Camera::SetTranslation(glm::vec3 translation)
        {
            m_position = translation;
            UpdateViewMatrix();
        }

        void Camera::Translate(glm::vec3 delta)
        {
            m_position += delta;
            UpdateViewMatrix();
        }

        void Camera::UpdateViewMatrix()
        {
            glm::mat4 rotM = glm::mat4();
            rotM = glm::rotate(rotM, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotM = glm::rotate(rotM, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotM = glm::rotate(rotM, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_matrices.m_view = rotM * glm::translate(glm::mat4(), m_position);
        }
    }
}
