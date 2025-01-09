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
         * @param width Número de columnas en la cuadrícula.
         * @param height Número de filas en la cuadrícula.
         */
        Plane(int width, int height); // Constructor con parámetros

        /**
         * @brief Destructor de la clase Plane.
         */
        ~Plane(); // Destructor

        void render(); // Método de renderización

    private:
        GLuint vao_id;
        GLuint vbo_ids[3];

        int grid_width;  /**< Ancho de la cuadrícula. */
        int grid_height; /**< Alto de la cuadrícula. */

        /**
         * @brief Constantes para la cantidad de VBOs y los índices de los buffers.
         */
        static const int VBO_COUNT = 3; /**< Número total de VBOs. */
        static const int COORDINATES_VBO = 0; /**< Índice del VBO de coordenadas. */
        static const int COLORS_VBO = 1; /**< Índice del VBO de colores. */
        static const int INDICES_EBO = 2; /**< Índice del VBO de índices (EBO). */

        static GLfloat* coordinates;
        static GLfloat* colors;
        static GLubyte* indices;

        void generateGeometry();
    };

}
