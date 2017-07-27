/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_HPP
#define UNICORN_VIDEO_CAMERA_HPP

#include <unicorn/utility/SharedMacros.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
/**
 * @brief Type of camera projection
 */
enum class ProjectionType
{
    Perspective
};

/**
 * @brief Camera class, holds view and projection matrices.
 */
class Camera
{
public:
    /**
     * @brief Camera constuctor
     * @param position Default position
     * @param direction Default view direction
     */
    UNICORN_EXPORT Camera(const glm::vec3& position, const glm::vec3& direction);

    /**
     * @brief Sets perspective projection mode from given frustum description
     * @param fov Field of view
     * @param aspect Aspect ratio
     * @param znear Near frustrum plane
     * @param zfar Far frustrum plane
     */
    UNICORN_EXPORT void SetPerspective(float fov, float aspect, float znear, float zfar);
    /**
     * @brief Sets aspect ratio
     * @param aspect Aspect value
     */
    UNICORN_EXPORT void SetAspectRatio(float aspect);
    /**
     * @brief Changes camera position by given vector
     * @param delta Vector of translation
     */
    UNICORN_EXPORT void Translate(const glm::vec3& delta);
    /**
     * @brief Sets direction
     * @param direction New direction value
     */
    UNICORN_EXPORT void SetDirection(const glm::vec3& direction);
    /**
     * @brief Sets up vector
     * @param upVector New up vector value
     */
    UNICORN_EXPORT void SetUpVector(const glm::vec3& upVector);
    /**
     * @brief Sets position
     * @param position New position value
     */
    UNICORN_EXPORT void SetPosition(const glm::vec3& position);
    /**
     * @brief Sets field of view value
     * @param fov New horizontal field of view
     */
    UNICORN_EXPORT void SetFov(float fov);
    /**
     * @brief Returns direction
     * @return Direction vector
     */
    UNICORN_EXPORT const glm::vec3& GetDirection() const;
    /**
     * @brief Returns up vector
     * @return Up vector value
     */
    UNICORN_EXPORT const glm::vec3& GetUpVector() const;
    /**
     * @brief Returns projection matrix
     * @return Projection matrix value
     */
    UNICORN_EXPORT const glm::mat4& GetProjection() const;
    /**
     * @brief Returns view matrix
     * @return View matrix value
     */
    UNICORN_EXPORT const glm::mat4& GetView() const;
    /**
     * @brief Returns field of view value
     * @return Field of view value
     */
    UNICORN_EXPORT float GetFov() const;
    /**
    * @brief Checks if view or projection need to be updated and update it, if needed
    */
    UNICORN_EXPORT void Frame();
private:
    float m_aspect;
    glm::vec3 m_position;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;

    struct
    {
        glm::mat4 m_projection;
        glm::mat4 m_view;
    } m_matrices;

    float m_fov;
    float m_znear, m_zfar;
    bool m_dirtyView;
    float m_fovLowerBound;
    float m_fovUpperBound;
    bool m_dirtyProjection;
    /**
     * @brief Calculates view matrix
     */
    void UpdateViewMatrix();
    /**
     * @brief Calculates projection matrix
     */
    void UpdateProjectionMatrix();
};
}
}

#endif // UNICORN_VIDEO_CAMERA_HPP
