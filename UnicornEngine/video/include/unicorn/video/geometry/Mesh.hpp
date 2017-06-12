/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MESH_HPP
#define UNICORN_VIDEO_GEOMETRY_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <wink/signal.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace unicorn
{
namespace video
{
namespace geometry
{
/**
 * @brief Vertex information
 */
struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec3 color);
    /**
     * @brief Position of vertex
     */
    glm::vec3 pos;
    /**
     * @brief Color of vertex
     */
    glm::vec3 color;
};

/**
* @brief Mesh data
*/
class Mesh
{
public:
    UNICORN_EXPORT Mesh();
    UNICORN_EXPORT ~Mesh();

    /**
    * @brief Updates vertices and indices geometry
    * @param vertices vertexes data
    * @param indices indices data
    */
    UNICORN_EXPORT void SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
    /**
     * @brief Returns mesh vertices
     * @return Mesh vertices
     */
    UNICORN_EXPORT const std::vector<Vertex>& GetVertices() const;
    /**
    * @brief Returns mesh indices
    * @return Mesh indices
    */
    UNICORN_EXPORT const std::vector<uint16_t>& GetIndices() const;
    /**
     * @brief Apply given color to each vertex
     * @param ñolor color that will be applied to each vertex; each color component shall be in range [0.0, 1.0]
     */
    UNICORN_EXPORT void SetColor(const glm::vec3& ñolor);
    /**
     * @brief Signal for GPU data update
     */
    wink::signal<wink::slot<void()>> VerticesUpdated;
    /**
     * @brief Model matrix of this mesh for MVP transformations
     * Model matrix update uniform buffer every frame and don't need to emit VerticesUpdated
     */
    glm::mat4 model;
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
}
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_HPP
