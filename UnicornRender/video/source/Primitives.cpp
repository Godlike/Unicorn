/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Texture.hpp>

#include <glm/gtc/constants.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>

namespace unicorn
{
namespace video
{
Mesh* Primitives::Box()
{
    Mesh* mesh = new Mesh;

    std::vector<Vertex> vertices{{
        //front
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},

        //right
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},

        //back
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},

        //left
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},

        //upper
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},

        //bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
    }};

    mesh->SetMeshData(vertices, {
                         0, 1, 2, 0, 2, 3,
                         4, 5, 6, 4, 6, 7,
                         8, 9, 10, 8, 10, 11,
                         12, 13, 14, 12, 14, 15,
                         16, 17, 18, 16, 18, 19,
                         20, 21, 22, 20, 22, 23});

    return mesh;
}

Mesh* Primitives::Quad()
{
    Mesh* mesh = new Mesh;

    std::vector<Vertex> vertices{{
        {{-0.5f, -0.5f, 0.0f},{0.0f, 1.0f}} ,
        {{0.5f, -0.5f, 0.0f},{1.0f, 1.0f}} ,
        {{0.5f, 0.5f, 0.0f},{1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f},{0.0f, 0.0f}},
    }};

    mesh->SetMeshData(vertices, {0, 1, 2, 2, 3, 0});

    return mesh;
}

Mesh* Primitives::Sphere(float radius, uint32_t rings, uint32_t sectors)
{
    Mesh* mesh = new Mesh;

    assert(radius > 0);
    assert(rings > 4 || sectors > 4);

    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;

    uint32_t vectorSize = rings * sectors;

    assert(vectorSize < vertices.max_size());

    vertices.resize(vectorSize);

    {
        float const R = 1.f / static_cast<float>(rings - 1);
        float const S = 1.f / static_cast<float>(sectors - 1);

        auto vert_iter = vertices.begin();
        for(uint32_t r = 0; r < rings; ++r)
        {
            for(uint32_t s = 0; s < sectors; s++)
            {
                float const y = glm::sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
                float const x = glm::cos(2 * glm::pi<float>() * s * S) * glm::sin(glm::pi<float>() * r * R);
                float const z = glm::sin(2 * glm::pi<float>() * s * S) * glm::sin(glm::pi<float>() * r * R);
                *vert_iter++ = {{x * radius, y * radius, z * radius}, {(sectors - s) * S, (rings - r) * R}};
            }
        }
    }

    indices.resize(sectors * rings * 6);
    auto indicesIt = indices.begin();
    for(uint32_t x = 0; x < sectors; ++x)
    {
        uint32_t const right = (x + 1) % sectors;
        for(uint32_t y = 0; y < rings; ++y)
        {
            uint32_t bottom = (y + 1) % rings;
            *indicesIt++ = x + y * sectors;
            *indicesIt++ = x + bottom * sectors;
            *indicesIt++ = right + y * sectors;
            *indicesIt++ = right + y * sectors;
            *indicesIt++ = x + bottom * sectors;
            *indicesIt++ = right + bottom * sectors;
        }
    }

    mesh->SetMeshData(vertices, indices);

    return mesh;
}

Mesh* ProcessMesh(aiMesh* mesh, aiScene const* scene, std::string const& dir);

void ProcessNode(aiNode* node, aiScene const* scene, std::string const& dir, std::list<Mesh*>& meshes)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene, dir));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, dir, meshes);
    }
}

std::vector<std::string> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string directory)
{
    std::vector<std::string> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        std::replace(path.begin(), path.end(), '\\', '/');
        textures.emplace_back(directory + "/" + path);
    }
    return textures;
}

Mesh* ProcessMesh(aiMesh* mesh, aiScene const* scene, std::string const& dir)
{
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tc = vec;
        }
        else
        {
            vertex.tc = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* unicornMesh = new Mesh;

    unicornMesh->name = mesh->mName.C_Str();

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        unicornMesh->m_diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, dir);
        unicornMesh->m_normal = LoadMaterialTextures(material, aiTextureType_NORMALS, dir);
        unicornMesh->m_aoMaps = LoadMaterialTextures(material, aiTextureType_LIGHTMAP, dir);
        unicornMesh->m_emissive = LoadMaterialTextures(material, aiTextureType_EMISSIVE, dir);
        unicornMesh->m_metalRougness = LoadMaterialTextures(material, aiTextureType_UNKNOWN, dir);
    }

    auto mat = std::make_shared<Material>();

    if (unicornMesh->m_diffuse.size() > 0)
    {
        auto diffuseTex = std::make_shared<Texture>();
        diffuseTex->Load(unicornMesh->m_diffuse.at(0));
        mat->SetAlbedo(diffuseTex);
        unicornMesh->SetMaterial(mat);
    }

    unicornMesh->SetMeshData(vertices, indices);

    return unicornMesh;
}

std::list<Mesh*> Primitives::LoadModel(std::string const& path)
{
    std::list<Mesh*> meshes;

    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ERROR importing %s mesh : %s", path.c_str(),
            importer.GetErrorString());
        return meshes;
    }

    std::string dir = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene, dir, meshes);

    return meshes;
}

}
}
