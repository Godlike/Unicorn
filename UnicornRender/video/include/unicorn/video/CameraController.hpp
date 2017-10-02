/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERACONTROLLER_HPP
#define UNICORN_VIDEO_CAMERACONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{

/** @brief Camera controller class to create new controllers for cameras */
class CameraController
{
public:
    /** @brief Sets direction of the camera */
    UNICORN_EXPORT void SetDirection(glm::vec3 const& direction);

    /** @brief Sets up vector of the camera */
    UNICORN_EXPORT void SetUpVector(glm::vec3 const& upVector);

    /** @brief Sets position of the camera */
    UNICORN_EXPORT void SetPosition(glm::vec3 const& position);

    /** @brief Returns direction of the camera */
    UNICORN_EXPORT glm::vec3 const& GetDirection() const;

    /** @brief Returns up vector of the camera */
    UNICORN_EXPORT glm::vec3 const& GetUpVector() const;

    /** @brief Returns position of the camera */
    UNICORN_EXPORT glm::vec3 const& GetPosition() const;

    /** @brief Recalculates view matrix if needed */
    UNICORN_EXPORT void Recalculate();
protected:
    CameraController(glm::mat4& cameraView);

    /** @brief Recalculates view matrix */
    void UpdateViewMatrix();

    glm::mat4& m_cameraView;
    glm::vec3 m_position;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;
    glm::vec3 m_rightVector;

    bool m_isDirty;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERACONTROLLER_HPP
