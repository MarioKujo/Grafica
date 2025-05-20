// Este código es de dominio público
// andrmatgonros@gmail.com

#pragma once

#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include "../Headers/Window.hpp"
#include <stdexcept>

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
        : camera(glm::vec3(0.f, 3.f, 8.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f)
    {
        // Inicializa el subsistema de video de SDL
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error("Failed to initialize the video subsystem.");
        }

        // Configura versión y atributos básicos del contexto OpenGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context_details.version_major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context_details.version_minor);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Doble buffer para evitar parpadeos
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Asegura aceleración por hardware

        // Aplica perfil core si está especificado
        if (context_details.core_profile)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Configura buffers de profundidad y stencil si están habilitados
        if (context_details.depth_buffer_size)
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context_details.depth_buffer_size);
        if (context_details.stencil_buffer_size)
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context_details.stencil_buffer_size);

        // Crea la ventana SDL con soporte OpenGL
        window_handle = SDL_CreateWindow
        (
            title,
            left_x,
            top_y,
            int(width),
            int(height),
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        // Verifica que la ventana fue creada correctamente
        if (!window_handle)
            throw std::runtime_error("Failed to create SDL Window.");


        // Crea el contexto OpenGL
        opengl_context = SDL_GL_CreateContext(window_handle);

        // Verifica que el contexto fue creado correctamente
        assert(opengl_context != nullptr);

        // Carga las funciones de OpenGL mediante GLAD
        GLenum glad_is_initialized = gladLoadGL();
        assert(glad_is_initialized); // Verifica la carga correcta

        // Habilita o deshabilita V-Sync
        SDL_GL_SetSwapInterval(context_details.enable_vsync ? 1 : 0);
    }

    Window::~Window()
    {
        // Elimina el contexto de OpenGL
        if (opengl_context)
        {
            SDL_GL_DeleteContext(opengl_context);
        }

        // Destruye la ventana SDL
        if (window_handle)
        {
            SDL_DestroyWindow(window_handle);
        }

        // Finaliza el subsistema de video de SDL
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void Window::swap_buffers()
    {
        // Intercambia los buffers del contexto actual para mostrar el nuevo frame
        SDL_GL_SwapWindow(window_handle);
    }

    void Window::poll_input_events(bool* exit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_MOUSEMOTION)
                camera.process_mouse_motion((float)event.motion.xrel, (float)event.motion.yrel);
            else if (event.type == SDL_QUIT)
                *exit = true;
        }
    }

    void Window::move_camera(bool* exit)
    {
        poll_input_events(exit);
        camera.process_keyboard(SDL_GetKeyboardState(NULL));
    }

    Camera Window::get_camera()
    {
        // Devuelve una copia de la cámara actual
        return camera;
    }
}
