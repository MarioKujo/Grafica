// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once

#include "../Headers/Scene.hpp"

namespace udit
{
    const string Scene::vertex_shader_code =
        "#version 330\n"
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 vertex_texCoords;"
        "layout (location = 2) in vec3 vertex_normal;"  // Normal del vértice
        "out vec2 texCoords;"
        "out vec3 fragNormal;"  // Enviar la normal al fragment shader
        "out vec3 fragPos;"     // Enviar la posición del vértice al fragment shader
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   texCoords = vertex_texCoords;"
        "   fragNormal = mat3(transpose(inverse(model_view_matrix))) * vertex_normal;" // Transformar la normal
        "   fragPos = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));"  // Obtener la posición
        "}";


    // Código fuente del shader de fragmentos para la escena
    const string Scene::fragment_shader_code =
        "#version 330 core\n"
        "in vec2 texCoords;"
        "in vec3 fragNormal;"
        "in vec3 fragPos;"
        "out vec4 fragment_color;"
        "uniform sampler2D textureSampler;"
        "uniform float transparency;"

        // Parametros de la luz
        "uniform vec3 lightPos;"  // Posición de la luz
        "uniform vec3 lightColor;"  // Color de la luz

        "void main()"
        "{"
        "    vec4 texColor = texture(textureSampler, texCoords);"

        // Cálculo de la iluminación difusa Lambert
        "    vec3 norm = normalize(fragNormal);"
        "    vec3 lightDir = normalize(lightPos - fragPos);"
        "    float diff = max(dot(norm, lightDir), 0.0);"
        "    vec3 diffuse = diff * lightColor;"

        // Color final del fragmento
        "    vec3 result = texColor.rgb * diffuse;"
        "    fragment_color = vec4(result, texColor.a * transparency);"
        "}";


    // Código fuente del shader de vértices para el skybox
    const string Scene::skybox_vertex_shader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "out vec3 TexCoords;"
        "uniform mat4 projection;"
        "uniform mat4 view;"
        "void main()"
        "{"
        "   TexCoords = aPos;"
        "   vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);"
        "   gl_Position = pos.xyww;"
        "}";

    // Código fuente del shader de fragmentos para el skybox
    const string Scene::skybox_fragment_shader =
        "#version 330 core\n"
        "in vec3 TexCoords;"
        "out vec4 FragColor;"
        "uniform samplerCube skybox;"
        "void main()"
        "{"
        "   FragColor = texture(skybox, TexCoords);"
        "}";

    // Constructor de la escena, inicializa objetos 3D, cámara, shaders y texturas
    Scene::Scene(unsigned width, unsigned height)
        : angle(0),
        camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f),
        plane(7, 5), cylinder(10, 10, 2, 5), cone(10, 2, 5),
        sphere1(10, 10, 3.5f), sphere2(10, 10, 3.75f),
        skybox({ "../Textures/sky-cube-map-0.png", "../Textures/sky-cube-map-1.png",
                 "../Textures/sky-cube-map-2.png", "../Textures/sky-cube-map-3.png",
                 "../Textures/sky-cube-map-4.png", "../Textures/sky-cube-map-5.png" }),
        heightmap("../Textures/heightmap.png", 10.0f, 10.0f, 0.5f)
    {

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.2f, .2f, .2f, 1.f);

        program_id = shaderProgram.compile_shaders(vertex_shader_code, fragment_shader_code);
        glUseProgram(program_id);
        skybox_program_id = shaderProgram.compile_shaders(skybox_vertex_shader, skybox_fragment_shader);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        lightPos = glm::vec3(10.0f, 10.0f, 10.0f);  // Posición de la luz
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Color blanco para la luz
        viewPos = glm::vec3(0.0f, 0.0f, 8.0f);  // Posición de la cámara

        // Cargar texturas
        planeTextureID = textureLoader.loadTexture("../Textures/plane_texture.jpg");
        cylinderTextureID = textureLoader.loadTexture("../Textures/cylinder_texture.jpg");
        coneTextureID = textureLoader.loadTexture("../Textures/cone_texture.jpg");
        sphereTextureID = textureLoader.loadTexture("../Textures/sphere_texture.jpg");
        heightmapTextureID = textureLoader.loadTexture("../Textures/heightmap_texture.jpg");
        skyboxTextureID = textureLoader.loadCubemap({ "../Textures/sky-cube-map-0.png", "../Textures/sky-cube-map-1.png",
                                                     "../Textures/sky-cube-map-2.png", "../Textures/sky-cube-map-3.png",
                                                     "../Textures/sky-cube-map-4.png", "../Textures/sky-cube-map-5.png" });
        skybox.set_texture(skyboxTextureID);

        glUniform1i(glGetUniformLocation(program_id, "textureSampler"), 0);
        resize(width, height);
    }

    // Actualiza la escena (cámara y rotación de objetos)
    void Scene::update()
    {
        angle += 0.01f;
    }

    // Renderiza todos los objetos en la escena
    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(skybox_program_id);

        glm::mat4 view_matrix = camera.get_view_matrix();
        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(skybox_program_id, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(glGetUniformLocation(skybox_program_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get_texture_id());
        skybox.render();

        glUseProgram(program_id);

        lightPos = glm::vec3(view_matrix * glm::vec4(10.0f, 10.0f, 10.0f, 1.0f));
        glUniform3fv(glGetUniformLocation(program_id, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(program_id, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(program_id, "viewPos"), 1, glm::value_ptr(viewPos));

        // Renderiza el plano
        glm::mat4 plane_matrix(1);
        plane_matrix = glm::translate(plane_matrix, glm::vec3(0.f, -2.f, 0.f));
        plane_matrix = glm::rotate(plane_matrix, glm::radians(-75.f), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 plane_view_matrix = view_matrix * plane_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(plane_view_matrix));
        glBindTexture(GL_TEXTURE_2D, planeTextureID);
        plane.render();

        lightPos = glm::vec3(view_matrix * glm::vec4(0.f, 10.f, 10.f, 1.0f));
        glUniform3fv(glGetUniformLocation(program_id, "lightPos"), 1, glm::value_ptr(lightPos));

        // Renderiza el cilindro
        glm::mat4 cylinder_matrix(1);
        cylinder_matrix = glm::translate(cylinder_matrix, glm::vec3(-4.f, 1.1f, -2.f));
        cylinder_matrix = glm::rotate(cylinder_matrix, glm::radians(15.f), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 cylinder_view_matrix = view_matrix * cylinder_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_view_matrix));
        glBindTexture(GL_TEXTURE_2D, cylinderTextureID);
        cylinder.render();

        lightPos = glm::vec3(view_matrix * glm::vec4(-15.f, -45.f, -20.f, 1.0f));
        glUniform3fv(glGetUniformLocation(program_id, "lightPos"), 1, glm::value_ptr(lightPos));

        // Renderiza el cono
        glm::mat4 cone_matrix(1);
        cone_matrix = glm::translate(cone_matrix, glm::vec3(4.f, -1.45f, -2.f));
        cone_matrix = glm::rotate(cone_matrix, glm::radians(15.f), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 cone_view_matrix = view_matrix * cone_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_view_matrix));
        glBindTexture(GL_TEXTURE_2D, coneTextureID);
        cone.render();


        lightPos = glm::vec3(view_matrix * glm::vec4(0.f, 10.f, 10.f, 1.0f));
        glUniform3fv(glGetUniformLocation(program_id, "lightPos"), 1, glm::value_ptr(lightPos));

        // Renderiza las esferas
        glm::mat4 sphere1_matrix(1);
        sphere1_matrix = glm::translate(sphere1_matrix, glm::vec3(0.f, 7.f, -2.f));
        sphere1_matrix = glm::rotate(sphere1_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 sphere1_view_matrix = view_matrix * sphere1_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(sphere1_view_matrix));
        glBindTexture(GL_TEXTURE_2D, sphereTextureID);
        sphere1.render();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLint transparency = glGetUniformLocation(program_id, "transparency");
        glUniform1f(transparency, 0.5f);

        glm::mat4 sphere2_matrix(1);
        sphere2_matrix = glm::translate(sphere2_matrix, glm::vec3(0.f, 7.f, -2.f));
        sphere2_matrix = glm::rotate(sphere2_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 sphere2_view_matrix = view_matrix * sphere2_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(sphere2_view_matrix));
        glBindTexture(GL_TEXTURE_2D, sphereTextureID);
        sphere2.render();

        glDisable(GL_BLEND);

        // Renderiza el mapa de alturas
        glm::mat4 heightmap_matrix(1);
        heightmap_matrix = glm::translate(heightmap_matrix, glm::vec3(0.0f, -45.0f, 0.0f));
        heightmap_matrix = glm::scale(heightmap_matrix, glm::vec3(100.0f, 100.0f, 100.0f));
        glm::mat4 heightmap_view_matrix = view_matrix * heightmap_matrix;
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(heightmap_view_matrix));
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glBindTexture(GL_TEXTURE_2D, heightmapTextureID);
        heightmap.render();
    }

    // Ajusta el tamaño de la ventana y la proyección
    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height);
    }

    void Scene::set_camera(Camera new_camera)
    {
        camera = new_camera;
    }


}
