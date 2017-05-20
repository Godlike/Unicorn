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
    UNICORN_EXPORT void Rotate(float angle, glm::vec3 axis);
    /**
     * @brief Move mesh
     * @param diff vector of "deltas" by axises
     */
    UNICORN_EXPORT void Move(glm::vec3 diff);
    /**
     * @brief Scale mesh
     * @param diff vector of scales by each axis
     */
    UNICORN_EXPORT void Scale(glm::vec3 diff);
    /**
     * @brief Set new color for mesh
     * @param color New color value from 0.0 to 1.0
     */
    UNICORN_EXPORT void SetColor(glm::vec3 color);
protected:
    std::shared_ptr<Mesh> m_mesh;
};
}
}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP
