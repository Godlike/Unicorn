/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_SHADER_HPP
#define VORPAL_GRAPHICS_SHADER_HPP

#include <string>

namespace vp
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

#endif // VORPAL_GRAPHICS_SHADER_HPP
