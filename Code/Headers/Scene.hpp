#pragma once

#include "Cube.hpp"
#include "Camera.hpp" // Incluir la cámara
#include <string>
#include <iostream>
#include <cassert>
#include <SDL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace udit
{

    class Scene
    {
    private:

        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;

        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;

        Cube   cube;
        float  angle;

        Camera camera; // Añadido: Cámara como parte de la escena

    public:

        Scene(unsigned width, unsigned height);

        void   update(float delta_time); // Se pasa el delta_time para movimientos suaves
        void   render();
        void   resize(unsigned width, unsigned height);

        void   handle_mouse_motion(int xrel, int yrel); // Para manejar movimientos del ratón

    private:

        GLuint compile_shaders();
        void   show_compilation_error(GLuint  shader_id);
        void   show_linkage_error(GLuint program_id);

    };

}
