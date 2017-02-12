/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_SHADER_HPP
#define UNICORN_GRAPHICS_SHADER_HPP

#include <string>

namespace uc
{
namespace graphics
{
/**
 * @brief The Shader class load shader binary info.
 */
class Shader
{
    bool LoadFromFile(const std::string& path);
};
}
}

#endif // UNICORN_GRAPHICS_SHADER_HPP
