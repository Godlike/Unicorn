/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Text.hpp>
#include <unicorn/video/Color.hpp>

namespace unicorn
{
namespace video
{

Text::Text(std::string const& textString, glm::vec3 pos, int size) 
    : m_textString(textString), m_position(pos), m_size(size), m_color(Color::White())
{
}

void Text::SetColor(glm::vec3 color)
{
}

void Text::SetPosition(glm::vec3 position)
{
}

void Text::SetText(std::string text)
{
    m_textString = text;
    TextUpdated.emit(this);
}

void Text::SetSize(uint8_t size)
{
}

glm::vec3 Text::GetColor() const
{
    return m_color;
}

glm::vec3 Text::GetPosition() const
{
    return m_position;
}

std::string const & Text::GetText() const
{
    return m_textString;
}

uint8_t Text::GetSize() const
{
    return m_size;
}

}
}
