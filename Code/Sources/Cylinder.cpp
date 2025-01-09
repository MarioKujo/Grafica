#include "../Headers/Cylinder.hpp"

namespace udit
{
    Cylinder::Cylinder(int stack, int slice, GLfloat r, GLfloat h)
        : stack_count(stack), slice_count(slice), radius(r), height(h)
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
        colors.resize(vertexCount * 3);
        indices.resize(indexCount);

        int vertexIndex = 0;
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
            }
        }

        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = -height / 2;
        coordinates[vertexIndex++] = 0.0f;

        int bottomCenterIndex = (vertexIndex / 3) - 1;

        coordinates[vertexIndex++] = 0.0f;
        coordinates[vertexIndex++] = height / 2;
        coordinates[vertexIndex++] = 0.0f;

        int topCenterIndex = (vertexIndex / 3) - 1;

        GLfloat rainbowColors[7][3] = {
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.29f, 0.0f, 0.51f}, {0.56f, 0.0f, 1.0f} };

        vertexIndex = 0;
        for (int stack = 0; stack < stack_count; ++stack)
        {
            for (int slice = 0; slice < slice_count; ++slice)
            {
                GLfloat* color = rainbowColors[slice % 7];
                colors[vertexIndex++] = color[0];
                colors[vertexIndex++] = color[1];
                colors[vertexIndex++] = color[2];
            }
        }

        int index = 0;
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
                indices[index++] = next;
                indices[index++] = above;

                indices[index++] = next;
                indices[index++] = aboveNext;
                indices[index++] = above;
            }
        }

        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = bottomCenterIndex;
            indices[index++] = slice;
            indices[index++] = nextSlice;
        }

        int offset = (stack_count - 1) * slice_count;
        for (int slice = 0; slice < slice_count; ++slice)
        {
            int nextSlice = (slice + 1) % slice_count;
            indices[index++] = topCenterIndex;
            indices[index++] = offset + slice;
            indices[index++] = offset + nextSlice;
        }
    }

    void Cylinder::render()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }
}
