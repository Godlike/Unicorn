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
    UNICORN_EXPORT void SetOrientation(glm::vec3 direction, glm::vec3 upvector);


    /** @brief Sets position of the camera */
    UNICORN_EXPORT void SetPosition(glm::vec3 position);

    UNICORN_EXPORT glm::vec3 GetDirection() const;

    UNICORN_EXPORT glm::vec3 GetRight() const;

    UNICORN_EXPORT glm::vec3 GetUp() const;

    UNICORN_EXPORT glm::vec3 GetPosition() const;

    UNICORN_EXPORT void Translate(glm::vec3 translate);

    UNICORN_EXPORT void TranslateLocalX(float distance);

    UNICORN_EXPORT void TranslateLocalY(float distance);

    UNICORN_EXPORT void TranslateLocalZ(float distance);

    UNICORN_EXPORT void TranslateWorldX(float distance);

    UNICORN_EXPORT void TranslateWorldY(float distance);

    UNICORN_EXPORT void TranslateWorldZ(float distance);

    UNICORN_EXPORT void RotateX(float radians);

    UNICORN_EXPORT void RotateY(float radians);

    UNICORN_EXPORT void RotateZ(float radians);

    UNICORN_EXPORT void Rotate(glm::vec3 rotation);

    UNICORN_EXPORT void RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point);

    /** @brief Recalculates view matrix if needed */
    UNICORN_EXPORT void Update();

    /** @brief Recalculates view matrix even if not needed */
    UNICORN_EXPORT void ForceUpdate();
protected:
    CameraController(glm::mat4& cameraView);

    /** @brief Recalculates view matrix */
    void UpdateViewMatrix();
    virtual void CalculateOrientation();

    glm::mat4& m_cameraView;
    glm::vec3 m_rotation;
    glm::vec3 m_position;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;
    const glm::vec3 m_worldX;
    const glm::vec3 m_worldY;
    const glm::vec3 m_worldZ;
    glm::vec3 m_rightVector;
    glm::quat m_orientation;
    bool m_isDirty;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERACONTROLLER_HPP
