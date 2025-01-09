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
        vector<GLfloat> colors;
        vector<GLubyte> indices;
        GLuint vao_id;
        GLuint vbo_ids[3];
        enum { COORDINATES_VBO, COLORS_VBO, INDICES_EBO, VBO_COUNT };
        void generateGeometry();
    };
}