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
#include <glm/gtc/quaternion.hpp>

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

    /** @brief Recalculates view matrix if needed */
    UNICORN_EXPORT void Update();

    UNICORN_EXPORT void Pitch(float rad);

    UNICORN_EXPORT void Yaw(float rad);

    UNICORN_EXPORT void Roll(float rad);

    UNICORN_EXPORT void LookAt(glm::vec3 dir, glm::vec3 up);

    UNICORN_EXPORT void Rotate(float rad, glm::vec3 axis);

    UNICORN_EXPORT void Rotate(glm::quat rotation);

    UNICORN_EXPORT glm::vec3 GetDirection() const;

    UNICORN_EXPORT glm::vec3 GetRight() const;

    UNICORN_EXPORT glm::vec3 GetUp() const;

    UNICORN_EXPORT void MoveForward(float movement);

    UNICORN_EXPORT void MoveLeft(float movement);

    UNICORN_EXPORT void MoveUp(float movement);

    UNICORN_EXPORT glm::mat4 GetViewMatrix() const;

protected:
    CameraController(glm::mat4& cameraView);

    float m_yaw;
    float m_pitch;
    float m_roll;

    /** @brief Recalculates view matrix */
    void UpdateViewMatrix();

    glm::mat4& m_cameraView;
    glm::vec3 m_position;
    const glm::vec3 m_upVector;
    const glm::vec3 m_direction;
    const glm::vec3 m_rightVector;
    glm::quat m_orientation;
    bool m_isDirty;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERACONTROLLER_HPP
