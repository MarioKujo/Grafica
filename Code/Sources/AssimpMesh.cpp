#include "../Headers/AssimpMesh.hpp"
#include <iostream>

namespace udit
{
    AssimpMesh::AssimpMesh(const std::string& filepath)
    {
        loadModel(filepath);
        generateBuffers(); // Llama al método heredado para subir a GPU
    }

    void AssimpMesh::loadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        // Solo cargamos la primera malla
        if (scene->mNumMeshes > 0)
        {
            processMesh(scene->mMeshes[0]);
        }
    }

    void AssimpMesh::processMesh(aiMesh* mesh)
    {
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            // Coordenadas
            coordinates.push_back(mesh->mVertices[i].x);
            coordinates.push_back(mesh->mVertices[i].y);
            coordinates.push_back(mesh->mVertices[i].z);

            // Normales
            if (mesh->HasNormals())
            {
                normals.push_back(mesh->mNormals[i].x);
                normals.push_back(mesh->mNormals[i].y);
                normals.push_back(mesh->mNormals[i].z);
            }

            // UVs
            if (mesh->mTextureCoords[0]) // Solo consideramos el primer set de UVs
            {
                texCoords.push_back(mesh->mTextureCoords[0][i].x);
                texCoords.push_back(mesh->mTextureCoords[0][i].y);
            }
            else
            {
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);
            }
        }

        // Índices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(static_cast<GLuint>(face.mIndices[j]));
            }
        }
    }
}
