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
void Primitives::Cube(Mesh& mesh)
{
    std::vector<Vertex> temp_vertices;
    //front
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), glm::vec2(1.0f, 1.0f));

    //right
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), glm::vec2(1.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), glm::vec2(1.0f, 0.0f));

    //back
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), glm::vec2(1.0f, 1.0f));

    //left
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), glm::vec2(1.0f, 1.0f));

    //upper
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, 1.0), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, 1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, 1.0, -1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, 1.0, -1.0), glm::vec2(1.0f, 1.0f));

    //bottom
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, -1.0), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, -1.0), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0, -1.0, 1.0), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0, -1.0, 1.0), glm::vec2(1.0f, 1.0f));

    mesh.SetMeshData(temp_vertices, {
                         0, 1, 2, 0, 2, 3,
                         4, 5, 6, 4, 6, 7,
                         8, 9, 10, 8, 10, 11,
                         12, 13, 14, 12, 14, 15,
                         16, 17, 18, 16, 18, 19,
                         20, 21, 22, 20, 22, 23});
}

void Primitives::Quad(Mesh& mesh)
{
    std::vector<Vertex> temp_vertices;
    temp_vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    temp_vertices.emplace_back(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
    temp_vertices.emplace_back(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f));

    mesh.SetMeshData(temp_vertices, {0, 1, 2, 2, 3, 0});
}
}
}
