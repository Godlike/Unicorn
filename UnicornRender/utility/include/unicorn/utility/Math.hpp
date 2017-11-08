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
 * @brief Computes rotation quaternion between two vectors
 *
 * @param[in]  start   origin vector
 * @param[in]  dest    destination vector
 * @param[in]  worldX  world X axis
 * @param[in]  worldZ  world Z axis
 *
 * Realization : realtime rendering 3rd edition, 79 page
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
 *
 * @return orientation quaternion
 */
glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp
    , glm::vec3 worldY = glm::vec3(0.0f, 1.0f, 0.0f)
    , glm::vec3 worldZ = glm::vec3(0.0f, 0.0f, 1.0f));

}
}
}

#endif // UNICORN_UTILITY_LOGGER_HPP
