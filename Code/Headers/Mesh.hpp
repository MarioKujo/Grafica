#pragma once
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <numbers>
#include "GeometryGenerator.hpp"
#include "ShaderProgram.hpp"
using namespace std;

namespace udit
{
    /**
     * @brief Clase que representa una malla 3D y gestiona sus buffers OpenGL.
     *
     * Esta clase encapsula los datos de la malla (vértices, normales, texturas, índices)
     * y proporciona funciones para crear y liberar buffers GPU, así como para renderizar.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructor por defecto.
         * Inicializa la malla vacía y genera los buffers OpenGL.
         */
        Mesh();

        /**
         * @brief Constructor con datos de malla.
         *
         * @param receivedData Datos de la malla (vértices, normales, texturas, índices).
         * Crea la malla y genera los buffers en GPU con esos datos.
         */
        Mesh(MeshData receivedData);

        /**
         * @brief Renderiza la malla usando los datos almacenados y buffers GPU.
         *
         * Si la malla no está inicializada, no hace nada.
         * Usa `glDrawElements` con modo GL_TRIANGLES.
         */
        void render();

        /**
         * @brief Genera los buffers OpenGL (VBOs y VAO) para la malla actual.
         *
         * Carga los datos (vértices, normales, texturas e índices) a la GPU.
         * Marca la malla como inicializada para renderizado posterior.
         */
        void generateBuffers();

        /**
        * @brief Carga datos en un buffer de OpenGL.
        *
        * @param bufferId Identificador del buffer a configurar.
        * @param target Tipo de buffer (por ejemplo, GL_ARRAY_BUFFER o GL_ELEMENT_ARRAY_BUFFER).
        * @param dataPtr Puntero a los datos a subir al buffer.
        * @param dataSize Tamaño en bytes de los datos.
        */
        void uploadBuffer(GLuint bufferId, GLenum target, const void* dataPtr, size_t dataSize);

        /**
         * @brief Elimina los buffers OpenGL asociados a esta malla.
         *
         * Libera memoria GPU y marca la malla como no inicializada.
         */
        void deleteBuffers();

    protected:

        /// Índices para los diferentes buffers usados
        enum { COORDINATES_VBO, TEXCOORDS_VBO, NORMALS_VBO, INDICES_EBO, VBO_COUNT };

        /// Indica si los buffers están inicializados y listos para renderizar
        bool isInitialized;

        /// Identificador del Vertex Array Object
        GLuint vao_id;

        /// Identificadores de Vertex Buffer Objects y Element Buffer Object
        GLuint vbo_ids[VBO_COUNT];

        /// Datos de la malla: coordenadas, normales, texturas e índices
        MeshData data;
    };
}
