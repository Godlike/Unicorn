/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP
#define UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/geometry/Mesh.hpp>

#include <glm/glm.hpp>
#include <memory>

namespace unicorn
{
namespace video
{
namespace geometry
{
/**
 * @brief This class is needed for wrapping raw mesh and manipulating it
 */
class MeshDescriptor
{
public:
    /**
     * @brief Mesh Descriptor constructor
     * @param mesh pointer to Renderer mesh
     */
    UNICORN_EXPORT MeshDescriptor(Mesh& mesh);
    /**
     * @brief Mesh Descriptor destructor
     */
    UNICORN_EXPORT ~MeshDescriptor();
    /**
     * @brief Rotate mesh
     * @param angle angle in degrees
     * @param axis vector of normalized axes, which you need to rotate, for example {1.f, 1.f, 0.f} will rotate by x and y only
     */
    UNICORN_EXPORT void Rotate(float angle, const glm::vec3& axis) const;
    /**
     * @brief Moves mesh
     * @param translateVector vector of deltas by axes
     */
    UNICORN_EXPORT void Translate( const glm::vec3& translateVector) const;
    /**
     * @brief Scales mesh
     * @param scaleVector vector of scale factors
     */
    UNICORN_EXPORT void Scale(const glm::vec3& scaleVector) const;
    /**
     * @brief Sets new color for mesh
     * @param color color value
     */
    UNICORN_EXPORT void SetColor(const glm::vec3& color) const;
protected:
    Mesh& m_mesh;
};
}
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP
