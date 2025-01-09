/**
 * @file Scene.cpp
 * @brief Implementación de la clase Scene para gestionar una escena en OpenGL.
 *
 * Este archivo define los métodos de la clase Scene, incluyendo la inicialización de shaders,
 * renderizado de objetos, manejo de eventos y configuración de la cámara.
 *
 * @author Angel Rodriguez
 * @note Public domain
 */

#pragma once

#include "../Headers/Scene.hpp"

namespace udit
{

    using namespace std;

    /**
     * @brief Código del shader de vértices en GLSL.
     */
    const string Scene::vertex_shader_code =
        "#version 330\n"
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        "out vec3 front_color;"
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";

    /**
     * @brief Código del shader de fragmentos en GLSL.
     */
    const string Scene::fragment_shader_code =
        "#version 330\n"
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        "uniform sampler2D textureSampler;" ///< Uniform de la textura
        "void main()"
        "{"
        "    fragment_color = texture(textureSampler, front_color.xy);"
        "}";

    /**
     * @brief Constructor de la clase Scene.
     *
     * Configura los parámetros iniciales, compila shaders, inicializa la cámara
     * y ajusta el viewport.
     *
     * @param width Ancho inicial del viewport.
     * @param height Alto inicial del viewport.
     */
    Scene::Scene(unsigned width, unsigned height)
        : angle(0), camera(glm::vec3(0.0f, 0.0f, 3.0f)), plane(7, 5), cylinder(10, 10, 2, 5), cone(10, 2, 5) ///< Inicialización de la cámara.
    {
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glClearColor(.2f, .2f, .2f, 1.f);

        program_id = compile_shaders();

        glUseProgram(program_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        // Cargar las texturas
        coneTextureID = textureLoader.loadTexture("../Textures/cone_texture.jpg");

        glUniform1i(glGetUniformLocation(program_id, "textureSampler"), 0); ///< Unir la textura al slot 0

        resize(width, height);
    }

    void Scene::update(float delta_time)
    {
        angle += 0.01f; ///< Incremento del ángulo para animación.

        const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);
        camera.process_keyboard(keyboard_state, delta_time); ///< Actualización de la cámara.
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Obtenemos la matriz de vista desde la cámara
        glm::mat4 view_matrix = camera.get_view_matrix();

        // Renderizado del cubo
        glm::mat4 cube_matrix(1);
        cube_matrix = glm::translate(cube_matrix, glm::vec3(0.f, 0.f, -4.f));
        cube_matrix = glm::rotate(cube_matrix, angle, glm::vec3(1.f, 2.f, 1.f));
        glm::mat4 cube_view_matrix = view_matrix * cube_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cube_view_matrix));
        cube.render();
        // Renderizado del plano
        glm::mat4 plane_matrix(1);
        plane_matrix = glm::translate(plane_matrix, glm::vec3(0.f, -2.f, 0.f));
        plane_matrix = glm::rotate(plane_matrix, glm::radians(-75.f),
            glm::vec3(1.f, 0.f, 0.f)); // Rotación del plano
        glm::mat4 plane_view_matrix = view_matrix * plane_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(plane_view_matrix));
        plane.render(); // Dibuja el plano

        // Renderizado del cilindro
        glm::mat4 cylinder_matrix(1);
        cylinder_matrix = glm::translate(cylinder_matrix, glm::vec3(-4.f, 1.1f, -2.f));
        cylinder_matrix = glm::rotate(cylinder_matrix, glm::radians(15.f),
            glm::vec3(1.f, 0.f, 0.f));

        glm::mat4 cylinder_view_matrix = view_matrix * cylinder_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_view_matrix));
        cylinder.render(); // Dibuja el cilindro

        // Renderizado del cono
        glm::mat4 cone_matrix(1);
        cone_matrix = glm::translate(cone_matrix, glm::vec3(4.f, -1.45f, -2.f));
        cone_matrix = glm::rotate(cone_matrix, glm::radians(15.f),
            glm::vec3(1.f, 0.f, 0.f));

        glm::mat4 cone_view_matrix = view_matrix * cone_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_view_matrix));
        glBindTexture(GL_TEXTURE_2D, coneTextureID);
        cone.render(); // Dibuja el cono
    }

    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height);
    }

    void Scene::handle_mouse_motion(int xrel, int yrel)
    {
        camera.process_mouse_motion(xrel, yrel); ///< Actualización de la orientación de la cámara.
    }

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

        return (program_id);
    }

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
