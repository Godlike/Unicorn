/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MESH_HPP
#define UNICORN_VIDEO_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>

#include <wink/signal.hpp>
#include <glm/glm.hpp>

#include <list>

namespace unicorn
{
namespace video
{
class Texture;

/**
 * @brief Vertex information
 */
struct Vertex
{
    Vertex(glm::vec3 pos);
    /**
     * @brief Position of vertex
     */
    glm::vec3 pos;
    /**
    * @brief Texture coordinates of vertex
    */
    glm::vec2 texCoords;
};

/**
* @brief Mesh data
*/
class Mesh
{
public:
    UNICORN_EXPORT Mesh();
    UNICORN_EXPORT Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::list<Texture*>& textures);
    UNICORN_EXPORT ~Mesh();

    /**
    * @brief Updates vertices and indices geometry
    * @param vertices vertexes data
    * @param indices indices data
    */
    UNICORN_EXPORT void SetMeshData(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::list<Texture*>& textures);
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

    UNICORN_EXPORT void AddTexture(const Texture& texture);
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
    std::list<Texture*> m_textures;
};
}
}

#endif // UNICORN_VIDEO_MESH_HPP
