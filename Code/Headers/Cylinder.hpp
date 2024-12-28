// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once
#include <glad/glad.h>
#include <numbers>
#include <cmath>
#include <cstdlib>

namespace udit
{
    /**
     * @class Cylinder
     * @brief Representa un cilindro en OpenGL.
     *
     * Esta clase maneja la creación y renderización de un cilindro. Utiliza VBOs y VAO para
     * almacenar los datos de los vértices, colores e índices.
     */
    class Cylinder
    {
    public:
        /**
         * @brief Constructor de la clase Cylinder.
         *
         * Inicializa los recursos de OpenGL y genera la geometría del cilindro.
         */
        Cylinder(int stack, int slice, GLfloat r, GLfloat h);

        /**
         * @brief Destructor de la clase Cylinder.
         *
         * Libera los recursos de memoria dinámica y de OpenGL utilizados por la clase.
         */
        ~Cylinder();

        /**
         * @brief Renderiza el cilindro.
         *
         * Dibuja el cilindro utilizando OpenGL.
         */
        void render();

    private:
        /**
         * @brief Parámetros del cilindro.
         *
         * Contienen las variables para dividir el cilindro en pilas (stack_count) y cortes
         * (slice_count), el radio (radius) y la altura (height).
         */
        int stack_count; /**< Número de divisiones en altura. */
        int slice_count; /**< Número de divisiones en circunferencia. */
        GLfloat radius; /**< Radio del cilindro. */
        GLfloat height; /**< Altura del cilindro. */

        /**
         * @brief Datos estáticos para la geometría.
         *
         * Arrays que contienen las coordenadas de los vértices, colores e índices para el
         * dibujo del cilindro.
         */
        static GLfloat* coordinates; /**< Coordenadas de los vértices del cilindro. */
        static GLfloat* colors; /**< Colores de los vértices del cilindro. */
        static GLubyte* indices; /**< Índices de los vértices para formar triángulos. */

        /**
         * @brief Identificadores de OpenGL.
         *
         * Contienen el identificador del Vertex Array Object (VAO) y los buffers de datos
         * (VBOs).
         */
        GLuint vao_id; /**< Identificador del Vertex Array Object. */
        GLuint vbo_ids[3]; /**< Identificadores para los buffers de coordenadas, colores e índices. */

        /**
         * @enum BufferIndices
         * @brief Índices para los buffers.
         *
         * Enumeración para identificar los buffers de coordenadas, colores e índices.
         */
        enum { COORDINATES_VBO, COLORS_VBO, INDICES_EBO, VBO_COUNT };

        /**
         * @brief Genera la geometría del cilindro.
         *
         * Calcula las coordenadas, colores e índices del cilindro y configura los buffers.
         */
        void generateGeometry();
    };
}
