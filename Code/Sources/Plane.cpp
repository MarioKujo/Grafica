// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Plane.hpp"
#include <vector>
using namespace std;

namespace udit
{
    // Constructor: Inicializa un plano con la cuadrícula especificada y genera la geometría.
    Plane::Plane(int width, int height) : grid_width(width), grid_height(height), Mesh()
    {
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
        int vertexCount = grid_width * grid_height;
        int indexCount = (grid_width - 1) * (grid_height - 1) * 6;

        coordinates.resize(vertexCount * 3, 0.0f);
        texCoords.resize(vertexCount * 2, 0.0f);
        normals.resize(vertexCount * 3, 0.0f);
        indices.resize(indexCount, 0);

        int vertexIndex = 0;
        int texCoordIndex = 0;
        int normalIndex = 0;

        // Generar posiciones de vértices, coordenadas de textura y normales
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                GLfloat posX = x * 2.0f - (grid_width - 1);
                GLfloat posY = y * 2.0f - (grid_height - 1);

                coordinates[vertexIndex++] = posX;
                coordinates[vertexIndex++] = posY;
                coordinates[vertexIndex++] = 0.0f;

                texCoords[texCoordIndex++] = static_cast<GLfloat>(x) / (grid_width - 1);
                texCoords[texCoordIndex++] = static_cast<GLfloat>(y) / (grid_height - 1);

                // En un plano, todas las normales son [0, 0, 1]
                normals[normalIndex++] = 0.0f;
                normals[normalIndex++] = 0.0f;
                normals[normalIndex++] = 1.0f;
            }
        }

        // Generar índices de los triángulos
        int index = 0;
        for (int y = 0; y < grid_height - 1; ++y)
        {
            for (int x = 0; x < grid_width - 1; ++x)
            {
                int topLeft = y * grid_width + x;
                int topRight = topLeft + 1;
                int bottomLeft = (y + 1) * grid_width + x;
                int bottomRight = bottomLeft + 1;

                // Añadir los índices para dos triángulos por celda
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