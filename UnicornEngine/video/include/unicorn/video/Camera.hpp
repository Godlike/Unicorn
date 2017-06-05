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
 * @brief Type of camera projection
 */
enum class ProjectionType
{
    Perspective
};

/**
 * @brief Camera class, hold view and projection matrices.
 */
class Camera
{
public:
    /**
     * @brief Camera constuctor
     * @param position Default position of camera
     * @param direction Default view direction of camera
     */
    UNICORN_EXPORT Camera(const glm::vec3& position, const glm::vec3& direction);

    /**
     * @brief Sets perspective
     * @param fov Field of view
     * @param aspect Aspect ratio
     * @param znear Camera's near frustrum plane
     * @param zfar Ñamera's far frustrum plane
     */
    UNICORN_EXPORT void SetPerspective(float fov, float aspect, float znear, float zfar);
    /**
     * @brief Sets aspect ratio
     * @param aspect Aspect value
     */
    UNICORN_EXPORT void SetAspectRatio(float aspect);
    /**
     * @brief Translate camera in space
     * @param delta vector of translation
     */
    UNICORN_EXPORT void Translate(const glm::vec3& delta);
    /**
     * @brief Sets direction of camera
     * @param direction New direction value
     */
    UNICORN_EXPORT void SetDirection(const glm::vec3& direction);
    /**
     * @brief Sets up vector for camera
     * @param upVector New up vector value
     */
    UNICORN_EXPORT void SetUpVector(const glm::vec3& upVector);
    /**
     * @brief Sets position for camera
     * @param position New position value
     */
    UNICORN_EXPORT void SetPosition(const glm::vec3& position);
    /**
     * @brief Sets projection type for camera
     * @param type New type value
     */
    UNICORN_EXPORT void SetProjectionType(ProjectionType type);
    /**
     * @brief Sets new field of view value
     * @param newFov New field of view
     */
    UNICORN_EXPORT void SetFov(float newFov);
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
    ProjectionType m_projectionType;
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
