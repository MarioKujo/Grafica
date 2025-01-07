/**
 * @file Window.cpp
 * @brief Implementación de la clase `Window` que encapsula la creación y manejo de una ventana
 *        OpenGL utilizando SDL y GLAD para la inicialización de OpenGL.
 *
 * @author angel.rodriguez@udit.es
 * @note Public domain
 */

#pragma once

#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include "Window.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase `Window`, encargado de inicializar SDL, configurar el contexto
     *        de OpenGL y crear la ventana.
     *
     * Este constructor configura los atributos de OpenGL según los detalles proporcionados, crea
     * una ventana con soporte para OpenGL, y establece un contexto OpenGL para esa ventana.
     *
     * @param title Título de la ventana.
     * @param left_x Coordenada X de la posición inicial de la ventana.
     * @param top_y Coordenada Y de la posición inicial de la ventana.
     * @param width Ancho de la ventana.
     * @param height Alto de la ventana.
     * @param context_details Detalles del contexto OpenGL que se debe crear.
     *
     * @throws const char* Excepción si no se puede inicializar el subsistema de video SDL o si
     *         algún contexto de OpenGL no se crea correctamente.
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
        // Se hace inicializa SDL:
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            throw "Failed to initialize the video subsystem.";
        }

        // Se preconfigura el contexto de OpenGL:
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

        // Se crea la ventana activando el soporte para OpenGL:
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

        // Se crea un contexto de OpenGL asociado a la ventana:
        opengl_context = SDL_GL_CreateContext(window_handle);

        assert(opengl_context != nullptr);

        // Una vez se ha creado el contexto de OpenGL ya se puede inicializar GLAD:
        GLenum glad_is_initialized = gladLoadGL();

        assert(glad_is_initialized);

        // Se activa la sincronización con el refresco vertical del display:
        SDL_GL_SetSwapInterval(context_details.enable_vsync ? 1 : 0);
    }

    /**
     * @brief Destructor de la clase `Window`.
     *
     * Este destructor se encarga de liberar los recursos asociados con la ventana y el contexto
     * OpenGL, así como de finalizar el subsistema de video de SDL.
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
     * @brief Intercambia los buffers de la ventana, mostrando el contenido renderizado.
     *
     * Este método es utilizado para actualizar la ventana y mostrar el contenido renderizado en
     * el contexto OpenGL.
     */
    void Window::swap_buffers()
    {
        SDL_GL_SwapWindow(window_handle);
    }

}
