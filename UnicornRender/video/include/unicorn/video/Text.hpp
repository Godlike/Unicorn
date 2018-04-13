/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_TEXT_HPP
#define UNICORN_VIDEO_TEXT_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <glm/glm.hpp>
#include <wink/signal.hpp>

#include <string>

namespace unicorn
{
namespace video
{
class Text
{
public:
    UNICORN_EXPORT Text(std::string const& textString, glm::vec3 pos, int size);

    UNICORN_EXPORT void SetColor(glm::vec3 color);
    UNICORN_EXPORT void SetPosition(glm::vec3 position);
    UNICORN_EXPORT void SetText(std::string text);
    UNICORN_EXPORT void SetSize(uint8_t size);

    UNICORN_EXPORT glm::vec3 GetColor() const;
    UNICORN_EXPORT glm::vec3 GetPosition() const;
    UNICORN_EXPORT std::string const& GetText() const;
    UNICORN_EXPORT uint8_t GetSize() const;

    wink::signal<wink::slot<void(Text*)>> TextUpdated;
private:
    std::string m_textString;
    glm::vec3 m_position;
    glm::vec3 m_color;
    uint8_t m_size;
};
}
}

#endif // UNICORN_VIDEO_TEXT_HPP
