/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Color.hpp>

namespace unicorn
{
namespace video
{

glm::vec3 Color::Red()
{
    return {1.0f, 0.0f, 0.0f};
}

glm::vec3 Color::Green()
{
    return {0.0f, 1.0f, 0.0f};
}

glm::vec3 Color::Blue()
{
    return {0.0f, 0.0f, 1.0f};
}

glm::vec3 Color::White()
{
    return {1.0f, 1.0f, 1.0f};
}

glm::vec3 Color::Black()
{
    return {0.0f, 0.0f, 0.0f};
}

glm::vec3 Color::Pink()
{
    return {1.0f, 0.0f, 0.5f};
}

glm::vec3 Color::LightPink()
{
    return {1.0f, 0.4f, 0.75f};
}

}
}
