#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

class Model
{
private:
    std::vector<Mesh> _meshes;

    static std::filesystem::path directory;
    std::ofstream model_log;

    std::map<std::string, Texture> _diffuse_dict;
    std::map<std::string, Texture> _specular_dict;

    std::map<size_t, Texture*> _diffuse_mapping;
    std::map<size_t, Texture*> _specular_mapping;

    glm::mat4 convertMatrix(aiMatrix4x4 mat)
    {
        return glm::mat4(
            mat.a1, mat.a2, mat.a3, mat.a4,
            mat.b1, mat.b2, mat.b3, mat.b4,
            mat.c1, mat.c2, mat.c3, mat.c4,
            mat.d1, mat.d2, mat.d3, mat.d4
        );
    }

    void process_node(aiNode* node, const aiScene* scene, const glm::mat4& trans = glm::mat4(1.f));
    void process_mesh(aiMesh* mesh, const aiScene* scene,  const glm::mat4& trans);
public:
    glm::mat4 _trans = glm::mat4(1.f);

    Model(
        const std::filesystem::path &path,
        bool flipUV=false,
        bool standart_dir=true
    );

    void draw(
        const Camera& camera,
        const Shader& shader,
        float aspect_ratio = 1.f
    )
    {
        shader.use();

        shader.setMat4("model", _trans);
        shader.setVec3("viewPos", camera._pos);
        shader.setMat4("view", camera.get_view());
        shader.setMat4("projection", camera.get_proj(aspect_ratio));
        shader.setInt("specular_text", 1);
        shader.setInt("diffuse_text", 0);
        for (size_t i = 0; i < _meshes.size(); ++i)
        {
            if(_diffuse_mapping.count(i))
            {
                glActiveTexture(GL_TEXTURE0);
                _diffuse_mapping[i]->bind();
            }
            if(_specular_mapping.count(i))
            {
                glActiveTexture(GL_TEXTURE1);
                _specular_mapping[i]->bind();
            }

            _meshes[i].draw();

            if(_diffuse_mapping.count(i))
                _diffuse_mapping[i]->unbind();
            if(_specular_mapping.count(i))
                _specular_mapping[i]->unbind();
        }
    }

    ~Model() = default;
};

Model::Model(const std::filesystem::path& path, bool flipUV, bool standart_dir)
{
    Assimp::Importer import;

    auto postprocess = aiProcess_Triangulate | 
    aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenNormals;
    postprocess |= flipUV ? aiProcess_FlipUVs : 0;

    const aiScene *scene = import.ReadFile(directory / path, postprocess);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        throw std::runtime_error(std::string("ERROR::ASSIMP::") + import.GetErrorString() + '\n');
    }
    model_log.open("model.log", std::ios_base::trunc);

    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        auto material = scene->mMaterials[i];
        aiString str;
        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string Str(str.C_Str());
            if (_diffuse_dict.count(Str) == 0)
                _diffuse_dict[Str] = Texture(directory / path.parent_path() / Str, false, false);
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR))
        {
            material->GetTexture(aiTextureType_SPECULAR, 0, &str);
            std::string Str(str.C_Str());
            if (_specular_dict.count(Str) == 0)
                _specular_dict[Str] = Texture(directory / path.parent_path() / Str, false, false);
        }
    }

    auto* rootNode = scene->mRootNode;
    process_node(rootNode, scene, convertMatrix(rootNode->mTransformation));

    model_log.close();
}

void Model::process_node(aiNode* node, const aiScene* scene, const glm::mat4& trans)
{
    for (size_t meshNum = 0; meshNum < node->mNumMeshes; ++meshNum)
        process_mesh(scene->mMeshes[node->mMeshes[meshNum]], scene, trans);
    
    for(size_t childNum = 0; childNum < node->mNumChildren; ++childNum)
        process_node( node->mChildren[childNum], scene, glm::mat4(1.f));
}

void Model::process_mesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& trans)
{
    model_log << "Mesh\n";

    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        
        Vertex vertex;

        model_log << "pos: " << i;
        model_log << " x: " << (vertex._pos.x = mesh->mVertices[i].x);
        model_log << " y: " << (vertex._pos.y = mesh->mVertices[i].y);
        model_log << " z: " << (vertex._pos.z = mesh->mVertices[i].z);
        model_log << '\n';

        if (mesh->HasNormals())
        {
            vertex._nor.x = mesh->mNormals[i].x;
            vertex._nor.y = mesh->mNormals[i].y;
            vertex._nor.z = mesh->mNormals[i].z;
        }

        //checks if mesh has texture coords
        if(mesh->mTextureCoords[0])
        {
           vertex._tex.x = mesh->mTextureCoords[0][i].x;
           vertex._tex.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }


    std::vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces * 3);
    
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);

        model_log << indices[i+0] << " "; 
        model_log << indices[i+1] << " "; 
        model_log << indices[i+2] << " ";
        model_log << '\n';
    }

    if (mesh->mMaterialIndex >= 0)
    {
        auto material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;
        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string Str(str.C_Str());
            _diffuse_mapping[_meshes.size()] = &_diffuse_dict[Str];
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR))
        {
            material->GetTexture(aiTextureType_SPECULAR, 0, &str);
            std::string Str(str.C_Str());
            _specular_mapping[_meshes.size()] = &_specular_dict[Str];
        }
    }

    _meshes.push_back(Mesh(vertices, indices));
    _meshes.back()._trans = trans;
}

std::filesystem::path Model::directory = "../resources/models/";

#endif