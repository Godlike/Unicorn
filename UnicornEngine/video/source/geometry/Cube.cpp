/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Cube.hpp>

namespace unicorn
{
namespace video
{
namespace geometry
{
Cube::Cube(std::shared_ptr<Mesh> mesh)
    : MeshDescriptor(mesh)
{
    std::vector<Vertex> temp_vertices;

    //front
    temp_vertices.push_back({{-1.0, -1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, 1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, 1.0}, {Color::White}});

    //right
    temp_vertices.push_back({{1.0, 1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, 1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, 1.0}, {Color::White}});

    //back
    temp_vertices.push_back({{-1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, 1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, -1.0}, {Color::White}});

    //left
    temp_vertices.push_back({{-1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, -1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, -1.0}, {Color::White}});

    //upper
    temp_vertices.push_back({{1.0, 1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, 1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, 1.0, -1.0}, {Color::White}});
    //bottom
    temp_vertices.push_back({{-1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, -1.0}, {Color::White}});
    temp_vertices.push_back({{1.0, -1.0, 1.0}, {Color::White}});
    temp_vertices.push_back({{-1.0, -1.0, 1.0}, {Color::White}});

    m_mesh->SetMeshData(temp_vertices, {0, 1, 2, 0, 2, 3, //front
                                           4,
                                           5,
                                           6,
                                           4,
                                           6,
                                           7, //right
                                           8,
                                           9,
                                           10,
                                           8,
                                           10,
                                           11, //back
                                           12,
                                           13,
                                           14,
                                           12,
                                           14,
                                           15, //left
                                           16,
                                           17,
                                           18,
                                           16,
                                           18,
                                           19, //upper
                                           20,
                                           21,
                                           22,
                                           20,
                                           22,
                                           23});
}
}
}
}