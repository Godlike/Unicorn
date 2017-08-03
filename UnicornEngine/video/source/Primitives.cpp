/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>

namespace unicorn
{
namespace video
{
Cube::Cube()
{
    std::vector<Vertex> temp_vertices;
    //front
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0));

    //right
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0));

    //back
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0));

    //left
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0));

    //upper
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0));
    //bottom
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0));
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0));

    Mesh::SetMeshData(temp_vertices, {0, 1, 2, 0, 2, 3,
                    4, 5, 6, 4, 6, 7,
                    8, 9, 10, 8, 10, 11,
                    12, 13, 14, 12, 14, 15,
                    16, 17, 18, 16, 18, 19,
                    21, 22, 20, 22, 23});
}

Quad::Quad()
{
    Mesh::SetMeshData({{{-1.0f, -1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}},
                    {{-1.0f, 1.0f, 0.0f}}},
                {0, 1, 2, 2, 3, 0});
}
}
}
