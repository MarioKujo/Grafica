#pragma once
#include <glad/glad.h> // Incluye GLAD
#include <numbers>
#include <cmath>
#include <cstdlib>
namespace udit
{
    class Cylinder
    {
    public:
        Cylinder();
        ~Cylinder();

        void render();

    private:
        static const int STACK_COUNT; // Número de divisiones en altura
        static const int SLICE_COUNT; // Número de divisiones en circunferencia
        static const GLfloat RADIUS;
        static const GLfloat HEIGHT;

        static GLfloat* coordinates;
        static GLfloat* colors;
        static GLubyte* indices;

        GLuint vao_id;
        GLuint vbo_ids[3]; // COORDINATES_VBO, COLORS_VBO, INDICES_EBO

        enum { COORDINATES_VBO, COLORS_VBO, INDICES_EBO, VBO_COUNT };

        void generateGeometry();
    };
}
