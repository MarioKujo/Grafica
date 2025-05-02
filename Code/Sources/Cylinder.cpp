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
        deleteBuffers();
    }

    // Función auxiliar para calcular la posición de un vértice
    glm::vec3 Cylinder::calculateVertex(int stack, int slice)
    {
        GLfloat angle = static_cast<GLfloat>(2.0f * std::numbers::pi * slice / slice_count);
        GLfloat x = radius * cos(angle);
        GLfloat z = radius * sin(angle);
        GLfloat y = -height / 2 + stack * (height / (stack_count - 1));

        return glm::vec3(x, y, z);
    }

    // Función auxiliar para calcular las coordenadas de textura
    glm::vec2 Cylinder::calculateTexCoord(int stack, int slice)
    {
        return glm::vec2(static_cast<GLfloat>(stack) / stack_count, static_cast<GLfloat>(slice) / slice_count);
    }

    // Función auxiliar para calcular la normal
    glm::vec3 Cylinder::calculateNormal(GLfloat x, GLfloat z)
    {
        return glm::normalize(glm::vec3(x / radius, 0.0f, z / radius));
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
            for (int slice = 0; slice < slice_count; ++slice)
            {
                glm::vec3 vertex = calculateVertex(stack, slice);
                coordinates[vertexIndex++] = vertex.x;
                coordinates[vertexIndex++] = vertex.y;
                coordinates[vertexIndex++] = vertex.z;

                glm::vec2 texCoord = calculateTexCoord(stack, slice);
                texCoords[texCoordIndex++] = texCoord.x;
                texCoords[texCoordIndex++] = texCoord.y;

                glm::vec3 normal = calculateNormal(vertex.x, vertex.z);
                normals[normalIndex++] = normal.x;
                normals[normalIndex++] = normal.y;
                normals[normalIndex++] = normal.z;
            }
        }

        // Añadir los vértices de las tapas (base y cima)
        addTopAndBottomVertices(vertexIndex, texCoordIndex, normalIndex);
        generateIndices();
    }

    void Cylinder::addTopAndBottomVertices(int& vertexIndex, int& texCoordIndex, int& normalIndex)
    {
        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = -height / 2;
        coordinates[vertexIndex++] = 0.0f;
        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 0.0f;
        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = -1.0f;
        normals[normalIndex++] = 0.0f;

        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height / 2;
        coordinates[vertexIndex++] = 0.0f;
        texCoords[texCoordIndex++] = 0.5f;
        texCoords[texCoordIndex++] = 1.0f;
        normals[normalIndex++] = 0.0f;
        normals[normalIndex++] = 1.0f;
        normals[normalIndex++] = 0.0f;
    }

    void Cylinder::generateIndices()
    {
        int index = 0;
        int bottomCenterIndex = (stack_count * slice_count);
        int topCenterIndex = (stack_count * slice_count) + 1;

        // Generar los índices como en la versión original, pero con un método independiente.
        generateSideIndices(index);
        generateBaseIndices(index, bottomCenterIndex, topCenterIndex);
    }

    void Cylinder::generateSideIndices(int& index)
    {
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
    }

    void Cylinder::generateBaseIndices(int& index, int bottomCenterIndex, int topCenterIndex)
    {
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = bottomCenterIndex;
            indices[index++] = slice;
            indices[index++] = nextSlice;

            int offset = (stack_count - 1) * slice_count;
            indices[index++] = topCenterIndex;
            indices[index++] = offset + nextSlice;
            indices[index++] = offset + slice;
        }
    }
}
