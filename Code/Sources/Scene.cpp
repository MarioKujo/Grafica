
// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include "../Headers/Scene.hpp"

#include <iostream>
#include <cassert>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr
#include <vector>

namespace udit
{

    using namespace std;

    const std::string Scene::vertex_shader_code =
        "#version 330 core\n"
        "\n"
        "uniform mat4 model_view_matrix;\n"
        "uniform mat4 projection_matrix;\n"
        "\n"
        "layout(location = 0) in float t;\n"
        "\n"
        "out vec3 front_color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    float num_turns = 5.0;\n"
        "    float height = 4.0;\n"
        "    float angle = t * num_turns * 2.0 * 3.14159265359;\n"
        "\n"
        "    float x = cos(angle);\n"
        "    float z = sin(angle);\n"
        "    float y = t * height;\n"
        "\n"
        "    vec3 position = vec3(x, y, z);\n"
        "    gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);\n"
        "\n"
        "    front_color = vec3(1.0, 1.0, 1.0);\n"
        "}\n";

    const string Scene::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(front_color, 1.0);"
        "}";

    Scene::Scene(unsigned width, unsigned height)
        :
        angle(0)
    {
        // Se establece la configuración básica:

        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glClearColor(.2f, .2f, .2f, 1.f);

        // Se compilan y se activan los shaders:

        GLuint program_id = compile_shaders();

        glUseProgram(program_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        resize(width, height);
    }

    void Scene::update()
    {
        angle += 0.01f;
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model_view_matrix(1);
        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(2.5f, 0.f, -5.f));
        model_view_matrix = glm::rotate(model_view_matrix, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        draw_spiral_single_float(1000);
    }

    void Scene::draw_spiral_single_float(int num_vertices)
    {
        // Vector con un único float por vértice, t en [0, 1]
        vector<float> ts(num_vertices);
        for (int i = 0; i < num_vertices; ++i)
        {
            ts[i] = i / float(num_vertices - 1);
        }

        GLuint vbo, vao;
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        // Copiar datos al VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, ts.size() * sizeof(float), ts.data(), GL_STATIC_DRAW);

        // Atributo con un float
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(ts.size()));

        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }


    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport(0, 0, width, height);
    }

    GLuint Scene::compile_shaders()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char* vertex_shaders_code[] = { vertex_shader_code.c_str() };
        const char* fragment_shaders_code[] = { fragment_shader_code.c_str() };
        const GLint    vertex_shaders_size[] = { (GLint)vertex_shader_code.size() };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram();

        // Se cargan los shaders compilados en el programa:

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram(program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return (program_id);
    }

    void Scene::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetShaderInfoLog(shader_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

    void Scene::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize(info_log_length);

        glGetProgramInfoLog(program_id, info_log_length, NULL, &info_log.front());

        cerr << info_log.c_str() << endl;

#ifdef _MSC_VER
        //OutputDebugStringA (info_log.c_str ());
#endif

        assert(false);
    }

}
