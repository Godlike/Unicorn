/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MESH_HPP
#define UNICORN_VIDEO_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/ModelMatrix.hpp>

#include <wink/signal.hpp>
#include <glm/glm.hpp>

#include <list>

namespace unicorn
{
namespace video
{
class Material;

/**
 * @brief Vertex information
 */
struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec2 tc);
    /**
     * @brief Position of vertex
     */
    glm::vec3 pos;
    /**
    * @brief Texture coordinates of vertex
    */
    glm::vec2 tc;
};

/**
* @brief Mesh data
*/
class Mesh
{
public:
    UNICORN_EXPORT Mesh(Material& material);
    UNICORN_EXPORT ~Mesh();

    /**
    * @brief Updates vertices and indices geometry
    * @param vertices vertexes data
    * @param indices indices data
    */
    UNICORN_EXPORT void SetMeshData(std::vector<Vertex> const& vertices, std::vector<uint16_t> const& indices);

    /**
    * @brief Sets new material
    * @param [in] material updated material for current mesh
    */
    UNICORN_EXPORT void SetMaterial(Material& material);

    /**
     * @brief Returns mesh vertices
     * @return Mesh vertices
     */
    UNICORN_EXPORT const std::vector<Vertex>& GetVertices() const;

    /**
    * @brief Returns mesh indices
    * @return Mesh indices
    */
    UNICORN_EXPORT std::vector<uint16_t> const& GetIndices() const;

    /**
    * @brief Returns mesh material
    * @return mesh material data
    */
    UNICORN_EXPORT Material const& GetMaterial() const;

    /**
    * @brief Matrix that contains model transformation
    */
    ModelMatrix modelMatrix;

    /**
    * @brief Signal for GPU that material was updated
    */

    wink::signal<wink::slot<void(Mesh*)>> MaterialUpdated;
    /**
    * @brief Signal for GPU data update
    */
    wink::signal<wink::slot<void()>> VerticesUpdated;
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
    Material* m_material;
};
}
}

#endif // UNICORN_VIDEO_MESH_HPP
