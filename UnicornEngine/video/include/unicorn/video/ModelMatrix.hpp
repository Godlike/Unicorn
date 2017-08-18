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
class ModelMatrix
{
public:
		ModelMatrix();
    /**
    * @brief Rotate mesh
    * @param angle angle in degrees
    * @param axis vector of normalized axes, which you need to rotate, for example {1.f, 1.f, 0.f} will rotate by x and y only
    */
    UNICORN_EXPORT void Rotate(float angle, const glm::vec3& axis);

    /**
    * @brief Moves mesh
    * @param translateVector vector of deltas by axes
    */
    UNICORN_EXPORT void Translate(const glm::vec3& translateVector);

    /**
    * @brief Scales mesh
    * @param scaleVector vector of scale factors
    */
    UNICORN_EXPORT void Scale(const glm::vec3& scaleVector);    
    /**
    * @brief Model matrix of this mesh for MVP transformations
    */
    glm::mat4 model;
};
}
}

#endif // UNICORN_VIDEO_MODEL_MATRIX_HPP
