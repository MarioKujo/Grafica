#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include "../Headers/stb_image.hpp"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();
    void set_texture(GLuint texture_id);
    GLuint get_texture_id();
    void render();  // Recibe las matrices desde la escena

private:
    GLuint vao_id;
    GLuint vbo_id;
    GLuint texture_id;
    std::vector<std::string> faces;

    void setup_buffers();
};
