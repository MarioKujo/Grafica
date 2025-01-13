/**
 * @file TextureLoader.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este archivo contiene la declaración de la clase `TextureLoader`, que facilita la carga
 * de texturas en OpenGL, incluyendo tanto texturas 2D normales como cubemaps para efectos
 * de entorno.
 */

#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include "../Headers/stb_image.hpp"

using namespace std;
namespace udit
{

    /**
     * @class TextureLoader
     * @brief Clase para cargar texturas en OpenGL.
     *
     * La clase `TextureLoader` maneja la carga de texturas 2D y cubemaps, proporcionando métodos
     * para cargar imágenes desde archivos y generar las texturas correspondientes en OpenGL.
     */
    class TextureLoader {
    public:
        /**
         * @brief Constructor de la clase TextureLoader.
         *
         * Este constructor inicializa la clase. Actualmente no se utiliza para realizar
         * ninguna operación en particular.
         */
        TextureLoader();

        /**
         * @brief Destructor de la clase TextureLoader.
         *
         * El destructor limpia cualquier recurso utilizado por la clase (si es necesario).
         */
        ~TextureLoader();

        /**
         * @brief Carga una textura 2D desde un archivo.
         *
         * Este método carga una imagen desde un archivo y crea una textura 2D en OpenGL.
         *
         * @param filePath Ruta del archivo de imagen.
         * @return GLuint ID de la textura cargada.
         */
        GLuint loadTexture(const string& filePath);

        /**
         * @brief Carga un cubemap desde una lista de archivos de imagen.
         *
         * Este método carga una serie de imágenes que representan las caras de un cubemap.
         *
         * @param faces Lista de rutas de los archivos de imagen que representan las caras del cubemap.
         * @return GLuint ID del cubemap cargado.
         */
        GLuint loadCubemap(const vector<string>& faces);

    private:
        GLuint textureID; /**< ID de la textura cargada. */
    };
}
