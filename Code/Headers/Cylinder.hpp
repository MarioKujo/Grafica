// Este código es de dominio público 
// andrmatgonros@gmail.com
#pragma once
#include <glad/glad.h>
#include <numbers>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

namespace udit
{
    class Cylinder
    {
    public:
        Cylinder(int stack, int slice, GLfloat r, GLfloat h);
        ~Cylinder();
        void render();

    private:
        int stack_count;
        int slice_count;
        GLfloat radius;
        GLfloat height;
        vector<GLfloat> coordinates;
        vector<GLfloat> texCoords; // Añadido vector para coordenadas de textura
        vector<GLubyte> indices;
        GLuint vao_id;
        GLuint vbo_ids[4]; // Aumentado tamaño para incluir texCoords
        enum { COORDINATES_VBO, TEXCOORDS_VBO, INDICES_EBO, VBO_COUNT };
        void generateGeometry();
    };
}
