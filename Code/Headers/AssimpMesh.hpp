#pragma once
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace udit
{
    /**
     * @class AssimpMesh
     * @brief Clase que extiende Mesh para cargar modelos 3D usando Assimp.
     *
     * Esta clase carga un modelo desde un archivo usando Assimp y genera
     * los buffers necesarios para renderizar la primera malla encontrada.
     */
    class AssimpMesh : public Mesh
    {
    public:
        /**
         * @brief Constructor que carga un modelo desde un archivo.
         * @param filepath Ruta al archivo del modelo 3D.
         */
        AssimpMesh(const std::string& filepath);

    private:
        /**
         * @brief Carga el modelo desde disco usando Assimp.
         * @param path Ruta al archivo del modelo 3D.
         *
         * Lee la escena completa y procesa la primera malla que encuentre.
         */
        void loadModel(const std::string& path);

        /**
         * @brief Procesa una malla de Assimp extrayendo vértices, normales, UVs e índices.
         * @param mesh Puntero a la malla de Assimp a procesar.
         */
        void processMesh(aiMesh* mesh);

        /**
         * @brief Procesa los vértices de la malla, incluyendo coordenadas, normales y UVs.
         * @param mesh Puntero a la malla de Assimp.
         */
        void processVertices(aiMesh* mesh);

        /**
         * @brief Procesa los índices de la malla para formar los triángulos.
         * @param mesh Puntero a la malla de Assimp.
         */
        void processIndices(aiMesh* mesh);
    };
}
