/**
 * @file Window.hpp
 * @brief Declaración de la clase Window para la creación y gestión de ventanas con soporte OpenGL.
 *
 * Proporciona una interfaz para inicializar una ventana SDL con un contexto OpenGL,
 * manejar su ciclo de vida y realizar operaciones básicas como el intercambio de buffers.
 *
 * @author Angel Rodriguez
 * @note Public domain
 */

#pragma once

#include <SDL.h>
#include <string>
#include <utility>

namespace udit
{
    /**
     * @class Window
     * @brief Clase para gestionar una ventana SDL con soporte para OpenGL.
     */
    class Window
    {
    public:

        /**
         * @brief Enumeración para especificar la posición de la ventana.
         */
        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED, ///< Posición no definida.
            CENTERED = SDL_WINDOWPOS_CENTERED,  ///< Posición centrada en la pantalla.
        };

        /**
         * @brief Configuración del contexto OpenGL asociado a la ventana.
         */
        struct OpenGL_Context_Settings
        {
            unsigned version_major = 3; ///< Versión mayor de OpenGL.
            unsigned version_minor = 3; ///< Versión menor de OpenGL.
            bool     core_profile = true; ///< Activar el perfil principal de OpenGL.
            unsigned depth_buffer_size = 24; ///< Tamaño del buffer de profundidad.
            unsigned stencil_buffer_size = 0;  ///< Tamaño del buffer de stencil.
            bool     enable_vsync = true; ///< Activar sincronización vertical (V-Sync).
        };

    private:

        SDL_Window* window_handle; ///< Puntero al manejador de la ventana SDL.
        SDL_GLContext opengl_context; ///< Contexto OpenGL asociado a la ventana.

    public:

        /**
         * @brief Constructor para inicializar una ventana con título y configuración OpenGL.
         *
         * @param title Título de la ventana.
         * @param left_x Posición X de la esquina superior izquierda.
         * @param top_y Posición Y de la esquina superior izquierda.
         * @param width Ancho de la ventana en píxeles.
         * @param height Alto de la ventana en píxeles.
         * @param context_details Configuración del contexto OpenGL.
         */
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

        /**
         * @brief Constructor para inicializar una ventana con título (como cadena C) y configuración OpenGL.
         *
         * @param title Título de la ventana.
         * @param left_x Posición X de la esquina superior izquierda.
         * @param top_y Posición Y de la esquina superior izquierda.
         * @param width Ancho de la ventana en píxeles.
         * @param height Alto de la ventana en píxeles.
         * @param context_details Configuración del contexto OpenGL.
         */
        Window
        (
            const char* title,
            int      left_x,
            int      top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings& context_details
        );

        /**
         * @brief Destructor de la clase Window.
         *
         * Libera los recursos de la ventana y el contexto OpenGL.
         */
        ~Window();

    public:

        /**
         * @brief Elimina el constructor de copia para evitar duplicación de recursos.
         */
        Window(const Window&) = delete;

        /**
         * @brief Elimina el operador de asignación por copia.
         */
        Window& operator = (const Window&) = delete;

        /**
         * @brief Constructor de movimiento.
         *
         * Transfiere la propiedad de los recursos al nuevo objeto.
         *
         * @param other Objeto Window del que se transfieren los recursos.
         */
        Window(Window&& other) noexcept
        {
            this->window_handle = std::exchange(other.window_handle, nullptr);
            this->opengl_context = std::exchange(other.opengl_context, nullptr);
        }

        /**
         * @brief Operador de asignación por movimiento.
         *
         * Transfiere la propiedad de los recursos al objeto actual.
         *
         * @param other Objeto Window del que se transfieren los recursos.
         * @return Referencia al objeto actual.
         */
        Window& operator = (Window&& other) noexcept
        {
            this->window_handle = std::exchange(other.window_handle, nullptr);
            this->opengl_context = std::exchange(other.opengl_context, nullptr);
        }

    public:

        /**
         * @brief Intercambia los buffers de la ventana.
         *
         * Este método presenta en pantalla el contenido renderizado.
         */
        void swap_buffers();

    };

}
