/* @file main.cpp
 * @author Original Author <angel.rodriguez@udit.es>
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Función principal para inicializar y ejecutar la escena 3D.
 *
 * Esta función inicializa la ventana de la aplicación, configura la escena, y ejecuta el ciclo principal del programa.
 * Durante la ejecución, se actualiza la escena, se renderiza y se manejan los eventos del ratón, así como el cálculo del tiempo entre fotogramas.
 *
 * @note Modificado por andrmatgonros@gmail.com para incluir:
 *       - Cálculo del tiempo entre fotogramas (`delta_time`).
 *       - Manejo del movimiento del ratón para controlar la cámara en la escena.
 *       - Cambio del nombre de la ventana a "Andrés Matías González Ros - Práctica Final".
 */
#include "../Headers/Scene.hpp"
#include "../Headers/Window.hpp"

using udit::Scene;
using udit::Window;

int main(int argc, char* argv[])
{
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;

    // Inicializa la ventana con un nuevo nombre "Práctica Final"
    Window window
    (
        "Andrés Matías González Ros - Práctica Final",  ///< Nombre de la ventana.
        Window::Position::CENTERED, ///< Posición centrada de la ventana en la pantalla.
        Window::Position::CENTERED, ///< Posición centrada de la ventana en la pantalla.
        viewport_width, ///< Ancho de la ventana.
        viewport_height, ///< Alto de la ventana.
        { 3, 3 } ///< Versión de OpenGL (3.3).
    );

    Scene scene(viewport_width, viewport_height); ///< Crea la escena 3D con el tamaño de la ventana.

    bool exit = false; ///< Flag para determinar si el programa debe salir.
    Uint32 last_time = SDL_GetTicks(); ///< Tiempo del último fotograma.
    do
    {
        SDL_Event event;

        // Bucle de eventos, captura y maneja los eventos del sistema.
        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true; ///< Salir cuando el evento sea un cierre de ventana.
            }
        }
        scene.update(); ///< Actualiza la escena.

        scene.render(); ///< Renderiza la escena.

        window.swap_buffers(); ///< Intercambia los buffers para mostrar la imagen renderizada.
    } while (not exit);

    SDL_Quit(); ///< Finaliza SDL al salir del bucle.

    return 0;
}
