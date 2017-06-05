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
     * @param mesh Pointer to Renderer mesh
     */
    UNICORN_EXPORT MeshDescriptor(std::shared_ptr<Mesh> mesh);
    /**
     * @brief Mesh Descriptor destructor
     */
    UNICORN_EXPORT ~MeshDescriptor();
    /**
     * @brief Rotate mesh
     * @param angle Angle in degrees
     * @param axis vector of normalized axises, which you need to rotate, for example {1.f, 1.f, 0.f} will rotate by x and y only
     */
    UNICORN_EXPORT void Rotate(float angle, const glm::vec3& axis);
    /**
     * @brief Move mesh
     * @param translateVector vector of "deltas" by axises
     */
    UNICORN_EXPORT void Move( const glm::vec3& translateVector);
    /**
     * @brief Scale mesh
     * @param scaleVector vector of scales by each axis
     */
    UNICORN_EXPORT void Scale(const glm::vec3& scaleVector);
    /**
     * @brief Set new color for mesh
     * @param color Color value
     */
    UNICORN_EXPORT void SetColor(const glm::vec3& color);
protected:
    std::shared_ptr<Mesh> m_mesh;
};
}
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP
