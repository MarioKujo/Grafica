#pragma once

#include <SDL.h>
#include <string>
#include <utility>
#include "Camera.hpp"

namespace udit
{
    /**
     * @class Window
     * @brief Clase que encapsula una ventana con soporte para contexto OpenGL y control de cámara.
     *
     * Esta clase se encarga de crear y manejar una ventana SDL, configurar el contexto de OpenGL,
     * controlar el ciclo de eventos básicos (ratón y teclado) y actualizar la cámara asociada.
     */
    class Window
    {
    public:
        /**
         * @brief Enumeración para definir la posición inicial de la ventana.
         */
        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED, /**< La posición de la ventana no está definida. */
            CENTERED = SDL_WINDOWPOS_CENTERED,   /**< La ventana se centrará en la pantalla. */
        };

        /**
         * @struct OpenGL_Context_Settings
         * @brief Estructura para configurar los parámetros del contexto de OpenGL.
         */
        struct OpenGL_Context_Settings
        {
            unsigned version_major = 3;          /**< Versión mayor del contexto OpenGL. */
            unsigned version_minor = 3;          /**< Versión menor del contexto OpenGL. */
            bool     core_profile = true;        /**< Indica si se debe usar el perfil core. */
            unsigned depth_buffer_size = 24;     /**< Tamaño del buffer de profundidad. */
            unsigned stencil_buffer_size = 0;    /**< Tamaño del buffer de stencil. */
            bool     enable_vsync = true;        /**< Si se debe activar V-Sync. */
        };

    private:
        SDL_Window* window_handle;     /**< Manejador de la ventana SDL. */
        SDL_GLContext opengl_context;  /**< Contexto OpenGL asociado con la ventana. */
        Camera camera;                 /**< Cámara utilizada para navegar la escena 3D. */

    public:

        /**
         * @brief Constructor alternativo usando `const char*` como título.
         */
        Window(
            const char* title,
            int      left_x,
            int      top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings& context_details
        );

        /**
         * @brief Destructor de la ventana. Libera el contexto y recursos SDL.
         */
        ~Window();

        /// @brief Constructor de copia eliminado (no se permite copiar ventanas).
        Window(const Window&) = delete;

        /// @brief Operador de copia eliminado.
        Window& operator = (const Window&) = delete;

        /**
         * @brief Intercambia los buffers del contexto OpenGL.
         *
         * Este método debe llamarse al final de cada frame para mostrar el contenido renderizado.
         */
        void swap_buffers();

        void poll_input_events(bool* exit);

        /**
         * @brief Procesa eventos de entrada para mover la cámara o salir.
         * @param exit Puntero a un booleano que será puesto en true si se solicita salir (evento SDL_QUIT).
         */
        void move_camera(bool* exit);

        /**
         * @brief Obtiene una copia de la cámara actual.
         * @return Objeto Camera con la configuración actual.
         */
        Camera get_camera();
    };

}
