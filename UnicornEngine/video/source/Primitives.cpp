/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>

#include <glm/gtc/constants.hpp>

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
    float const R = 1. / static_cast<float>(rings - 1);
    float const S = 1. / static_cast<float>(sectors - 1);

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
    std::vector<uint16_t> indices;

    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    std::vector<float>::iterator v = vertices.begin();
    std::vector<float>::iterator n = normals.begin();
    std::vector<float>::iterator t = texcoords.begin();

    std::vector<Vertex> temp_vertices;

    for(int r = 0; r < rings; r++)
    {
        for(int s = 0; s < sectors; s++)
        {
            float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
            float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
            float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
            Vertex vrt;
            vrt.pos = {x * radius , y * radius , z * radius};
            vrt.tc = {s * S, r * R};
            temp_vertices.push_back(vrt);
        }
    }

    for(int x = 0; x < sectors; x++)
    {
        for(int y = 0; y < rings; y++)
        {
            int left = x;
            float right = (x + 1) % sectors;
            float top = y;
            float bottom = (y + 1) % rings;
            indices.push_back(left + top * sectors);
            indices.push_back(left + bottom * sectors);
            indices.push_back(right + top * sectors);
            indices.push_back(right + top * sectors);
            indices.push_back(left + bottom * sectors);
            indices.push_back(right + bottom * sectors);
        }
    }

    mesh.SetMeshData(temp_vertices, indices);
}
}
}
