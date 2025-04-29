// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Plane.hpp"
#include <vector>
using namespace std;

namespace udit
{
    // Constructor: Inicializa un plano con la cuadrícula especificada y genera la geometría.
    Plane::Plane(int subdivisionsX, int subdivisionsY, float width, float height) : subdivX(subdivisionsX), subdivY(subdivisionsY), grid_width(width), grid_height(height), Mesh()
    {
        generateBuffers();
        generateGeometry();
    }

    // Destructor: Libera los recursos de OpenGL utilizados para el plano.
    Plane::~Plane()
    {
        deleteBuffers();
    }

    // Genera la geometría del plano, incluyendo vértices, coordenadas de textura e índices.
    void Plane::generateGeometry()
    {
        int vertexCount = (subdivX + 1) * (subdivY + 1);
        int indexCount = subdivX * subdivY * 6;

        coordinates.resize(vertexCount * 3);
        texCoords.resize(vertexCount * 2);
        normals.resize(vertexCount * 3);
        indices.resize(indexCount);

        int vertexIndex = 0;
        int texCoordIndex = 0;
        int normalIndex = 0;

        // Espaciado entre vértices
        float dx = grid_width / subdivX;
        float dy = grid_height / subdivY;

        // Centrar el plano alrededor del origen
        float originX = -grid_width / 2.0f;
        float originY = -grid_height / 2.0f;

        for (int y = 0; y <= subdivY; ++y)
        {
            for (int x = 0; x <= subdivX; ++x)
            {
                float posX = originX + x * dx;
                float posY = originY + y * dy;

                coordinates[vertexIndex++] = posX;
                coordinates[vertexIndex++] = 0.0f;
                coordinates[vertexIndex++] = posY;

                texCoords[texCoordIndex++] = static_cast<GLfloat>(x) / subdivX;
                texCoords[texCoordIndex++] = static_cast<GLfloat>(y) / subdivY;

                normals[normalIndex++] = 0.0f;
                normals[normalIndex++] = 1.0f; // Apuntando hacia arriba (Y)
                normals[normalIndex++] = 0.0f;
            }
        }

        int index = 0;
        for (int y = 0; y < subdivY; ++y)
        {
            for (int x = 0; x < subdivX; ++x)
            {
                int topLeft = y * (subdivX + 1) + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * (subdivX + 1) + x;
                int bottomRight = bottomLeft + 1;

                indices[index++] = topLeft;
                indices[index++] = bottomLeft;
                indices[index++] = bottomRight;
                indices[index++] = topLeft;
                indices[index++] = bottomRight;
                indices[index++] = topRight;
            }
        }

        Mesh::generateBuffers();
    }
}