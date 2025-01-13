// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Sphere.hpp"

namespace udit
{
    Sphere::Sphere(int latDivisions, int longDivisions, GLfloat radius)
        : latitudeDivisions(latDivisions), longitudeDivisions(longDivisions), radius(radius)
    {
        // Genera la geometría de la esfera
        generateGeometry();

        // Genera los buffers de OpenGL
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        // Configura el buffer de coordenadas de vértices
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Configura el buffer de coordenadas de textura
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Configura el buffer de índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Sphere::~Sphere()
    {
        // Limpia los recursos de OpenGL
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Sphere::generateGeometry()
    {
        int indexCount = latitudeDivisions * longitudeDivisions * 6;

        // Redimensiona los vectores para las coordenadas de vértices, coordenadas de textura e índices
        coordinates.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 3);
        texCoords.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 2);
        indices.resize(indexCount);

        int index = 0;
        int coordIndex = 0;

        // Calcula los vértices y coordenadas de textura
        for (int lat = 0; lat <= latitudeDivisions; ++lat)
        {
            for (int lon = 0; lon <= longitudeDivisions; ++lon)
            {
                GLfloat theta = static_cast<GLfloat>((static_cast<double>(lat) / latitudeDivisions) * std::numbers::pi);
                GLfloat phi = static_cast<GLfloat>((static_cast<double>(lon) / longitudeDivisions) * 2.0f * std::numbers::pi);

                // Calcula las coordenadas esféricas
                GLfloat x = radius * sin(theta) * cos(phi);
                GLfloat y = radius * cos(theta);
                GLfloat z = radius * sin(theta) * sin(phi);

                // Asigna las coordenadas de los vértices
                coordinates[coordIndex++] = x;
                coordinates[coordIndex++] = y;
                coordinates[coordIndex++] = z;

                // Asigna las coordenadas de textura
                texCoords[(lat * (longitudeDivisions + 1) + lon) * 2] = static_cast<GLfloat>(lon) / longitudeDivisions;
                texCoords[(lat * (longitudeDivisions + 1) + lon) * 2 + 1] = static_cast<GLfloat>(lat) / latitudeDivisions;
            }
        }

        // Genera los índices para los triángulos de la esfera
        index = 0;
        for (int lat = 0; lat < latitudeDivisions; ++lat)
        {
            for (int lon = 0; lon < longitudeDivisions; ++lon)
            {
                int first = (lat * (longitudeDivisions + 1)) + lon;
                int second = first + longitudeDivisions + 1;

                // Triángulos para la malla de la esfera
                indices[index++] = first;
                indices[index++] = first + 1;
                indices[index++] = second;

                indices[index++] = second;
                indices[index++] = first + 1;
                indices[index++] = second + 1;
            }
        }
    }

    void Sphere::render()
    {
        // Configura el modo de polígonos y activa el culling
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        // Dibuja la esfera utilizando los índices
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
