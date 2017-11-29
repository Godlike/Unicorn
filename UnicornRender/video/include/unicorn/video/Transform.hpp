/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TRANSFORM_HPP
#define UNICORN_VIDEO_TRANSFORM_HPP

#include <unicorn/utility/SharedMacros.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{
/**
 * @brief Base class for every object that can be translated or rotated
 */
class Transform
{
public:
    /** @brief Default constructor */
    Transform();

    /** @brief Default destructor */
    virtual ~Transform() = default;

    /**
     * @brief Sets look at provided direction
     *
     * @param[in] direction new direction vectorr
     */
    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction);

    /**
     * @brief Sets look at provided direction
     *
     * @param[in] direction new direction vector
     * @param[in] upVector new up vector
     */
    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction, glm::vec3 upVector);

    /**
     * @brief Sets up vector
     *
     * @param[in] upVector new up vector
     */
    UNICORN_EXPORT void SetUp(glm::vec3 upVector);

    /**
     * @brief      Sets the world coordinates
     *
     * @param[in]  x world axis
     * @param[in]  y world axis
     * @param[in]  z world axis
     */
    UNICORN_EXPORT void SetWorldCoordinates(glm::vec3 x, glm::vec3 y, glm::vec3 z);

    /**
     * @brief Sets translate
     *
     * @param[in]  translate new translate vector
     */
    UNICORN_EXPORT void SetTranslation(glm::vec3 translate);

    /**
     * @brief Returns direction
     *
     * @return direction
     */
    UNICORN_EXPORT glm::vec3 GetDirection() const;

    /**
     * @brief Return right vector
     *
     * @return right vector
     */
    UNICORN_EXPORT glm::vec3 GetRight() const;

    /**
     * @brief Returns up vector
     *
     * @return up vector
     */
    UNICORN_EXPORT glm::vec3 GetUp() const;

    /**
     * @brief Return translate
     *
     * @return translate
     */
    UNICORN_EXPORT glm::vec3 GetTranslate() const;

    /**
     * @brief Returns model matrix
     *
     * Don't forget to recalculate @sa Calculate before
     *
     * @return model matrix
     */
    UNICORN_EXPORT glm::mat4 const& GetModelMatrix() const;

    /**
     * @brief Translates by local X - right vector
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateLocalX(float distance);

    /**
     * @brief Translates by local Y - up vector
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateLocalY(float distance);

    /**
     * @brief Translates by local Z - direction vector
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateLocalZ(float distance);

    /**
     * @brief Translates by local X
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateWorldX(float distance);

    /**
     * @brief Translates by local Y
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateWorldY(float distance);

    /**
     * @brief Translates by local Z
     *
     * @param[in] distance distance to translate
     */
    UNICORN_EXPORT void TranslateWorldZ(float distance);

    /**
     * @brief Rotates by X axis counterclockwise
     *
     * @param[in] radians amount of radians
     */
    UNICORN_EXPORT void RotateX(float radians);

    /**
     * @brief Rotates by Y axis counterclockwise
     *
     * @param[in] radians amount of radians
     */
    UNICORN_EXPORT void RotateY(float radians);

    /**
     * @brief Rotates by Z axis counterclockwise
     *
     * @param[in] radians amount of radians
     */
    UNICORN_EXPORT void RotateZ(float radians);

    /**
     * @brief Rotates by all axises counterclockwise
     *
     * @param[in] rotation amount of radians at each axis
     */
    UNICORN_EXPORT void Rotate(glm::vec3 rotation);

    /**
     * @brief Rotates by quaternion
     *
     * @param[in] rotation quaternion
     */
    UNICORN_EXPORT void Rotate(glm::quat rotation);

    /**
     * @brief Rotates around point
     *
     * @param[in] radians amount of radians in frame
     * @param[in] axis axis of rotation
     * @param[in] point point of interest
     */
    UNICORN_EXPORT void RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point);

    /**
     * @brief Rotates around axis
     *
     * @param[in] angleRadians amount of radians to rotate
     * @param[in] axis axis of rotation
     */
    UNICORN_EXPORT void Rotate(float angleRadians, glm::vec3 axis);
protected:

    /** @brief Abstract method to calculate orientation quaternion */
    virtual void CalculateOrientation();

    /** @brief Recalculates model matrix */
    void Update();

    glm::vec3 m_rotation;
    glm::vec3 m_translation;
    glm::quat m_orientation;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;
    glm::vec3 m_rightVector;

    glm::vec3 m_worldX;
    glm::vec3 m_worldY;
    glm::vec3 m_worldZ;

    glm::mat4 m_transformMatrix;

    bool m_isDirty;
};
}
}

#endif // UNICORN_VIDEO_TRANSFORM_HPP
