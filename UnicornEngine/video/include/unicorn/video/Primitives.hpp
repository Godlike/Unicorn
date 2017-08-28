/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PRIMITIVES_HPP
#define UNICORN_VIDEO_PRIMITIVES_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Mesh.hpp>

namespace unicorn
{
namespace video
{
/** @brief  Fills meshes with geometric primitive's data */
class Primitives
{
public:
    /**
    *  @brief Fills given mesh with cube data
    *
    *  @param  mesh    geometry mesh
    */
    UNICORN_EXPORT static void Cube(Mesh& mesh);

    /**
    *  @brief Fills given mesh with quad data
    *
    *  @param  mesh    geometry mesh
    */
    UNICORN_EXPORT static void Quad(Mesh& mesh);
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
