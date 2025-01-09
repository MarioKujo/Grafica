// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <numbers>

using namespace std;

namespace udit
{
    class Cone
    {
    public:
        Cone(int d, GLfloat r, GLfloat h);
        ~Cone();
        void render();

    private:
        int divisions;
        GLfloat radius;
        GLfloat height;

        vector<GLfloat> coordinates;
        vector<GLfloat> colors;
        vector<GLfloat> texCoords;
        vector<GLubyte> indices;

        GLuint vao_id;
        GLuint vbo_ids[4];

        enum { COORDINATES_VBO, COLORS_VBO, INDICES_EBO, TEXCOORDS_VBO, VBO_COUNT };


        void generateGeometry();
    };
}