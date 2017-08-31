/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MODEL_MATRIX_HPP
#define UNICORN_VIDEO_MODEL_MATRIX_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
/*
 * @brief Holds all transformations for model view projection multiplication
 */
class ModelMatrix
{
public:
    UNICORN_EXPORT ModelMatrix();
    /**
    * @brief Rotate object
    * 
    * Example {1.f, 1.f, 0.f} will rotate by x and y only
    * 
    * @param[in] angle angle in degrees
    * @param[in] axis vector of normalized axes, which you need to rotate
    */
    UNICORN_EXPORT void Rotate(float angle, glm::vec3 const& axis);

    /**
    * @brief Moves object
    * @param[in] translateVector vector of deltas by axes
    */
    UNICORN_EXPORT void Translate(glm::vec3 const& translateVector);

    /**
    * @brief Sets position
    * @param[in] position new position
    */
    UNICORN_EXPORT void SetPosition(glm::vec3 const& position);

    /**
    * @brief Scales object
    * @param[in] scaleVector vector of scale factors
    */

    UNICORN_EXPORT void Scale(glm::vec3 const& scaleVector);
    /**
    * @brief Model matrix for MVP transformations
    */
    glm::mat4 model;
private:
    glm::vec3 m_position;
};
}
}

#endif // UNICORN_VIDEO_MODEL_MATRIX_HPP
