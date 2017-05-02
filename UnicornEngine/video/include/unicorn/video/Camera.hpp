/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_HPP
#define UNICORN_VIDEO_CAMERA_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unicorn/utility/SharedMacros.hpp>

namespace unicorn
{
namespace video
{
enum class ProjectionType
{
    Perspective,
    Orthographic
};

class Camera
{
public:
    UNICORN_EXPORT Camera(glm::vec3 postion, glm::vec3 direction);

    UNICORN_EXPORT void SetPerspective(float fov, float aspect, float znear, float zfar);

    UNICORN_EXPORT void UpdateAspectRatio(float aspect);

    UNICORN_EXPORT void Translate(glm::vec3 delta);
    UNICORN_EXPORT void UpdateMouseView(double posX, double posY);

    UNICORN_EXPORT glm::mat4 GetProjection() const;
    UNICORN_EXPORT glm::mat4 GetView() const;

private:
    float m_camSpeed;
    float m_camYaw;
    float m_aspect;
    double m_lastX, m_lastY, m_yaw, m_pitch;
    double m_sensitivity;
    glm::vec3 m_camPosition;
    glm::vec3 m_upVector;
    glm::vec3 m_camDirection;
    bool m_firstMouse;
    struct
    {
        glm::mat4 m_perspective;
        glm::mat4 m_view;
    } m_matrices;

    float m_fov;
    float m_znear, m_zfar;

    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
};
}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
