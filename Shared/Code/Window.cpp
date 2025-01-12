/**
 * @file Window.cpp
 * @author angel.rodriguez@udit.es
 * @date 2025-01-12
 *
 * Este archivo contiene la implementación de la clase `Window`, que facilita la creación y manejo
 * de una ventana con contexto OpenGL utilizando SDL.
 */

#pragma once

#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include "Window.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase `Window`.
     *
     * Este constructor inicializa el subsistema de video de SDL, configura los atributos del
     * contexto OpenGL según los detalles proporcionados y crea una ventana con un contexto OpenGL.
     *
     * @param title Título de la ventana.
     * @param left_x Posición en el eje X de la ventana.
     * @param top_y Posición en el eje Y de la ventana.
     * @param width Ancho de la ventana.
     * @param height Alto de la ventana.
     * @param context_details Detalles del contexto OpenGL, como versión, perfil, etc.
     *
     * @throws "Failed to initialize the video subsystem." Si SDL no puede inicializar el subsistema de video.
     * @throws `nullptr` Si no se puede crear la ventana o el contexto OpenGL.
     */
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

        if (context_details.core_profile)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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

        assert(window_handle != nullptr);

        // Crear el contexto OpenGL asociado con la ventana
        opengl_context = SDL_GL_CreateContext(window_handle);

        assert(opengl_context != nullptr);

        // Cargar GLAD
        GLenum glad_is_initialized = gladLoadGL();

        assert(glad_is_initialized);

        // Configurar V-Sync
        SDL_GL_SetSwapInterval(context_details.enable_vsync ? 1 : 0);
    }

    /**
     * @brief Destructor de la clase `Window`.
     *
     * Este destructor destruye el contexto OpenGL y la ventana, y limpia el subsistema de video de SDL.
     */
    Window::~Window()
    {
        if (opengl_context)
        {
            SDL_GL_DeleteContext(opengl_context);
        }

        if (window_handle)
        {
            SDL_DestroyWindow(window_handle);
        }

        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    /**
     * @brief Intercambia los buffers de la ventana.
     *
     * Este método intercambia los buffers del contexto OpenGL, mostrando el contenido renderizado
     * en el buffer de la ventana.
     */
    void Window::swap_buffers()
    {
        SDL_GL_SwapWindow(window_handle);
    }

}
