/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/ModelMatrix.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
namespace video
{
ModelMatrix::ModelMatrix() : model( glm::mat4( 1.0 ) )
{
}

void ModelMatrix::Rotate(float angle, const glm::vec3& axis)
{
    model = glm::rotate( model, glm::radians( angle ) , axis );
}

void ModelMatrix::Translate(const glm::vec3& translateVector)
{
    model = glm::translate( model, translateVector );
}

void ModelMatrix::Scale(const glm::vec3& scaleVector)
{
    model = glm::scale( model, scaleVector );
}
}
}
