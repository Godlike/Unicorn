/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Primitives.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/utility/Math.hpp>

#include <mule/Logger.hpp>

#include <glm/gtc/constants.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>

namespace unicorn
{
namespace video
{
Mesh& Primitives::Box(Mesh& mesh)
{
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

    mesh.SetMeshData(vertices, {
                         0, 1, 2, 0, 2, 3,
                         4, 5, 6, 4, 6, 7,
                         8, 9, 10, 8, 10, 11,
                         12, 13, 14, 12, 14, 15,
                         16, 17, 18, 16, 18, 19,
                         20, 21, 22, 20, 22, 23});

    return mesh;
}

Mesh& Primitives::Quad(Mesh& mesh)
{
    std::vector<Vertex> vertices{{
        {{-0.5f, -0.5f, 0.0f},{0.0f, 1.0f}} ,
        {{0.5f, -0.5f, 0.0f},{1.0f, 1.0f}} ,
        {{0.5f, 0.5f, 0.0f},{1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f},{0.0f, 0.0f}},
    }};

    mesh.SetMeshData(vertices, {0, 1, 2, 2, 3, 0});

    return mesh;
}

Mesh& Primitives::Sphere(Mesh& mesh, float radius, uint32_t rings, uint32_t sectors)
{
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

    mesh.SetMeshData(vertices, indices);

    return mesh;
}

namespace
{

/**
 * @brief Reads mesh from assimp and creates unicorn::Mesh.
 * @param [in] mesh pointer to assimp mesh
 * @param [in] scene assimp hierarhy scene
 * @param [in] dir directory, where mesh is locating
 *
 * @return created unicorn::Mesh
 */
Mesh* ProcessMesh(aiMesh const* mesh, aiScene const* scene, std::string const& dir);

/**
* @brief Reads each node and calls ProcessMesh for each aiMesh
* @param [in] root the root node in the scene tree
* @param [in] scene assimp hierarhy scene
* @param [in] dir directory, where mesh is locating
* @param [out] meshes fills list with create meshes
*/
void ProcessNodes(aiNode const* root, aiScene const* scene, std::string const& dir, std::list<Mesh*>& meshes)
{
    assert(nullptr != root);
    assert(nullptr != scene);

    glm::mat4 matrix = utility::math::AssimpMatrixToGlm(root->mTransformation);
    std::vector<std::pair<aiNode const*, glm::mat4>> stack{ { root, matrix } };

    while (!stack.empty())
    {
        auto const frame = stack.back();
        stack.pop_back();

        for (uint32_t i = 0; i < frame.first->mNumMeshes; ++i)
        {
            aiMesh const* mesh = scene->mMeshes[frame.first->mMeshes[i]];
            auto unicornMesh = ProcessMesh(mesh, scene, dir);

            unicornMesh->TransformByMatrix(frame.second);
            unicornMesh->UpdateTransformMatrix();

            meshes.push_back(unicornMesh);
        }

        for (uint32_t i = 0; i < frame.first->mNumChildren; ++i)
        {
            matrix = utility::math::AssimpMatrixToGlm(frame.first->mChildren[i]->mTransformation) * frame.second;
            stack.emplace_back( frame.first->mChildren[i], matrix );
        }
    }
}

/**
* @brief Reads from assimp materials and returns path to texture with given type
* @param [in] mat assimp material, which holds all visual appearance info
* @param [in] type texture visual type (albedo, normal map, ao map and so on)
* @param [in] directory directory, where mesh is locating
*
* @return vector of paths
*/
std::vector<std::string> LoadMaterialTextures(aiMaterial const* mat, aiTextureType type, std::string const& directory)
{
    assert(nullptr != mat);

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

Mesh* ProcessMesh(aiMesh const* mesh, aiScene const* scene, std::string const& dir)
{
    assert(nullptr != mesh);
    assert(nullptr != scene);

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

    auto mat = std::make_shared<Material>();

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuseTexture = LoadMaterialTextures(material, aiTextureType_DIFFUSE, dir);

    aiColor3D color(0.f, 0.f, 0.f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mat->SetColor({ color.r, color.g, color.b });

    if (!diffuseTexture.empty())
    {
        auto diffuseTex = std::make_shared<Texture>();
        diffuseTex->Load(diffuseTexture.at(0));
        mat->SetAlbedo(diffuseTex);
    }
    unicornMesh->SetMaterial(mat);

    unicornMesh->SetMeshData(vertices, indices);

    return unicornMesh;
}
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

    std::string const dir = path.substr(0, path.find_last_of('/'));

    ProcessNodes(scene->mRootNode, scene, dir, meshes);

    return meshes;
}

}
}
