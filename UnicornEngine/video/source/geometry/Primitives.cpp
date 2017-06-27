/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Primitives.hpp>
#include <unicorn/video/Color.hpp>

namespace unicorn
{
namespace video
{
namespace geometry
{

MeshDescriptor Primitives::Triangle(Mesh& mesh)
{
    mesh.SetMeshData({{{ 0.0f, -0.5f,  0.0f}, { 1.0f,  1.0f,  1.0f}},
                      {{ 0.5f,  0.5f,  0.0f}, { 1.0f,  1.0f,  1.0f}},
                      {{-0.5f,  0.5f,  0.0f}, { 1.0f,  1.0f,  1.0f}}},
                      {0, 1, 2});

    return MeshDescriptor(mesh);
}

MeshDescriptor Primitives::Cube(Mesh& mesh)
{
    std::vector<Vertex> temp_vertices;
    //front
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), Color::White);

    //right
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), Color::White);

    //back
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), Color::White);

    //left
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), Color::White);

    //upper
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), Color::White);
    //bottom
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), Color::White);
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), Color::White);

    mesh.SetMeshData(temp_vertices, { 0,  1,  2,  0,  2,  3,
                                      4,  5,  6,  4,  6,  7,
                                      8,  9, 10,  8, 10, 11,
                                     12, 13, 14, 12, 14, 15,
                                     16, 17, 18, 16, 18, 19,
                                     21, 22, 20, 22, 23});

    return MeshDescriptor(mesh);
}

MeshDescriptor Primitives::Quad(Mesh& mesh)
{
    mesh.SetMeshData({{{-1.0f, -1.0f,  0.0f}, { 1.0f,  1.0f,  1.0f}},
                      {{ 1.0f, -1.0f,  0.0f}, { 1.0f,  1.0f,  1.0f}},
                      {{ 1.0f,  1.0f,  0.0f}, { 1.0f,  1.0f,  1.0f}},
                      {{-1.0f,  1.0f,  0.0f}, { 1.0f,  1.0f,  1.0f}}},
                      {0, 1, 2, 2, 3, 0});

    return MeshDescriptor(mesh);
}

}
}
}
