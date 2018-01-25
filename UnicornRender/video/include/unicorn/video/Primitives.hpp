/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PRIMITIVES_HPP
#define UNICORN_VIDEO_PRIMITIVES_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Mesh.hpp>

#include <string>
#include <list>

namespace unicorn
{
namespace video
{
/** @brief  Fills meshes with geometric primitives data */
class Primitives
{
public:
    /**
    *  @brief Fills given mesh with cube data
    *
    *  @param[in, out] mesh geometry mesh
    *  @return pointer to filled mesh
    */
    UNICORN_EXPORT static Mesh* Box(Mesh* mesh = nullptr);

    /**
    *  @brief Fills given mesh with quad data
    *
    *  @param[in, out] mesh geometry mesh
    *  @return pointer to updated mesh
    */
    UNICORN_EXPORT static Mesh* Quad(Mesh* mesh = nullptr);

    /**
    *  @brief Fills given mesh with sphere data
    *
    *  @param[in] radius radius of sphere (> 0)
    *  @param[in] rings horizontal slices count (>= 4)
    *  @param[in] sectors vertical slices count (>= 4)
    *  @param[out] mesh geometry mesh
    *  @attention `rings * sectors` must be less than `std::vector<Vertex>::max_size()`
    *  @return pointer to updated mesh
    */
    UNICORN_EXPORT static Mesh* Sphere(float radius, uint32_t rings, uint32_t sectors, Mesh* mesh = nullptr);

    /**
    *  @brief Loading model from file, fills materials and returns list of pointers to meshes
    *
    *  Supported formats: gltf 2.0 (only gltf for now); obj
    *
    *  @param[in] path path to model
    *  @return list of pointers to meshes
    */
    UNICORN_EXPORT static std::list<Mesh*> LoadModel(std::string const& path);
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
