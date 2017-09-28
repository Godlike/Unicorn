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
    *  @param[out] mesh    geometry mesh
    *  @return pointer to updated mesh
    */
    UNICORN_EXPORT static Mesh& Cube(Mesh& mesh);

    /**
    *  @brief Fills given mesh with quad data
    *
    *  @param[out] mesh    geometry mesh
    *  @return pointer to updated mesh
    */
    UNICORN_EXPORT static Mesh& Quad(Mesh& mesh);

    /**
    *  @brief Fills given mesh with sphere data
    *
    *  @param[out] mesh geometry mesh
    *  @param[in] radius radius of sphere (> 0)
    *  @param[in] rings horizontal slices count (>= 4)
    *  @param[in] sectors vertical slices count (>= 4)
    *  @attention `rings * sectors` must be less than `std::vector<Vertex>::max_size()`
    *  @return pointer to updated mesh
    */
    UNICORN_EXPORT static Mesh& Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors);
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
