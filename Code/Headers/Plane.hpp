#pragma once
#ifndef PLANE_HPP
#define PLANE_HPP

#include <glad/glad.h>

namespace udit
{
    class Plane
    {
    public:
        Plane();  // Constructor
        ~Plane(); // Destructor

        void render(); // Método de renderización

    private:
        // Identificadores de los VBOs y VAO
        GLuint vao_id;
        GLuint vbo_ids[3];

        // Constantes para los VBOs
        static const int VBO_COUNT = 3;
        static const int COORDINATES_VBO = 0;
        static const int COLORS_VBO = 1;
        static const int INDICES_EBO = 2;

        // Datos estáticos: coordenadas, colores e índices
        static const GLfloat coordinates[];
        static const GLfloat colors[];
        static const GLubyte indices[];
    };
}

#endif // PLANE_HPP
