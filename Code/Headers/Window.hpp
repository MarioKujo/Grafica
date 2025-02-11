/**
 * @file Window.hpp
 * @author angel.rodriguez@udit.es
 * @date 2025-01-12
 *
 * Este archivo contiene la definición de la clase `Window`, que facilita la creación y manejo
 * de una ventana con contexto OpenGL utilizando SDL.
 */

#pragma once

#include <SDL.h>
#include <string>
#include <utility>
#include "Camera.hpp"

namespace udit
{

    /**
     * @class Window
     * @brief Clase que gestiona la creación y manipulación de una ventana con contexto OpenGL.
     *
     * Esta clase proporciona una interfaz sencilla para crear una ventana que utilice OpenGL
     * como contexto de renderizado, configurando los detalles del contexto de OpenGL y ofreciendo
     * funcionalidades como el intercambio de buffers.
     */
    class Window
    {
    public:

        /**
         * @enum Position
         * @brief Define las posiciones predefinidas para la ventana.
         *
         * Esta enumeración contiene las posibles posiciones que la ventana puede tener en la pantalla.
         */
        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED, /**< La posición de la ventana no está definida. */
            CENTERED = SDL_WINDOWPOS_CENTERED,   /**< La ventana se centrará en la pantalla. */
        };

        /**
         * @struct OpenGL_Context_Settings
         * @brief Estructura que contiene los detalles del contexto OpenGL.
         *
         * Esta estructura permite configurar los detalles del contexto de OpenGL al crear la ventana.
         */
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

        /**
         * @brief Constructor de la clase `Window`.
         *
         * Este constructor crea una ventana con las especificaciones dadas y configura
         * el contexto OpenGL utilizando los parámetros proporcionados.
         *
         * @param title Título de la ventana.
         * @param left_x Posición en el eje X de la ventana.
         * @param top_y Posición en el eje Y de la ventana.
         * @param width Ancho de la ventana.
         * @param height Alto de la ventana.
         * @param context_details Detalles del contexto OpenGL.
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
         * @brief Constructor de la clase `Window` (versión con título como `const char*`).
         *
         * Este constructor permite crear una ventana con un título de tipo `const char*` y
         * las especificaciones necesarias para el contexto OpenGL.
         *
         * @param title Título de la ventana.
         * @param left_x Posición en el eje X de la ventana.
         * @param top_y Posición en el eje Y de la ventana.
         * @param width Ancho de la ventana.
         * @param height Alto de la ventana.
         * @param context_details Detalles del contexto OpenGL.
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
         * @brief Destructor de la clase `Window`.
         *
         * Este destructor destruye la ventana y limpia el contexto OpenGL asociado.
         */
        ~Window();

    public:

        /**
         * @brief Constructor de copia deshabilitado.
         *
         * Este constructor de copia ha sido deshabilitado para evitar la copia del objeto
         * `Window`, ya que no tiene sentido clonar una ventana.
         */
        Window(const Window&) = delete;

        /**
         * @brief Operador de asignación deshabilitado.
         *
         * El operador de asignación ha sido deshabilitado para evitar la asignación de un objeto
         * `Window` a otro, ya que este tipo de operación no es válida en este contexto.
         */
        Window& operator = (const Window&) = delete;

        /**
         * @brief Constructor de movimiento.
         *
         * Este constructor transfiere la propiedad de la ventana y el contexto OpenGL de un objeto
         * `Window` a otro mediante el uso de `std::exchange`.
         *
         * @param other Objeto `Window` cuyo contenido se moverá.
         */
        Window(Window&& other) noexcept
        {
            this->window_handle = std::exchange(other.window_handle, nullptr);
            this->opengl_context = std::exchange(other.opengl_context, nullptr);
        }

        /**
         * @brief Operador de asignación por movimiento.
         *
         * Este operador transfiere la propiedad de la ventana y el contexto OpenGL de un objeto
         * `Window` a otro mediante el uso de `std::exchange`.
         *
         * @param other Objeto `Window` cuyo contenido se moverá.
         * @return Referencia al objeto `Window` actualizado.
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
         * Este método intercambia los buffers del contexto OpenGL, mostrando el contenido renderizado
         * en el buffer de la ventana.
         */
        void swap_buffers();

        /**
        * @brief Actualiza el movimiento de la cámara.
        * 
        * Este método recibe las entradas de teclado mediante un evento de SDL y ejecuta
        * la función de la clase Camera.
        */
        void move_camera(bool* exit);

        /**
        * @brief Devuelve la cámara.
        * 
        * Este método sirve para que otra clase pueda recibir la cámara de la ventana para 
        pasarla a otro lado, como la escena, por ejemplo.
        */
        Camera get_camera();
    };

}
