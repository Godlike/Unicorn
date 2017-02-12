/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_MESH_HPP
#define VORPAL_GRAPHICS_MESH_HPP

#include <string>
#include <vorpal/utility/SharedMacros.hpp>

namespace vp
{
namespace graphics
{
/**
 * @brief The Mesh class have indices and vertices of loaded .obj file.
 */
class VORPAL_EXPORT Mesh
{
public:
    bool LoadFromFile(const std::string& path);
};
}
}

#endif // VORPAL_GRAPHICS_MESH_HPP
