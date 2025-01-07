/**
 * @file main.cpp
 * @brief Ejemplo de uso de OpenGL con SDL2 para renderizar una escena básica.
 *
 * Este código implementa una ventana OpenGL y maneja eventos básicos de SDL2.
 * Permite procesar eventos del ratón, actualizar una escena y renderizarla.
 *
 * @author Angel Rodriguez
 * @note Public domain
 */

#include "../Headers/Scene.hpp"
#include <Window.hpp>

using udit::Scene;
using udit::Window;

/**
 * @brief Función principal del programa.
 *
 * Configura una ventana con OpenGL, inicializa una escena y entra en un bucle principal
 * donde se procesan eventos, se actualiza la lógica y se renderiza.
 *
 * @param argc Número de argumentos de línea de comandos (no utilizado).
 * @param argv Argumentos de línea de comandos (no utilizado).
 * @return Código de salida del programa.
 */
int main(int argc, char* argv[])
{
    // Dimensiones del viewport
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;

    /**
     * @brief Creación de la ventana principal.
     *
     * Se configura la posición centrada, el tamaño del viewport y la versión
     * de OpenGL a utilizar (3.3).
     */
    Window window
    (
        "OpenGL example",                ///< Título de la ventana.
        Window::Position::CENTERED,       ///< Posición X de la ventana.
        Window::Position::CENTERED,       ///< Posición Y de la ventana.
        viewport_width,                   ///< Ancho del viewport.
        viewport_height,                  ///< Alto del viewport.
        { 3, 3 }                          ///< Versión de OpenGL (mayor, menor).
    );

    /**
     * @brief Creación de la escena.
     *
     * Inicializa la escena con el tamaño del viewport especificado.
     */
    Scene scene(viewport_width, viewport_height);

    bool exit = false; ///< Bandera para controlar el bucle principal.

    do
    {
        // Procesamiento de eventos acumulados:

        SDL_Event event; ///< Estructura para eventos de SDL2.

        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true; ///< Se establece la bandera de salida si se solicita cerrar la ventana.
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                /**
                 * @brief Manejo del movimiento del ratón.
                 *
                 * Pasa los desplazamientos relativos del ratón a la escena.
                 */
                scene.handle_mouse_motion(event.motion.xrel, event.motion.yrel);
            }
        }

        // Actualización de la escena:

        float delta_time = 0.016f; ///< Aproximación básica para un intervalo de 60 FPS.
        scene.update(delta_time); ///< Actualiza la lógica de la escena.

        // Renderizado de la escena:

        scene.render(); ///< Renderiza la escena en el contexto actual.

        // Actualización del contenido de la ventana:

        window.swap_buffers(); ///< Intercambia los buffers para mostrar el contenido renderizado.
    } while (not exit); ///< Bucle principal, se repite hasta que exit sea verdadero.

    SDL_Quit(); ///< Limpieza y salida de SDL2.

    return 0; ///< Fin del programa.
}
