/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP
#define UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/Color.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace unicorn
{
    namespace video
    {
        namespace geometry
        {
            enum class Axis
            {
                X, Y, Z
            };

            class MeshDescriptor
            {
            public:
                UNICORN_EXPORT MeshDescriptor(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {};
                UNICORN_EXPORT ~MeshDescriptor() {} ;
                UNICORN_EXPORT  void Rotate(Axis axis, float angle);
                UNICORN_EXPORT  void Move(Axis axis, float diff);
                UNICORN_EXPORT  void Scale(Axis axis, float diff);
                UNICORN_EXPORT  void SetColor(Color color);
            protected:
                std::shared_ptr<Mesh> m_mesh;
            };
        }
    }
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_DESCRIPTOR_HPP