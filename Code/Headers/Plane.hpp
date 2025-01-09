#pragma once
#include <glad/glad.h>
#include <vector>
using namespace std;

namespace udit
{
    class Plane
    {
    public:
        Plane(int width, int height);
        ~Plane();
        void render();

    private:
        GLuint vao_id;
        GLuint vbo_ids[3];

        int grid_width;
        int grid_height;
        enum { COORDINATES_VBO, COLORS_VBO, INDICES_EBO, VBO_COUNT };

        vector<GLfloat> coordinates;
        vector<GLfloat> colors;
        vector<GLubyte> indices;

        void generateGeometry();
    };
}