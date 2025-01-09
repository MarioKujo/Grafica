#include "../Headers/Plane.hpp"
#include <vector>
using namespace std;

namespace udit
{
    Plane::Plane(int width, int height) : grid_width(width), grid_height(height)
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Plane::~Plane()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Plane::generateGeometry()
    {
        int vertexCount = grid_width * grid_height;
        int indexCount = (grid_width - 1) * (grid_height - 1) * 6;

        coordinates.resize(vertexCount * 3, 0.0f);
        colors.resize(vertexCount * 3, 0.0f);
        indices.resize(indexCount, 0);

        int vertexIndex = 0;
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                GLfloat posX = x * 2.0f - (grid_width - 1);
                GLfloat posY = y * 2.0f - (grid_height - 1);

                coordinates[vertexIndex++] = posX;
                coordinates[vertexIndex++] = posY;
                coordinates[vertexIndex++] = 0.0f;
            }
        }

        vertexIndex = 0;
        for (int i = 0; i < vertexCount; ++i)
        {
            colors[vertexIndex++] = 0.0f;
            colors[vertexIndex++] = 1.0f;
            colors[vertexIndex++] = 0.0f;
        }

        int index = 0;
        for (int y = 0; y < grid_height - 1; ++y)
        {
            for (int x = 0; x < grid_width - 1; ++x)
            {
                int topLeft = y * grid_width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * grid_width + x;
                int bottomRight = bottomLeft + 1;

                indices[index++] = topLeft;
                indices[index++] = bottomLeft;
                indices[index++] = bottomRight;
                indices[index++] = topLeft;
                indices[index++] = bottomRight;
                indices[index++] = topRight;
            }
        }
    }

    void Plane::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
