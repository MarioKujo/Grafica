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
    do
    {
        scene.update(); ///< Actualiza la escena.

        scene.render(); ///< Renderiza la escena.

        window.swap_buffers(); ///< Intercambia los buffers para mostrar la imagen renderizada.
        window.move_camera(&exit); ///< Mueve la cámara
        scene.set_camera(window.get_camera()); ///< La cámara de la escena recibe los valores de la cámara de la ventana
    } while (not exit);

    SDL_Quit(); ///< Finaliza SDL al salir del bucle.

    return 0;
}
