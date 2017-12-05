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
     * @brief Sets the direction of the view
     *
     * @param[in] direction new direction vectorr
     */
    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction);

    /**
     * @brief Sets the direction of the view
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
    UNICORN_EXPORT glm::vec3 GetTranslation() const;

    /**
     * @brief Returns model matrix
     *
     * @attention Don't forget to recalculate @sa Calculate before
     *
     * @return model matrix
     */
    UNICORN_EXPORT glm::mat4 const& GetModelMatrix() const;

    /**
     * @brief Scales object
     *
     * @param[in] scale basis scale factors
     *
     * Lineary scales each basis component of the mesh vectors by the given scale factors
     * {1,1,1} is origin scale, {0.5, 0.5, 2} is x and y diminished twice
     * and z is twice bigger
     */
    UNICORN_EXPORT void Scale(glm::vec3 scale);

    /**
     * @brief Translates by axises
     *
     * @param[in] distance per component distance
     */
    UNICORN_EXPORT void TranslateLocal(glm::vec3 distance);
    /**
     * @brief Translates by world axises
     *
     * @param[in] distance per component distance
     */
    UNICORN_EXPORT void TranslateWorld(glm::vec3 distance);

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
     * @brief Rotates around axis
     *
     * @param[in] angleRadians amount of radians to rotate
     * @param[in] axis axis of rotation
     */
    UNICORN_EXPORT void Rotate(float angleRadians, glm::vec3 axis);

    /**
     * @brief Sets orientation quaternion
     *
     * @param[in] quat new orientation
     */
    UNICORN_EXPORT void SetOrientation(glm::quat quat);

    /**
     * @brief Constructs orientation from per component rotation
     *
     * @param[in] rotation per axis rotation
     */
    UNICORN_EXPORT void SetRotation(glm::vec3 rotation);

    /** @brief Abstract method to calculate orientation quaternion */
    virtual void UpdateOrientation();

    /** @brief Recalculates model matrix */
    void UpdateModelMatrix();
protected:
    glm::vec3 m_rotation;
    glm::vec3 m_translation;
    glm::quat m_orientation;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;
    glm::vec3 m_rightVector;
    glm::vec3 m_scale;

    glm::vec3 m_worldX;
    glm::vec3 m_worldY;
    glm::vec3 m_worldZ;

    glm::mat4 m_transformMatrix;

    bool m_isDirty;
};
}
}

#endif // UNICORN_VIDEO_TRANSFORM_HPP
