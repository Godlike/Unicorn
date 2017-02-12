/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_MESH_HPP
#define UNICORN_GRAPHICS_MESH_HPP

#include <string>
#include <unicorn/utility/SharedMacros.hpp>

namespace uc
{
namespace graphics
{
/**
 * @brief The Mesh class have indices and vertices of loaded .obj file.
 */
class UNICORN_EXPORT Mesh
{
public:
    bool LoadFromFile(const std::string& path);
};
}
}

#endif // UNICORN_GRAPHICS_MESH_HPP
