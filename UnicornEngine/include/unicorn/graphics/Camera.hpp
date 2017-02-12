/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_CAMERA_HPP
#define UNICORN_GRAPHICS_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unicorn/utility/SharedMacros.hpp>

namespace uc
{
namespace graphics
{
/**
 * @brief The Camera class to look around.
 */
class UNICORN_EXPORT Camera
{
public:
    Camera();
    ~Camera();

    void SetPerspective(float fov, float aspect, float znear, float zfar);

    void UpdateAspectRatio(float aspect);
    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void Rotate(glm::vec3 delta);
    void SetTranslation(glm::vec3 translation);
    void Translate(glm::vec3 delta);

private:
    void UpdateViewMatrix();

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

#endif // UNICORN_GRAPHICS_CAMERA_HPP
