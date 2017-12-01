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
     * @param[in] material visual describing material
     */
    UNICORN_EXPORT Mesh(Material const& material);

    /** @brief Default destructor */
    UNICORN_EXPORT ~Mesh() = default;

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
    UNICORN_EXPORT void SetMaterial(Material const& material);

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

    /**
     * @brief Scales mesh
     *
     * @param[in] scale basis scale factors
     *
     * Lineary scales each basis component of the mesh vectors by the given scale factors
     * {1,1,1} is origin scale, {0.5, 0.5, 2} is x and y diminished twice
     * and z is twice bigger
     */
    UNICORN_EXPORT void Scale(glm::vec3 scale);

    /**
     * @brief Updates model matrix for model view projection calculation
     */
    UNICORN_EXPORT void Update();

    /** @brief Event triggered when material is changed */
    wink::signal<wink::slot<void()>> MaterialUpdated;

    /** @brief Event triggered when vertices are changed */
    wink::signal<wink::slot<void()>> VerticesUpdated;
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    Material m_material;
    glm::vec3 m_scale;
};
}
}

#endif // UNICORN_VIDEO_MESH_HPP
