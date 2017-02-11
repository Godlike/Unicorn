/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_MESH_HPP
#define VORPAL_GRAPHICS_MESH_HPP

#include <string>

namespace vp
{
namespace graphics
{
/**
 * @brief The Mesh class have indices and vertices of loaded .obj file.
 */
class Mesh
{
public:
    bool LoadFromFile(const std::string& path);
};
}
}

#endif // VORPAL_GRAPHICS_MESH_HPP
