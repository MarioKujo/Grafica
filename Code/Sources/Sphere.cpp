#include "../Headers/Sphere.hpp"

namespace udit
{
    Sphere::Sphere(int latDivisions, int longDivisions, GLfloat radius)
        : latitudeDivisions(latDivisions), longitudeDivisions(longDivisions), radius(radius)
    {
        generateGeometry();

        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXCOORDS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Sphere::~Sphere()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Sphere::generateGeometry()
    {
        int indexCount = latitudeDivisions * longitudeDivisions * 6;

        coordinates.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 3);
        texCoords.resize((latitudeDivisions + 1) * (longitudeDivisions + 1) * 2);
        indices.resize(indexCount);

        int index = 0;
        int coordIndex = 0;

        for (int lat = 0; lat <= latitudeDivisions; ++lat)
        {
            for (int lon = 0; lon <= longitudeDivisions; ++lon)
            {
                GLfloat theta = static_cast<GLfloat>(lat) / latitudeDivisions * std::numbers::pi;
                GLfloat phi = static_cast<GLfloat>(lon) / longitudeDivisions * 2.0f * std::numbers::pi;

                GLfloat x = radius * sin(theta) * cos(phi);
                GLfloat y = radius * cos(theta);
                GLfloat z = radius * sin(theta) * sin(phi);

                coordinates[coordIndex++] = x;
                coordinates[coordIndex++] = y;
                coordinates[coordIndex++] = z;

                texCoords[(lat * (longitudeDivisions + 1) + lon) * 2] = static_cast<GLfloat>(lon) / longitudeDivisions;
                texCoords[(lat * (longitudeDivisions + 1) + lon) * 2 + 1] = static_cast<GLfloat>(lat) / latitudeDivisions;
            }
        }

        index = 0;

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

    void Sphere::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
