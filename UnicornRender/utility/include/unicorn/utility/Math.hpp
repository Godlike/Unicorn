/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_MATH_HPP
#define UNICORN_UTILITY_MATH_HPP

#include <glm/glm.hpp>

namespace unicorn
{
namespace utility
{
namespace math
{

/**
 * @brief Calculates rotation quaternion between two vectors
 *
 * @param[in] start  origin vector
 * @param[in] dest   destination vector
 * @param[in] worldX world X axis
 * @param[in] worldZ world Z axis
 *
 * Realisation : Realtime rendering 3rd edition, 79 page by Eric Haines, Naty Hoffman, and Tomas Möller
 *
 * @return returns a quaternion such that q*start = dest
 */
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest
    , glm::vec3 worldX = glm::vec3(1.0f, 0.0f, 0.0f)
    , glm::vec3 worldZ = glm::vec3(0.0f, 0.0f, 1.0f));

/**
 * @brief Caclculates orientation quaternion
 *
 * @param[in] direction new direction
 * @param[in] desiredUp new up vector
 * @param[in] desiredUp new world Y vector
 * @param[in] desiredUp new world Z vector
 * @attention worldY and worldZ must be orthogonal
 *
 * @return orientation quaternion
 */
glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp
    , glm::vec3 worldY = glm::vec3(0.0f, 1.0f, 0.0f)
    , glm::vec3 worldZ = glm::vec3(0.0f, 0.0f, 1.0f));

/**
 * @brief Calculates new position, translated aroud provided point
 *
 * @param[in] originalTranslation original translation
 * @param[in] radians amount of radians in frame
 * @param[in] axis axis of rotation
 * @param[in] point point of interest
 *
 * @return calculated translation
 */
glm::vec3 RotateAroundPoint(glm::vec3 originalTranslation, float radians,
    glm::vec3 axis, glm::vec3 point);

}
}
}

#endif // UNICORN_UTILITY_MATH_HPP
