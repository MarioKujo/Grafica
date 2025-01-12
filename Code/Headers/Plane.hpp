/*@file Plane.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Declaración de la clase Plane para representar un plano 3D.
 *
 * Esta clase genera un plano de malla utilizando OpenGL, donde el plano tiene una
 * cuadrícula definida por su ancho y alto. Se utiliza para representar superficies planas.
 */

#pragma once
#include <glad/glad.h>
#include <vector>
using namespace std;

namespace udit
{
    /**
     * @class Plane
     * @brief Clase para representar un plano 3D.
     *
     * Esta clase genera un plano de malla utilizando OpenGL. El plano tiene una
     * cuadrícula definida por su ancho y alto, y se utiliza para representar superficies planas.
     */
    class Plane
    {
    public:
        /**
         * @brief Constructor de la clase Plane.
         *
         * Inicializa un plano con la cuadrícula definida por el ancho y alto proporcionados.
         * Genera la geometría del plano (vértices, coordenadas de textura e índices).
         *
         * @param width El ancho del plano, define la cantidad de columnas de la cuadrícula.
         * @param height La altura del plano, define la cantidad de filas de la cuadrícula.
         */
        Plane(int width, int height);

        /**
         * @brief Destructor de la clase Plane.
         *
         * Libera los recursos de OpenGL utilizados para almacenar la geometría del plano.
         */
        ~Plane();

        /**
         * @brief Renderiza el plano.
         *
         * Dibuja el plano utilizando los datos de geometría generados con OpenGL.
         */
        void render();

    private:
        GLuint vao_id;               ///< Identificador del Vertex Array Object (VAO).
        GLuint vbo_ids[3];           ///< Identificadores de los Vertex Buffer Objects (VBOs) para las coordenadas, coordenadas de textura e índices.

        int grid_width;              ///< Número de columnas en la cuadrícula.
        int grid_height;             ///< Número de filas en la cuadrícula.

        enum { COORDINATES_VBO, TEXCOORDS_VBO, INDICES_EBO, VBO_COUNT }; ///< Índices para los VBOs y el EBO.

        vector<GLfloat> coordinates; ///< Vectores que contienen las coordenadas de los vértices del plano.
        vector<GLfloat> texCoords;   ///< Vectores que contienen las coordenadas de textura (UV) para los vértices.
        vector<GLubyte> indices;     ///< Vectores que contienen los índices para dibujar el plano con triángulos.

        /**
         * @brief Genera la geometría del plano.
         *
         * Calcula las posiciones de los vértices, las coordenadas de textura y los índices
         * que definen la malla del plano en función de las dimensiones de la cuadrícula.
         */
        void generateGeometry();
    };
}
