// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Cylinder.hpp"

namespace udit
{
    Cylinder::Cylinder(int stack, int slice, GLfloat r, GLfloat h)
        : stack_count(stack), slice_count(slice), radius(r), height(h)
    {
        generateGeometry();
        generateBuffers();
    }

    Cylinder::~Cylinder()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cylinder::generateGeometry()
    {
        int vertexCount = stack_count * slice_count + 2;
        int indexCount = stack_count * slice_count * 6 + slice_count * 6;

        coordinates.resize(vertexCount * 3);
        texCoords.resize(vertexCount * 2);
        normals.resize(vertexCount * 3);
        indices.resize(indexCount);

        int vertexIndex = 0;
        int texCoordIndex = 0;
        int normalIndex = 0;

        for (int stack = 0; stack < stack_count; ++stack)
        {
            GLfloat y = -height / 2 + stack * (height / (stack_count - 1));
            for (int slice = 0; slice < slice_count; ++slice)
            {
                GLfloat angle = static_cast<GLfloat>(2.0f * std::numbers::pi * slice / slice_count);
                GLfloat x = radius * cos(angle);
                GLfloat z = radius * sin(angle);

                coordinates[vertexIndex++] = x;
                coordinates[vertexIndex++] = y;
                coordinates[vertexIndex++] = z;

                texCoords[texCoordIndex++] = static_cast<GLfloat>(stack) / stack_count;
                texCoords[texCoordIndex++] = static_cast<GLfloat>(slice) / slice_count;

                normals[normalIndex++] = x / radius;
                normals[normalIndex++] = 0.0f;
                normals[normalIndex++] = z / radius;
            }
        }

        // Vértices del centro de la base y la cima
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = -height / 2;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 0.0f;

        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = -1.0f;
        normals[normalIndex++] = 0.0f;

        int bottomCenterIndex = (vertexIndex / 3) - 1;

        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height / 2;
        coordinates[vertexIndex++] = 0.0f;

        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 1.0f;

        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = 1.0f;
        normals[normalIndex++] = 0.0f;

        int topCenterIndex = (vertexIndex / 3) - 1;

        int index = 0;
        // Genera los índices para las caras laterales del cilindro
        for (int stack = 0; stack < stack_count - 1; ++stack)
        {
            for (int slice = 0; slice < slice_count; ++slice)
            {
                int nextSlice = (slice + 1) % slice_count;
                int current = stack * slice_count + slice;
                int next = stack * slice_count + nextSlice;
                int above = (stack + 1) * slice_count + slice;
                int aboveNext = (stack + 1) * slice_count + nextSlice;

                indices[index++] = current;
                indices[index++] = above;
                indices[index++] = next;

                indices[index++] = next;
                indices[index++] = above;
                indices[index++] = aboveNext;
            }
        }

        // Genera los índices para la base del cilindro
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = bottomCenterIndex;
            indices[index++] = slice;
            indices[index++] = nextSlice;
        }

        // Genera los índices para la tapa superior del cilindro
        int offset = (stack_count - 1) * slice_count;
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = topCenterIndex;
            indices[index++] = offset + nextSlice;
            indices[index++] = offset + slice;
        }
    }
}
