#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

using namespace std;

namespace udit
{
    /**
     * @brief Clase para cargar texturas 2D y cubemaps en OpenGL.
     * 
     * Utiliza stb_image para leer imágenes desde disco y cargarlas como texturas
     * compatibles con OpenGL. Soporta tanto texturas 2D estándar como cubemaps.
     */
    class TextureLoader
    {
    public:
        /**
         * @brief Constructor por defecto.
         * 
         * Inicializa el identificador de textura a 0.
         */
        TextureLoader();

        /**
         * @brief Destructor.
         * 
         * Libera la textura si ha sido creada.
         */
        ~TextureLoader();

        /**
         * @brief Carga una textura 2D desde un archivo.
         * 
         * @param filePath Ruta al archivo de imagen.
         * @return GLuint Identificador de la textura generada en OpenGL. Devuelve 0 si hay error.
         */
        GLuint loadTexture(const string& filePath);

        /**
         * @brief Carga un cubemap desde 6 rutas de archivo (una por cada cara).
         * 
         * Las caras deben estar ordenadas según GL_TEXTURE_CUBE_MAP_POSITIVE_X + i.
         * 
         * @param faces Vector con las rutas a las imágenes de cada cara.
         * @return GLuint Identificador del cubemap generado en OpenGL. Devuelve 0 si hay error.
         */
        GLuint loadCubemap(const vector<string>& faces);

    private:
        GLuint textureID; /**< Identificador de la textura generada en OpenGL. */
    };
}
