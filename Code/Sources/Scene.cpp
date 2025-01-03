// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include "../Headers/Scene.hpp"

#include <iostream>
#include <cassert>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

namespace udit
{

    using namespace std;

    /**
     * @class Scene
     * @brief Representa una escena 3D con un cilindro y un plano, gestionados por shaders y transformaciones.
     */
    const string Scene::vertex_shader_code =
        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";

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

    /**
     * @brief Constructor de la clase Scene.
     *
     * Configura el estado inicial de OpenGL, compila los shaders y ajusta el tamaño de la proyección.
     *
     * @param width Ancho de la ventana.
     * @param height Alto de la ventana.
     */
    Scene::Scene(unsigned width, unsigned height)
        :
        angle(0), plane(7, 5), cylinder(10, 10, 2, 5), cone(10, 2, 5)
    {
        glEnable(GL_CULL_FACE); // Habilita el culling de caras
        glDisable(GL_DEPTH_TEST); // Desactiva la prueba de profundidad
        glClearColor(.2f, .2f, .2f, 1.f); // Color de fondo de la escena

        GLuint program_id = compile_shaders(); // Compila y enlaza los shaders
        glUseProgram(program_id);

        // Obtiene las ubicaciones de las matrices uniformes
        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        resize(width, height); // Configura la proyección inicial
    }

    /**
     * @brief Actualiza el estado de la escena.
     *
     * Incrementa el ángulo de rotación para animaciones.
     */
    void Scene::update()
    {
        angle += 0.01f;
    }

    /**
     * @brief Renderiza la escena.
     *
     * Limpia el buffer, configura las matrices de transformación y dibuja el plano y el cilindro.
     */
    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Matriz de modelo y vista
        glm::mat4 model_view_matrix(1);
        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(-1.25f, 0.f, -10.f)); // Cámara alejada

        // Renderizado del plano
        glm::mat4 plane_model_view_matrix = model_view_matrix;
        plane_model_view_matrix = glm::translate(plane_model_view_matrix, glm::vec3(0.f, -2.f, 0.f));

        plane_model_view_matrix = glm::rotate(plane_model_view_matrix, glm::radians(-75.f), glm::vec3(1.f, 0.f, 0.f)); // Rotación del plano

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(plane_model_view_matrix));
        plane.render(); // Dibuja el plano

        // Renderizado del cilindro
        glm::mat4 cylinder_model_view_matrix = model_view_matrix;
        cylinder_model_view_matrix = glm::translate(cylinder_model_view_matrix, glm::vec3(-4.f, 0.f, -2.f));
        cylinder_model_view_matrix = glm::rotate(cylinder_model_view_matrix, glm::radians(30.f),
            glm::vec3(1.f, 0.f, 0.f));

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_model_view_matrix));
        cylinder.render(); // Dibuja el cilindro

        // Renderizado del cono
        glm::mat4 cone_model_view_matrix = model_view_matrix;
        cone_model_view_matrix = glm::translate(cone_model_view_matrix, glm::vec3(4.f, -2.f, -2.f));
        cone_model_view_matrix = glm::rotate(cone_model_view_matrix, glm::radians(30.f),
            glm::vec3(1.f, 0.f, 0.f));

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_model_view_matrix));
        cone.render(); // Dibuja el cono
    }

    /**
     * @brief Ajusta el tamaño de la proyección en función del tamaño de la ventana.
     *
     * @param width Ancho de la ventana.
     * @param height Alto de la ventana.
     */
    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height);
    }

    /**
     * @brief Compila y enlaza los shaders.
     *
     * Crea un programa de shaders a partir del código fuente de los shaders de vértices y fragmentos.
     *
     * @return ID del programa de shaders.
     */
    GLuint Scene::compile_shaders()
    {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_shaders_code[] = { vertex_shader_code.c_str() };
        const char* fragment_shaders_code[] = { fragment_shader_code.c_str() };
        const GLint vertex_shaders_size[] = { (GLint)vertex_shader_code.size() };
        const GLint fragment_shaders_size[] = { (GLint)fragment_shader_code.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        GLuint program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);
        glLinkProgram(program_id);

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return program_id;
    }

    /**
     * @brief Muestra el error de compilación de un shader.
     *
     * @param shader_id ID del shader que falló.
     */
    void Scene::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint info_log_length;

        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &info_log.front());
        cerr << info_log.c_str() << endl;

        assert(false);
    }

    /**
     * @brief Muestra el error de enlace de un programa de shaders.
     *
     * @param program_id ID del programa que falló.
     */
    void Scene::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint info_log_length;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length);
        glGetProgramInfoLog(program_id, info_log_length, NULL, &info_log.front());
        cerr << info_log.c_str() << endl;

        assert(false);
    }

}
