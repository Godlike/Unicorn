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
/** @brief todo */
class Transform
{
public:
    Transform()
        : m_rotation(0)
        , m_translation(0)
        , m_orientation(glm::vec3(0))
        , m_upVector({ 0.f, 1.f, 0.f })
        , m_direction({ 0.f, 0.f, 1.f })
        , m_rightVector({ 1., 0.f, 0.f })
        , m_worldX({ 1., 0.f, 0.f })
        , m_worldY({ 0.f, 1.f, 0.f })
        , m_worldZ({ 0.f, 0.f, 1.f })
        , m_transformMatrix(1.f)
        , m_isDirty(true)
    {

    }

    UNICORN_EXPORT void LookAtDirection(glm::vec3 direction)
    {
        glm::mat4 mat = lookAt(m_translation, m_translation + direction, m_upVector);
        m_orientation = normalize(quat_cast(mat));

        m_isDirty = true;
    }

    UNICORN_EXPORT void LookAtPoint(glm::vec3 point)
    {
        glm::mat4 mat = lookAt(m_translation, point, m_upVector);
        m_orientation = normalize(quat_cast(mat));

        m_isDirty = true;
    }

    UNICORN_EXPORT void SetUp(glm::vec3 upVector)
    {
        glm::mat4 mat = lookAt(m_translation, m_translation + m_direction, upVector);
        m_orientation = normalize(quat_cast(mat));

        m_isDirty = true;
    }

    UNICORN_EXPORT void SetRight(glm::vec3 rightVector)
    {
        m_rightVector = rightVector;
        m_upVector = cross(m_direction, m_rightVector);

        m_isDirty = true;
    }

    UNICORN_EXPORT void SetTranslate(glm::vec3 translate)
    {
        m_translation = translate;

        m_isDirty = true;
    }

    UNICORN_EXPORT void SetWorldCoordinates(glm::vec3 x, glm::vec3 y, glm::vec3 z)
    {
        m_worldX = x;
        m_worldY = y;
        m_worldZ = z;
    }

    UNICORN_EXPORT void Translate(glm::vec3 translate)
    {
        m_translation += translate;
        
        m_isDirty = true;
    }

    UNICORN_EXPORT glm::vec3 GetDirection() const
    {
        return m_direction;
    }

    UNICORN_EXPORT glm::vec3 GetRight() const
    {
        return m_rightVector;
    }

    UNICORN_EXPORT glm::vec3 GetUp() const
    {
        return m_upVector;
    }

    UNICORN_EXPORT glm::vec3 GetTranslate() const
    {
        return m_translation;
    }

    UNICORN_EXPORT void TranslateLocal(glm::vec3 translate)
    {

        m_isDirty = true;
    }

    UNICORN_EXPORT void TranslateGlobal(glm::vec3 translate)
    {


        m_isDirty = true;
    }

    UNICORN_EXPORT void TranslateLocalX(float distance) 
    {
        Translate(m_rightVector * distance);
    }

    UNICORN_EXPORT void TranslateLocalY(float distance)
    {
        Translate(m_upVector * distance);
    }

    UNICORN_EXPORT void TranslateLocalZ(float distance)
    {
        Translate(m_direction * distance);
    }

    UNICORN_EXPORT void TranslateWorldX(float distance)
    {
        Translate(m_worldX * distance);
    }

    UNICORN_EXPORT void TranslateWorldY(float distance)
    {
        Translate(m_worldY * distance);
    }

    UNICORN_EXPORT void TranslateWorldZ(float distance)
    {
        Translate(m_worldZ * distance);
    }

    UNICORN_EXPORT void RotateX(float radians)
    {
        m_rotation.x += radians;
     
        m_isDirty = true;
    }

    UNICORN_EXPORT void RotateY(float radians)
    {
        m_rotation.y += radians;

        m_isDirty = true;
    }

    UNICORN_EXPORT void RotateZ(float radians)
    {
        m_rotation.z += radians;

        m_isDirty = true;
    }

    UNICORN_EXPORT void Rotate(glm::vec3 rotation)
    {
        m_rotation += rotation;
        m_isDirty = true;
    }

    UNICORN_EXPORT void Rotate(glm::quat rotation)
    {
        m_orientation = rotation * m_orientation;
    }

    UNICORN_EXPORT void RotateAroundPoint(float radians, glm::vec3 axis, glm::vec3 point)
    {
        glm::vec3 dir = point - m_translation;
        Translate(dir);
        glm::quat q = angleAxis(radians, axis);
        Translate(q * -dir);
        m_isDirty = true;
    }

    UNICORN_EXPORT void Rotate(float angleRadians, glm::vec3 axis) {
        glm::quat q = glm::angleAxis(angleRadians, axis);
        Rotate(q);
    }
protected:
    virtual void CalculateOrientation()
    {
        glm::quat x = glm::angleAxis(m_rotation.x, m_worldX);
        glm::quat y = glm::angleAxis(m_rotation.y, m_worldY);
        glm::quat z = glm::angleAxis(m_rotation.z, m_worldZ);

        m_orientation = z * x * y * m_orientation;

        m_rotation = { 0 };
    }

    void Update()
    {
        if (m_isDirty)
        {
            CalculateOrientation();

            m_direction = glm::conjugate(m_orientation) * m_worldZ;
            m_upVector = glm::conjugate(m_orientation) * m_worldY;
            m_rightVector = glm::cross(m_upVector, m_direction);

            m_transformMatrix = glm::lookAt(m_translation, m_translation + m_direction, m_upVector);

            m_isDirty = false;
        }
    }

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
