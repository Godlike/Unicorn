/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TRANSFORM_HPP
#define UNICORN_VIDEO_TRANSFORM_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{
/** @brief Base class for every object that can be translated or rotated */
class Transform
{
public:
    /** @brief Default constructor */
    Transform();

    /** @brief Default destructor */
    virtual ~Transform() = default;

    /** @brief Returns true if transformations must be recalculated @sa UpdateTransformMatrix, false otherwise */
    bool IsDirty() const;

    /**
     * @brief Updates orientation with provided up vector
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] upVector new up vector
     */
    void SetUp(glm::vec3 upVector);

    /**
     * @brief Sets the world axes and updates orientation
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in]  x world axis
     * @param[in]  y world axis
     * @param[in]  z world axis
     */
    void SetWorldAxes(glm::vec3 x, glm::vec3 y, glm::vec3 z);

    /**
     * @brief Sets world translation
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in]  translate new translate vector
     */
    void SetTranslation(glm::vec3 translate);

    /**
     * @brief Returns direction vector
     *
     * @return direction
     */
    glm::vec3 GetDirection() const;

    /**
     * @brief Returns right vector
     *
     * @return right vector
     */
    glm::vec3 GetRight() const;

    /**
     * @brief Returns up vector
     *
     * @return up vector
     */
    glm::vec3 GetUp() const;

    /**
     * @brief Returns world translation
     *
     * @return world translation
     */
    glm::vec3 GetTranslation() const;

    /**
     * @brief Returns model matrix
     *
     * @return model matrix
     */
    glm::mat4 const& GetModelMatrix() const;

    /**
     * @brief Scales object
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * Linearly scales each basis component of the mesh vectors by the given scale factors
     * Example: {1,1,1} is origin scale, {0.5, 0.5, 2} is x and y diminished twice
     * and z is twice bigger
     *
     * @param[in] scale basis scale factors
     */
    void Scale(glm::vec3 scale);

    /**
     * @brief Adds given @p distance to the translation vector with respect to current up, right and forward orientation vectors
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] distance per component distance
     */
    void TranslateByBasis(glm::vec3 distance);

    /**
    * @brief Applies transform from given matrix
    *
    * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
    *
    * @param[in] transformMatrix matrix containing the transformation
    */
    void TransformByMatrix(glm::mat4 const& transformMatrix);

    /**
     * @brief Translates by world axes
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] distance per component distance
     */
    void TranslateWorld(glm::vec3 distance);

    /**
     * @brief Rotates by all axes counterclockwise
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] rotation amount of radians at each axis
     */
    void Rotate(glm::vec3 rotation);

    /**
     * @brief Rotates by quaternion
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] rotation quaternion
     */
    void Rotate(glm::quat rotation);

    /**
     * @brief Rotates around axis
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] angleRadians amount of radians to rotate
     * @param[in] axis axis of rotation
     */
    void Rotate(float angleRadians, glm::vec3 axis);

    /**
     * @brief Sets new orientation quaternion
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] quat new orientation
     */
    void SetOrientation(glm::quat quat);

    /**
     * @brief Updates orientation with provided direction
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] direction new direction vector
     */
    void SetOrientation(glm::vec3 direction);

    /**
     * @brief Generates new orientation from provided direction and up vector
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] direction new direction vector
     * @param[in] upVector new up vector
     */
    void SetOrientation(glm::vec3 direction, glm::vec3 upVector);

    /**
     * @brief Constructs orientation from per component rotation
     *
     * @attention Don't forget to recalculate @sa UpdateTransformMatrix to commit all changes to the object
     *
     * @param[in] rotation per axis rotation
     */
    void SetRotation(glm::vec3 rotation);

    /** @brief Recalculates all transformation components and updates transform matrix */
    void UpdateTransformMatrix();
protected:
    /** @brief Abstract method to calculate orientation quaternion */
    virtual void UpdateOrientation();

    glm::vec3 m_rotation;
    glm::vec3 m_translation;
    glm::quat m_orientation;
    glm::vec3 m_scale;

    glm::vec3 m_worldX;
    glm::vec3 m_worldY;
    glm::vec3 m_worldZ;

    glm::vec3 m_rightVector;
    glm::vec3 m_upVector;
    glm::vec3 m_direction;

    glm::mat4 m_transformMatrix;

    bool m_isDirty;
};
}
}

#endif // UNICORN_VIDEO_TRANSFORM_HPP
