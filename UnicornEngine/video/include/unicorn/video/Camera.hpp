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
/**
 * @brief Type of projection, which we can use in Camera
 */
enum class ProjectionType
{
    Perspective
};

/**
 * @brief Camera class, which create View and Projection matrices.
 */
class Camera
{
public:
    /**
     * @brief Camera constuctor
     * @param postion Default position of camera
     * @param direction Default view direction of camera
     */
    UNICORN_EXPORT Camera(glm::vec3 postion, glm::vec3 direction);

    /**
     * @brief 
     * @param fov Field of view
     * @param aspect Aspect ratio - 4/3, 16/9 and so on.
     * @param znear Near camera's frustrum plane
     * @param zfar Far camera's frustrum plane
     */
    UNICORN_EXPORT void SetPerspective(float fov, float aspect, float znear, float zfar);
    /**
     * @brief Setting new aspect ratio
     * @param aspect New aspect value
     */
    UNICORN_EXPORT void UpdateAspectRatio(float aspect);
    /**
     * @brief Translate camera in space
     * @param delta vector of translation, for example {15.f, 3.f, 0.f} will translate x by 15m, y by 3 and z by 0
     */
    UNICORN_EXPORT void Translate(const glm::vec3& delta);
    /**
     * @brief Setting new direction of camera
     * @param direction New direction value
     */
    UNICORN_EXPORT void SetDirection(const glm::vec3& direction);
    /**
     * @brief Setting new up vector for camera
     * @param upVector New up vector value
     */
    UNICORN_EXPORT void SetUpVector(const glm::vec3& upVector);
    /**
     * @brief Setting new position for camera
     * @param position New position value
     */
    UNICORN_EXPORT void SetPosition(const glm::vec3& position);
    /**
     * @brief Setting new projection type for camera
     * @param newType New type value
     */
    UNICORN_EXPORT void SetProjectionType(ProjectionType newType);
    /**
     * @brief Set new field of view value
     * @param newFov New field of view
     */
    UNICORN_EXPORT void SetFov(float newFov);
    /**
     * @brief Getter for direction
     * @return Direction vector
     */
    UNICORN_EXPORT glm::vec3 GetDirection() const;
    /**
     * @brief Getter for up vector
     * @return Up vector value
     */
    UNICORN_EXPORT glm::vec3 GetUpVector() const;
    /**
     * @brief Getter for projection matrix
     * @return Projection matrix value
     */
    UNICORN_EXPORT glm::mat4 GetProjection() const;
    /**
     * @brief Getter for view matrix
     * @return View matrix value
     */
    UNICORN_EXPORT glm::mat4 GetView() const;
    /**
     * @brief Getter for field of view value
     * @return Field of view value
     */
    UNICORN_EXPORT float GetFov() const;
    /**
     * @brief Update field of view with new delta value. Useful for zooming effect.
     * @param deltaFov Adding this value to old one.
     */
    UNICORN_EXPORT void ChangeFov(float deltaFov);

private:
    float m_aspect;
    glm::vec3 m_camPosition;
    glm::vec3 m_upVector;
    glm::vec3 m_camDirection;

    struct
    {
        glm::mat4 m_projection;
        glm::mat4 m_view;
    } m_matrices;

    float m_fov;
    float m_znear, m_zfar;
    ProjectionType m_projectionType;
    /**
     * @brief Recalculating view matrix
     */
    void UpdateViewMatrix();
    /**
     * @brief Recalculating projection matrix
     */
    void UpdateProjectionMatrix();
};
}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
