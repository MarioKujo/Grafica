#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include <stb_image.h>
#include "Mesh.hpp"
#include "TextureLoader.hpp"

namespace udit
{
    /**
     * @class Skybox
     * @brief Clase que representa un skybox para el entorno 3D.
     *
     * Maneja la carga, asignación y renderizado de la textura del skybox.
     */
    class Skybox
    {
    public:

        /**
         * @brief Constructor que inicializa el skybox con los datos de la malla y el shader.
         * @param mesh Datos de la malla que representa el skybox.
         * @param shader Puntero al shader que se usará para renderizar el skybox.
         */
        Skybox(MeshData mesh, ShaderProgram* shader);

        /**
         * @brief Destructor que libera la textura del skybox.
         */
        ~Skybox();

        /**
         * @brief Establece la textura del skybox.
         * @param textureID Identificador de la textura de cubemap.
         */
        void setTexture(GLuint textureID);

        /**
         * @brief Obtiene el identificador de la textura del skybox.
         * @return El ID de la textura.
         */
        GLuint getTextureID();

        /**
         * @brief Renderiza el skybox usando las matrices de vista y proyección proporcionadas.
         * @param view Matriz de vista.
         * @param projection Matriz de proyección.
         */
        void render(const glm::mat4& view, const glm::mat4& projection);

    private:
        Mesh mesh;                  ///< Malla que representa el skybox.
        ShaderProgram* shader;      ///< Shader usado para renderizar el skybox.

        GLuint textureID;           ///< Identificador de la textura del skybox.
    };
}
