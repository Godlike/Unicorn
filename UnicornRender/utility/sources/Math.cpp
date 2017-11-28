/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/utility/Math.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace unicorn
{
namespace utility
{
namespace math
{

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest, glm::vec3 worldX, glm::vec3 worldZ)
{
    start = glm::normalize(start);
    dest = glm::normalize(dest);

    float const cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;

    float const floatError = 0.001f;

    if (cosTheta < -1 + floatError) // they look in different sides
    {
        rotationAxis = glm::cross(worldZ, start);
        if (glm::length2(rotationAxis) < floatError) // they are parallel
        {
            rotationAxis = glm::cross(worldX, start);
        }
        rotationAxis = glm::normalize(rotationAxis);

        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }

    rotationAxis = glm::cross(start, dest);

    float const s = glm::sqrt((1 + cosTheta) * 2);
    float const invs = 1 / s;

    return glm::quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
}

glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp, glm::vec3 worldY, glm::vec3 worldZ)
{
    if (length2(direction) < 0.0001f )
    {
        return glm::quat();
    }

    glm::vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);

    glm::quat rot1 = RotationBetweenVectors(worldZ, direction);
    glm::quat rot2 = RotationBetweenVectors(worldY, desiredUp);

    return rot2 * rot1;
}
}
}
}
