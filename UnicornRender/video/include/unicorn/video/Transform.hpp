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
 * @brief Base class for every object that can translate or rotate
 */
class Transform
{
public:
    /** @brief Default constructor */
    Transform();

    /** @brief Default destructor */
    virtual ~Transform() = default;

    /**
     * @brief Sets camera look at provided direction
     *
     * @param[in] direction Camera new direction vector
     */
    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction);

    /**
     * @brief Sets camera look at provided direction
     *
     * @param[in] direction Camera new direction vector
     * @param[in] upVector Camera new up vector
     */
    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction, glm::vec3 upVector);

    /**
     * @brief Sets camera up vector
     *
     * @param[in] upVector The up vector
     */
    UNICORN_EXPORT void SetUp(glm::vec3 upVector);

    /**
     * @brief Sets camera translate
     *
     * @param[in] translate
     */
    UNICORN_EXPORT void SetTranslate(glm::vec3 translate);

    /**
     * @brief      Sets the world coordinates.
     *
     * @param[in]  x     { parameter_description }
     * @param[in]  y     { parameter_description }
     * @param[in]  z     { parameter_description }
     */
    UNICORN_EXPORT void SetWorldCoordinates(glm::vec3 x, glm::vec3 y, glm::vec3 z);

    /**
     * @brief      { function_description }
     *
     * @param[in]  translate  The translate
     */
    UNICORN_EXPORT void Translate(glm::vec3 translate);

    /**
     * @brief      Gets the direction.
     *
     * @return     The direction.
     */
    UNICORN_EXPORT glm::vec3 GetDirection() const;

    /**
     * @brief      Gets the right.
     *
     * @return     The right.
     */
    UNICORN_EXPORT glm::vec3 GetRight() const;

    /**
     * @brief      { function_description }
     *
     * @return     The up.
     */
    UNICORN_EXPORT glm::vec3 GetUp() const;

    /**
     * @brief      Gets the translate.
     *
     * @return     The translate.
     */
    UNICORN_EXPORT glm::vec3 GetTranslate() const;

    /**
     * @brief Returns model matrix
     *
     * @return model matrix
     *
     * Don't forget to recalculate @sa Calculate
     */
    UNICORN_EXPORT glm::mat4 const& GetModelMatrix() const;
    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateLocalX(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateLocalY(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateLocalZ(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateWorldX(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateWorldY(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  distance  The distance
     */
    UNICORN_EXPORT void TranslateWorldZ(float distance);

    /**
     * @brief      { function_description }
     *
     * @param[in]  radians  The radians
     */
    UNICORN_EXPORT void RotateX(float radians);

    /**
     * @brief      { function_description }
     *
     * @param[in]  radians  The radians
     */
    UNICORN_EXPORT void RotateY(float radians);

    /**
     * @brief      { function_description }
     *
     * @param[in]  radians  The radians
     */
    UNICORN_EXPORT void RotateZ(float radians);

    /**
     * @brief      { function_description }
     *
     * @param[in]  rotation  The rotation
     */
    UNICORN_EXPORT void Rotate(glm::vec3 rotation);

    /**
     * @brief      { function_description }
     *
     * @param[in]  rotation  The rotation
     */
    UNICORN_EXPORT void Rotate(glm::quat rotation);

    /**
     * @brief      { function_description }
     *
     * @param[in]  radians  The radians
     * @param[in]  axis     The axis
     * @param[in]  point    The point
     */
    UNICORN_EXPORT void RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point);

    /**
     * @brief      { function_description }
     *
     * @param[in]  angleRadians  The angle radians
     * @param[in]  axis          The axis
     */
    UNICORN_EXPORT void Rotate(float angleRadians, glm::vec3 axis);
protected:
    /**
     * @brief      { function_description }
     */
    virtual void CalculateOrientation();

    /**
     * @brief      { function_description }
     */
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
