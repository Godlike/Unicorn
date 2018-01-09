/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Model.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/Material.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>
#include <unicorn/utility/Logger.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

namespace unicorn
{
namespace video
{

void Model::AddMesh(Mesh* mesh)
{
    m_meshes.push_back(mesh);
}

bool Model::LoadModel(std::string const& pathToModel)
{
    /*utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
    utility::asset::Handler modelHandler = storage.Get(pathToModel);

    if (!modelHandler.IsValid())
    {
        LOG_ERROR("Can't find model - %s", pathToModel.c_str());
        return false;
    }
*/
    Assimp::Importer importer;

    /*aiScene const* scene = importer.ReadFileFromMemory(modelHandler.GetContent().GetBuffer().data(),
        modelHandler.GetContent().GetBuffer().size(),
        aiProcess_Triangulate |
        aiProcess_FlipUVs);*/

    aiScene const* scene = importer.ReadFile(pathToModel,
        aiProcess_Triangulate |
        aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ERROR importing %s mesh : %s", pathToModel.c_str(),
            importer.GetErrorString());
        return false;
    }

    m_directory = pathToModel.substr(0, pathToModel.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

    return true;
}


void Model::ProcessNode(aiNode* node, aiScene const* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::vector<std::string> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string directory)
{
    std::vector<std::string> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        std::replace(path.begin(), path.end(), '\\', '/');
        textures.emplace_back(directory + "/" + path);
    }
    return textures;
}

Mesh* Model::ProcessMesh(aiMesh* mesh, aiScene const* scene)
{
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
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

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* unicornMesh = new Mesh;

    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        unicornMesh->m_diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, m_directory);
        unicornMesh->m_normal = LoadMaterialTextures(material, aiTextureType_NORMALS, m_directory);
        unicornMesh->m_aoMaps = LoadMaterialTextures(material, aiTextureType_LIGHTMAP, m_directory);
        unicornMesh->m_emissive = LoadMaterialTextures(material, aiTextureType_EMISSIVE, m_directory);
        unicornMesh->m_metalRougness = LoadMaterialTextures(material, aiTextureType_UNKNOWN, m_directory);
    }

    auto mat = std::make_shared<Material>();

    if(unicornMesh->m_diffuse.size() > 0)
    {
        Texture* diffuseTex = new Texture;
        diffuseTex->Load(unicornMesh->m_diffuse.at(0));
        mat->SetAlbedo(diffuseTex);
        unicornMesh->SetMaterial(mat);
    }

    unicornMesh->SetMeshData(vertices, indices);

    return unicornMesh;
}

} // namespace video
} // namespace unicorn
