/* @file Camera.cpp
* 
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 * 
 * Este código es de dominio público.
 */
#include "../Headers/Camera.hpp"

namespace udit
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), up(up), yaw(yaw), pitch(pitch), movement_speed(2.5f), mouse_sensitivity(0.1f)
    {
        start_camera_control();
        update_camera_vectors(); // Calcula los vectores iniciales de la cámara.
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        // Genera la matriz de vista utilizando la posición, dirección (front) y orientación (up).
        return glm::lookAt(position, position + front, up);
    }

    void Camera::process_keyboard(const Uint8* keyboard_state, float delta_time)
    {
        float velocity = movement_speed * delta_time;

        // Mueve la cámara según las teclas presionadas.
        if (keyboard_state[SDL_SCANCODE_W])
            position += front * velocity;
        if (keyboard_state[SDL_SCANCODE_S])
            position -= front * velocity;
        if (keyboard_state[SDL_SCANCODE_A])
            position -= right * velocity;
        if (keyboard_state[SDL_SCANCODE_D])
            position += right * velocity;
    }

    void Camera::start_camera_control()
    {
        // Configura el ratón en modo relativo y oculta el cursor.
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_ShowCursor(SDL_FALSE);
    }

    void Camera::process_mouse_motion(float xrel, float yrel)
    {
        // Ajusta los ángulos de orientación según el movimiento del ratón.
        xrel *= mouse_sensitivity;
        yrel *= mouse_sensitivity;

        yaw += xrel;
        pitch -= yrel;

        // Limita el pitch para evitar "gimbal lock".
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_camera_vectors();
    }

    void Camera::update_camera_vectors()
    {
        // Calcula el nuevo vector de dirección (front) basado en yaw y pitch.
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        // Recalcula los vectores right y up para mantener la orientación de la cámara.
        glm::vec3 world_up(0.0f, 1.0f, 0.0f);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
}
