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
    UNICORN_EXPORT void Recalculate();

    UNICORN_EXPORT void Pitch(float pitchRadians) {
        Rotate(pitchRadians, m_rightVector);
    }

    UNICORN_EXPORT void LookAt(glm::vec3 dir, glm::vec3 up);

    UNICORN_EXPORT void Rotate(float angleRadians, const glm::vec3& axis) {
        glm::quat q = glm::angleAxis(angleRadians, axis);
        Rotate(q);
    }

    UNICORN_EXPORT void Rotate(const glm::quat& rotation) {
        m_orientation = rotation * m_orientation;
    }

    UNICORN_EXPORT glm::vec3 GetDirection() const {
        return glm::conjugate(m_orientation) * m_direction;
    }

    UNICORN_EXPORT glm::vec3 GetRight() const {
        return glm::conjugate(m_orientation) * m_rightVector;
    }

    UNICORN_EXPORT glm::vec3 GetUp() const {
        return glm::conjugate(m_orientation) * m_upVector;
    }

    UNICORN_EXPORT void MoveForward(float movement) {
        m_position += GetDirection() * movement;
    }

    UNICORN_EXPORT void MoveLeft(float movement) {
        m_position += GetRight() * movement;
    }

    UNICORN_EXPORT void MoveUp(float movement) {
        m_position += GetUp() * movement;
    }

    glm::mat4 GetViewMatrix() const;

protected:
    CameraController(glm::mat4& cameraView);

    /** @brief Recalculates view matrix */
    virtual void UpdateViewMatrix() = 0;

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
