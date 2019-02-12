/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_PRIMITIVES_HPP
#define UNICORN_VIDEO_PRIMITIVES_HPP

#include <unicorn/video/Mesh.hpp>

#include <string>
#include <list>

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
    *  @param[in, out] mesh geometry mesh
    *  @return reference to filled mesh
    */
    static Mesh& Box(Mesh& mesh);

    /**
    *  @brief Fills given mesh with quad data
    *
    *  @param[in, out] mesh geometry mesh
    *  @return reference to updated mesh
    */
    static Mesh& Quad(Mesh& mesh);

    /**
    *  @brief Fills given mesh with sphere data
    *
    *  @param[out] mesh geometry mesh
    *  @param[in] radius radius of sphere (> 0)
    *  @param[in] rings horizontal slices count (>= 4)
    *  @param[in] sectors vertical slices count (>= 4)
    *  @attention `rings * sectors` must be less than `std::vector<Vertex>::max_size()`
    *  @return reference to updated mesh
    */
    static Mesh& Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors);

    /**
    @brief Loads and processes model

    * Loads model from given filepath, initializes Materials and Meshes from the model
    *
    * Supported formats:
    * - gltf 2.0 (without binary glb)
    * - obj
    * - fbx (without embedded textures)
    *
    * @todo use storage handler when assimp's issues regarding loading from memory are fixed
    *
    *  @param[in] path path to model
    *  @return list of pointers to meshes
    */
    static std::list<Mesh*> LoadModel(std::string const& path);
};
}
}

#endif // UNICORN_VIDEO_PRIMITIVES_HPP
