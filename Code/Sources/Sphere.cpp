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
        generateBuffers();
    }

    Sphere::~Sphere()
    {
        deleteBuffers();
    }

    void Sphere::generateGeometry()
    {
        int indexCount = latitudeDivisions * longitudeDivisions * 6;

        // Redimensiona los vectores para las coordenadas de vértices, coordenadas de textura, normales e índices
        coordinates.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 3);
        texCoords.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 2);
        normals.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 3); // Se agrega buffer de normales
        indices.resize(indexCount);

        int index = 0;
        int coordIndex = 0;
        int normalIndex = 0;

        // Calcula los vértices, coordenadas de textura y normales
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

                // Calcula y asigna las normales (posición normalizada)
                normals[normalIndex++] = x / radius;
                normals[normalIndex++] = y / radius;
                normals[normalIndex++] = z / radius;
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
}
