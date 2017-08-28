/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>

#include <glm/gtc/constants.hpp>
#include "unicorn/utility/Logger.hpp"

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

void Primitives::Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors)
{
    if(radius < 0)
    {
        LOG_WARNING("Sphere radius less then 0! Sphere vertices will be inversed!");
    }
    if(rings < 4 || sectors < 4)
    {
        LOG_WARNING("Rings of sectors less then 4! No visible object will appear!");
    }

    std::vector<uint16_t> indices;
    std::vector<Vertex> temp_vertices;

    temp_vertices.resize(rings * sectors);
    {
        float const R = 1. / static_cast<float>(rings - 1);
        float const S = 1. / static_cast<float>(sectors - 1);

        auto vert_iter = temp_vertices.begin();
        for(uint32_t r = 0; r < rings; r++)
        {
            for(uint32_t s = 0; s < sectors; s++)
            {
                float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
                float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                *vert_iter++ = {{x * radius , y * radius , z * radius}, {s * S, r * R}};
            }
        }
    }

    indices.resize(sectors * rings * 6);
    auto indices_iter = indices.begin();
    for(uint32_t x = 0; x < sectors; x++)
    {
        for(uint32_t y = 0; y < rings; y++)
        {
            int left = x;
            float right = (x + 1) % sectors;
            float top = y;
            float bottom = (y + 1) % rings;
            *indices_iter++ = {static_cast<uint16_t>(left + top * sectors)};
            *indices_iter++ = {static_cast<uint16_t>(left + bottom * sectors)};
            *indices_iter++ = {static_cast<uint16_t>(right + top * sectors)};
            *indices_iter++ = {static_cast<uint16_t>(right + top * sectors)};
            *indices_iter++ = {static_cast<uint16_t>(left + bottom * sectors)};
            *indices_iter++ = {static_cast<uint16_t>(right + bottom * sectors)};
        }
    }

    mesh.SetMeshData(temp_vertices, indices);
}
}
}
