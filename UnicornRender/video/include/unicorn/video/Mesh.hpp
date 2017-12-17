/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MESH_HPP
#define UNICORN_VIDEO_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Material.hpp>
#include <unicorn/video/Transform.hpp>

#include <wink/signal.hpp>
#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{

/** @brief Vertex information */
struct Vertex
{
    /** @brief Position of vertex */
    glm::vec3 pos;

    /** @brief Texture coordinates of vertex */
    glm::vec2 tc;
};

/** @brief Mesh data */
class Mesh : public Transform
{
public:
    /**
     * @brief Constructs mesh
     *
     * @param[in] material visual appearance description
     */
    UNICORN_EXPORT Mesh(Material material);

    /**
    * @brief Updates vertices and indices geometry
     *
    * @param[in] vertices vertices data
    * @param[in] indices indices data
    */
    UNICORN_EXPORT void SetMeshData(std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices);

    /**
    * @brief Sets new material
    *
    * @param[in] material material to be used for current mesh
    */
    UNICORN_EXPORT void SetMaterial(Material material);

    /**
     * @brief Returns mesh vertices
     *
     * @return Mesh vertices
     */
    UNICORN_EXPORT const std::vector<Vertex>& GetVertices() const;

    /**
    * @brief Returns mesh indices
    *
    * @return Mesh indices
    */
    UNICORN_EXPORT std::vector<uint32_t> const& GetIndices() const;

    /**
    * @brief Returns mesh material
    *
    * @return mesh material data
    */
    UNICORN_EXPORT Material const& GetMaterial() const;

    /** @brief Event triggered when material is changed */
    wink::signal<wink::slot<void()>> MaterialUpdated;

    /** @brief Event triggered when vertices are changed */
    wink::signal<wink::slot<void()>> VerticesUpdated;
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    Material m_material;
};
}
}

#endif // UNICORN_VIDEO_MESH_HPP
