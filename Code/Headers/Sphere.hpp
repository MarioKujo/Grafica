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
    class Sphere
    {
    public:
        Sphere(int latDivisions, int longDivisions, GLfloat radius);
        ~Sphere();
        void render();

    private:
        int latitudeDivisions;
        int longitudeDivisions;
        GLfloat radius;

        vector<GLfloat> coordinates;
        vector<GLfloat> texCoords;
        vector<GLubyte> indices;

        GLuint vao_id;
        GLuint vbo_ids[3];

        enum { COORDINATES_VBO, INDICES_EBO, TEXCOORDS_VBO, VBO_COUNT };


        void generateGeometry();
    };
}
