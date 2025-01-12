/**
 * @file Sphere.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este archivo implementa la clase `Sphere`, la cual genera y renderiza una esfera 3D
 * utilizando OpenGL. La esfera se genera utilizando una malla de subdivisiones latitudinales
 * y longitudinales, lo que permite definir la resolución de la geometría.
 */

#include "../Headers/Sphere.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase Sphere.
     *
     * El constructor toma las divisiones latitudinales, longitudinales y el radio de la
     * esfera, luego genera la geometría y configura los buffers de OpenGL para los vértices,
     * coordenadas de textura y los índices.
     *
     * @param latDivisions Número de divisiones latitudinales de la esfera.
     * @param longDivisions Número de divisiones longitudinales de la esfera.
     * @param radius Radio de la esfera.
     */
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

    /**
     * @brief Destructor de la clase Sphere.
     *
     * El destructor limpia los recursos de OpenGL, eliminando los buffers de vértices y
     * el Vertex Array Object (VAO) utilizados para renderizar la esfera.
     */
    Sphere::~Sphere()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    /**
     * @brief Genera la geometría de la esfera.
     *
     * Este método calcula los vértices, las coordenadas de textura y los índices que definen
     * la malla de la esfera utilizando las divisiones latitudinales, longitudinales y el radio.
     */
    void Sphere::generateGeometry()
    {
        int indexCount = latitudeDivisions * longitudeDivisions * 6;

        // Redimensiona los vectores para almacenar las coordenadas de vértices, las coordenadas de textura y los índices
        coordinates.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 3);
        texCoords.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 2);
        indices.resize(indexCount);

        int index = 0;
        int coordIndex = 0;

        // Calcula los vértices y las coordenadas de textura para la esfera
        for (int lat = 0; lat <= latitudeDivisions; ++lat)
        {
            for (int lon = 0; lon <= longitudeDivisions; ++lon)
            {
                GLfloat theta = static_cast<GLfloat>((static_cast<double>(lat) / latitudeDivisions) * std::numbers::pi);
                GLfloat phi = static_cast<GLfloat>((static_cast<double>(lon) / longitudeDivisions) * 2.0f * std::numbers::pi);

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

        index = 0;

        // Genera los índices para los triángulos de la esfera
        for (int lat = 0; lat < latitudeDivisions; ++lat)
        {
            for (int lon = 0; lon < longitudeDivisions; ++lon)
            {
                int first = (lat * (longitudeDivisions + 1)) + lon;
                int second = first + longitudeDivisions + 1;

                indices[index++] = first;
                indices[index++] = first + 1;
                indices[index++] = second;

                indices[index++] = second;
                indices[index++] = first + 1;
                indices[index++] = second + 1;
            }
        }
    }

    /**
     * @brief Renderiza la esfera.
     *
     * Este método dibuja la esfera utilizando los datos de vértices, coordenadas de textura e índices
     * almacenados en los buffers. Utiliza un `Vertex Array Object` (VAO) para hacer uso de los
     * datos de la malla y renderiza los triángulos utilizando el modo de dibujo `GL_TRIANGLES`.
     */
    void Sphere::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
