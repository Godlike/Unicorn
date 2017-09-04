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
    *  @param[in] mesh    geometry mesh
    */
    UNICORN_EXPORT static void Cube(Mesh& mesh);

    /**
    *  @brief Fills given mesh with quad data
    *
    *  @param[in] mesh    geometry mesh
    */
    UNICORN_EXPORT static void Quad(Mesh& mesh);

    /**
    *  @brief Fills given mesh with sphere data
    *
    *  @param[in] mesh geometry mesh
    *  @param[in] radius radius of sphere
    *  @param[in] rings horizontal slices count
    *  @param[in] sectors vertical slices count
    *  @attention rings * sectors must be higher then 16, less then std::vector::max_size() and less then sizeof(uint32_t)
    */
    UNICORN_EXPORT static void Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors);
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
