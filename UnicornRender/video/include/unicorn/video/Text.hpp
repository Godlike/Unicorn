/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TEXT_HPP
#define UNICORN_VIDEO_TEXT_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <glm/glm.hpp>

#include <string>

namespace unicorn
{
namespace video
{
class Text
{
public:
    Text(std::string const& textString, glm::vec2 pos, int size);

    void SetColor(glm::vec3 color);
private:
    std::string m_textString;
    glm::vec2 m_position;
};
}
}

#endif // UNICORN_VIDEO_TEXT_HPP
