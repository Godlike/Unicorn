/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>
#include <unicorn/utility/Logger.hpp>

#include <glm/gtc/constants.hpp>

namespace unicorn
{
namespace video
{
void Primitives::Cube(Mesh& mesh)
{
    std::vector<Vertex> vertices{{
        //front
        {{-0.5f, -0.5f, 0.5f},{1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f},{0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f},{0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f, 1.0f}},

        //right
        {{0.5f, 0.5f, 0.5f},{1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f},{0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f},{0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f},{1.0f, 0.0f}},

        //back
        {{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f},{0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f},{0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f},{1.0f, 1.0f}},

        //left
        {{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f},{0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f},{1.0f, 1.0f}},

        //upper
        {{0.5f, 0.5f, 0.5f},{1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f},{0.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f},{1.0f, 1.0f}},

        //bottom
        {{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f},{0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f},{0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f},{1.0f, 1.0f}},
    }};

    mesh.SetMeshData(vertices, {
                         0, 1, 2, 0, 2, 3,
                         4, 5, 6, 4, 6, 7,
                         8, 9, 10, 8, 10, 11,
                         12, 13, 14, 12, 14, 15,
                         16, 17, 18, 16, 18, 19,
                         20, 21, 22, 20, 22, 23});
}

void Primitives::Quad(Mesh& mesh)
{
    std::vector<Vertex> vertices{{
        {{-0.5f , -0.5f , 0.0f},{1.0f , 0.0f}} ,
        {{0.5f , -0.5f , 0.0f},{0.0f , 0.0f}} ,
        {{0.5f , 0.5f , 0.0f},{0.0f , 1.0f}},
        {{-0.5f , 0.5f , 0.0f},{1.0f , 1.0f}},
    }};

    mesh.SetMeshData(vertices, {0, 1, 2, 2, 3, 0});
}

void Primitives::Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors)
{
    if(radius < 0)
    {
        LOG_WARNING("Sphere radius less then 0! UV will be inverted!");
    }
    if(rings < 4 || sectors < 4)
    {
        LOG_WARNING("Rings or sectors are less then 4, sphere will not be generated!");
        return;
    }

    std::vector<uint32_t> indices;
    std::vector<Vertex> temp_vertices;

    uint32_t vectorSize = rings * sectors;

    if(vectorSize > temp_vertices.max_size())
    {
        LOG_WARNING("Number of vertices is too big, sphere will not be generated!");
        return;
    }

    temp_vertices.resize(rings * sectors);
    {
        float const R = 1.f / static_cast<float>(rings - 1);
        float const S = 1.f / static_cast<float>(sectors - 1);

        auto vert_iter = temp_vertices.begin();
        for(uint32_t r = 0; r < rings; r++)
        {
            for(uint32_t s = 0; s < sectors; s++)
            {
                float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
                float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                *vert_iter++ = {{x * radius , y * radius , z * radius},{s * S, r * R}};
            }
        }
    }

    indices.resize(sectors * rings * 6);
    auto indices_iter = indices.begin();
    for(uint32_t x = 0; x < sectors; x++)
    {
        for(uint32_t y = 0; y < rings; y++)
        {
            uint32_t left = x;
            float right = static_cast<float>((x + 1) % sectors);
            float top = static_cast<float>(y);
            float bottom = static_cast<float>((y + 1) % rings);
            *indices_iter++ = {static_cast<uint32_t>(left + top * sectors)};
            *indices_iter++ = {static_cast<uint32_t>(left + bottom * sectors)};
            *indices_iter++ = {static_cast<uint32_t>(right + top * sectors)};
            *indices_iter++ = {static_cast<uint32_t>(right + top * sectors)};
            *indices_iter++ = {static_cast<uint32_t>(left + bottom * sectors)};
            *indices_iter++ = {static_cast<uint32_t>(right + bottom * sectors)};
        }
    }

    mesh.SetMeshData(temp_vertices, indices);
}
}
}
