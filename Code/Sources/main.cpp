// Este código es de dominio público
// angel.rodriguez@udit.es

#include "../Headers/Scene.hpp"
#include <Window.hpp>

using udit::Scene;
using udit::Window;

int main(int argc, char* argv[])
{
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;

    Window window
    (
        "Andrés Matías González Ros - Práctica Final",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height,
        { 3, 3 }
    );

    Scene scene(viewport_width, viewport_height);

    bool exit = false;
    Uint32 last_time = SDL_GetTicks();
    do
    {
        SDL_Event event;

        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true;
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                scene.handle_mouse_motion((float)event.motion.xrel, (float)event.motion.yrel);
            }
        }

        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        scene.update(delta_time);

        scene.render();

        window.swap_buffers();
    } while (not exit);

    SDL_Quit();

    return 0;
}
