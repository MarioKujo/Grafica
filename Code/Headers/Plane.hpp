// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once
#include <glad/glad.h>

namespace udit
{
    /**
     * @class Plane
     * @brief Clase que representa un plano en OpenGL.
     *
     * Esta clase se encarga de crear y renderizar un plano utilizando OpenGL. Utiliza VBOs
     * (Vertex Buffer Objects) y VAOs (Vertex Array Objects) para manejar los datos de los
     * vértices, colores e índices para la cuadrícula.
     */
    class Plane
    {
    public:
        /**
         * @brief Constructor de la clase Plane.
         *
         * Inicializa los identificadores de los VBOs y VAO y configura los datos estáticos
         * de geometría.
         */
        Plane();  // Constructor

        /**
         * @brief Destructor de la clase Plane.
         *
         * Libera los recursos de OpenGL (como los VBOs y VAO) cuando el objeto es destruido.
         */
        ~Plane(); // Destructor

        /**
         * @brief Método para renderizar el plano.
         *
         * Este método se encarga de hacer el dibujo del plano utilizando los buffers de OpenGL.
         */
        void render(); // Método de renderización

    private:
        /**
         * @brief Identificadores de los objetos de OpenGL.
         *
         * vao_id es el identificador del Vertex Array Object (VAO). vbo_ids es un array
         * que contiene los identificadores de los Vertex Buffer Objects (VBOs).
         */
        GLuint vao_id;
        GLuint vbo_ids[3];

        /**
         * @brief Constantes para la cantidad de VBOs y los índices de los buffers.
         */
        static const int VBO_COUNT = 3; /**< Número total de VBOs. */
        static const int COORDINATES_VBO = 0; /**< Índice del VBO de coordenadas. */
        static const int COLORS_VBO = 1; /**< Índice del VBO de colores. */
        static const int INDICES_EBO = 2; /**< Índice del VBO de índices (EBO). */

        /**
         * @brief Datos estáticos utilizados para la geometría del plano.
         *
         * Estos arrays contienen los datos de los vértices, colores e índices que definen
         * el plano.
         */
        static GLfloat* coordinates; /**< Coordenadas de los vértices. */
        static GLfloat* colors; /**< Colores de los vértices. */
        static GLubyte* indices; /**< Índices de los vértices para el EBO. */

        /**
         * @brief Parámetros de la cuadrícula del plano.
         *
         * Estos parámetros definen el tamaño de la cuadrícula que forma el plano.
         */
        static const int GRID_WIDTH = 6; /**< Ancho de la cuadrícula. */
        static const int GRID_HEIGHT = 4; /**< Alto de la cuadrícula. */

        /**
         * @brief Método privado para generar la geometría del plano.
         *
         * Este método configura los VBOs y VAO y llena los buffers con los datos de la
         * cuadrícula.
         */
        void generateGeometry();
    };
}
