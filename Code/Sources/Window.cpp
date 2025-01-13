// Este código es de dominio público
// andrmatgonros@gmail.com
#pragma once

#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include "../Headers/Window.hpp"

namespace udit
{
    Window::Window
    (
        const char* title,
        int left_x,
        int top_y,
        unsigned width,
        unsigned height,
        const OpenGL_Context_Settings& context_details
    )
    {
        // Inicializar el subsistema de video de SDL
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            throw "Failed to initialize the video subsystem.";
        }

        // Configurar los atributos del contexto OpenGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context_details.version_major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context_details.version_minor);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        // Si se requiere el perfil core de OpenGL, configurarlo
        if (context_details.core_profile)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Configurar tamaño del buffer de profundidad y stencil
        if (context_details.depth_buffer_size)
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context_details.depth_buffer_size);
        if (context_details.stencil_buffer_size)
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context_details.stencil_buffer_size);

        // Crear la ventana SDL con soporte OpenGL
        window_handle = SDL_CreateWindow
        (
            title,
            left_x,
            top_y,
            int(width),
            int(height),
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        // Asegurarse de que la ventana se creó correctamente
        assert(window_handle != nullptr);

        // Crear el contexto OpenGL asociado con la ventana
        opengl_context = SDL_GL_CreateContext(window_handle);

        // Asegurarse de que el contexto se creó correctamente
        assert(opengl_context != nullptr);

        // Cargar GLAD para gestionar las funciones de OpenGL
        GLenum glad_is_initialized = gladLoadGL();

        // Asegurarse de que GLAD se inicializó correctamente
        assert(glad_is_initialized);

        // Configurar V-Sync según la configuración proporcionada
        SDL_GL_SetSwapInterval(context_details.enable_vsync ? 1 : 0);
    }

    Window::~Window()
    {
        // Eliminar el contexto OpenGL si existe
        if (opengl_context)
        {
            SDL_GL_DeleteContext(opengl_context);
        }

        // Destruir la ventana SDL si existe
        if (window_handle)
        {
            SDL_DestroyWindow(window_handle);
        }

        // Limpiar el subsistema de video de SDL
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void Window::swap_buffers()
    {
        // Intercambiar los buffers del contexto OpenGL para mostrar el contenido renderizado
        SDL_GL_SwapWindow(window_handle);
    }
}
