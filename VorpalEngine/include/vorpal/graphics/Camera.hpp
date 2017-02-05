/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_CAMERA_HPP
#define VORPAL_GRAPHICS_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vp
{
namespace graphics
{
class Camera
{
public:
    Camera();
    ~Camera();

    void SetPerspective(float fov, float aspect, float znear, float zfar)
    {
        m_fov = fov;
        m_znear = znear;
        m_zfar = zfar;
        m_perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
    }

    void UpdateAspectRatio(float aspect)
    {
        m_perspective = glm::perspective(glm::radians(m_fov), aspect, m_znear, m_zfar);
    }

    void SetPosition(glm::vec3 position)
    {
        m_position = position;
        UpdateViewMatrix();
    }
    void SetRotation(glm::vec3 rotation)
    {
        m_rotation = rotation;
        UpdateViewMatrix();
    }

    void Rotate(glm::vec3 delta)
    {
        m_rotation += delta;
        UpdateViewMatrix();
    }

    void SetTranslation(glm::vec3 translation)
    {
        m_position = translation;
        UpdateViewMatrix();
    }

    void Translate(glm::vec3 delta)
    {
        m_position += delta;
        UpdateViewMatrix();
    }

private:
    void UpdateViewMatrix()
    {
        glm::mat4 rotM = glm::mat4();
        glm::mat4 transM;

        rotM = glm::rotate(rotM, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotM = glm::rotate(rotM, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotM = glm::rotate(rotM, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        transM = glm::translate(glm::mat4(), m_position);
        m_view = rotM * transM;
    }

    float m_fov;
    float m_znear;
    float m_zfar;
    glm::vec3 m_rotation;
    glm::vec3 m_position;
    glm::mat4 m_perspective;
    glm::mat4 m_view;
    float m_rotationSpeed;
    float m_movementSpeed;
};
}
}

#endif // VORPAL_GRAPHICS_CAMERA_HPP
