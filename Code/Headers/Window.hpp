#pragma once

#include <SDL.h>
#include <string>
#include <utility>
#include "Camera.hpp"

namespace udit
{
    class Window
    {
    public:
        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED, /**< La posición de la ventana no está definida. */
            CENTERED = SDL_WINDOWPOS_CENTERED,   /**< La ventana se centrará en la pantalla. */
        };

        struct OpenGL_Context_Settings
        {
            unsigned version_major = 3; /**< Versión principal de OpenGL. */
            unsigned version_minor = 3; /**< Versión secundaria de OpenGL. */
            bool     core_profile = true; /**< Si se debe usar el perfil core de OpenGL. */
            unsigned depth_buffer_size = 24; /**< Tamaño del buffer de profundidad. */
            unsigned stencil_buffer_size = 0; /**< Tamaño del buffer de stencil. */
            bool     enable_vsync = true; /**< Si se debe habilitar el V-Sync. */
        };

    private:

        SDL_Window* window_handle; /**< Manejador de la ventana de SDL. */
        SDL_GLContext opengl_context; /**< Contexto de OpenGL asociado con la ventana. */
        Camera camera; /**< Cámara para observar los objetos de la escena. */
    public:

        Window
        (
            const std::string& title,
            int      left_x,
            int      top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings& context_details
        )
            :
            Window(title.c_str(), left_x, top_y, width, height, context_details)
        {
        }


        Window
        (
            const char* title,
            int      left_x,
            int      top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings& context_details
        );


        ~Window();

    public:


        Window(const Window&) = delete;


        Window& operator = (const Window&) = delete;


        Window(Window&& other) noexcept
        {
            this->window_handle = std::exchange(other.window_handle, nullptr);
            this->opengl_context = std::exchange(other.opengl_context, nullptr);
        }


        Window& operator = (Window&& other) noexcept
        {
            this->window_handle = std::exchange(other.window_handle, nullptr);
            this->opengl_context = std::exchange(other.opengl_context, nullptr);
        }

    public:


        void swap_buffers();


        void move_camera(bool* exit);


        Camera get_camera();
    };

}
