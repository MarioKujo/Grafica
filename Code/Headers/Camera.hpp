#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SDL.h>

namespace udit
{
    class Camera
    {
    private:
        glm::vec3 position; /**< Posición de la cámara en el espacio 3D. */
        glm::vec3 front; /**< Dirección hacia donde la cámara está mirando. */
        glm::vec3 up; /**< Vector que define la orientación hacia arriba de la cámara. */
        glm::vec3 right; /**< Vector que define la dirección hacia la derecha de la cámara. */
        glm::vec3 world_up; /**< Vector global de orientación hacia arriba (en el mundo). */

        float yaw; /**< Ángulo de orientación de la cámara alrededor del eje Y (derecha-izquierda). */
        float pitch; /**< Ángulo de orientación de la cámara alrededor del eje X (arriba-abajo). */
        float movement_speed; /**< Velocidad de movimiento de la cámara. */
        float mouse_sensitivity; /**< Sensibilidad del ratón para la rotación de la cámara. */

    public:
        Camera(
            glm::vec3 start_position,
            glm::vec3 start_up,
            float start_yaw,
            float start_pitch
        );
        Camera();

        glm::mat4 get_view_matrix() const;

        glm::vec3 get_position() const;

        void process_keyboard(const Uint8* state);

        void start_camera_control();

        void process_mouse_motion(float xrel, float yrel);

    private:

        void update_camera_vectors();
    };
}
