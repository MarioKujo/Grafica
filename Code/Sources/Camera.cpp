/*@file Camera.cpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Implementación de la clase Camera.
 *
 * Este archivo contiene la implementación de los métodos de la clase Camera, que
 * permite controlar una cámara en un espacio tridimensional. Los métodos permiten
 * mover la cámara mediante el teclado, rotarla usando el ratón y actualizar los
 * vectores de dirección de la cámara para mantener la orientación correcta en el
 * espacio 3D.
 */

#include "../Headers/Camera.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase Camera.
     *
     * Inicializa la posición, orientación, velocidad de movimiento y sensibilidad
     * del ratón de la cámara. También inicia el control del ratón y actualiza los
     * vectores de la cámara.
     *
     * @param position Posición inicial de la cámara en el espacio 3D.
     * @param up Vector que define la dirección hacia arriba de la cámara.
     * @param yaw Ángulo de orientación inicial alrededor del eje Y (derecha-izquierda).
     * @param pitch Ángulo de orientación inicial alrededor del eje X (arriba-abajo).
     */
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), up(up), yaw(yaw), pitch(pitch), movement_speed(2.5f), mouse_sensitivity(0.1f)
    {
        start_camera_control();
        update_camera_vectors();
    }

    /**
     * @brief Obtiene la matriz de vista de la cámara.
     *
     * La matriz de vista se utiliza para posicionar la cámara correctamente en el
     * mundo 3D y obtener una perspectiva adecuada.
     *
     * @return glm::mat4 Matriz de vista calculada a partir de la posición, dirección
     * y orientación de la cámara.
     */
    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    /**
     * @brief Procesa el movimiento del teclado para mover la cámara.
     *
     * Este método ajusta la posición de la cámara en función de las teclas presionadas
     * (W, A, S, D) y el tiempo transcurrido entre fotogramas.
     *
     * @param keyboard_state Estado actual de las teclas (teclas presionadas o no).
     * @param delta_time Tiempo transcurrido entre fotogramas.
     */
    void Camera::process_keyboard(const Uint8* keyboard_state, float delta_time)
    {
        float velocity = movement_speed * delta_time;

        if (keyboard_state[SDL_SCANCODE_W])
            position += front * velocity;
        if (keyboard_state[SDL_SCANCODE_S])
            position -= front * velocity;
        if (keyboard_state[SDL_SCANCODE_A])
            position -= right * velocity;
        if (keyboard_state[SDL_SCANCODE_D])
            position += right * velocity;
    }

    /**
     * @brief Inicia el control de la cámara mediante el ratón.
     *
     * Este método configura el ratón en modo relativo y oculta el cursor para que
     * el movimiento del ratón pueda ser utilizado para rotar la cámara.
     */
    void Camera::start_camera_control()
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_ShowCursor(SDL_FALSE);
    }

    /**
     * @brief Procesa el movimiento del ratón para rotar la cámara.
     *
     * Este método ajusta los ángulos de yaw y pitch de la cámara en función del
     * movimiento del ratón, permitiendo rotar la cámara en el espacio 3D.
     *
     * @param xrel Desplazamiento horizontal del ratón.
     * @param yrel Desplazamiento vertical del ratón.
     */
    void Camera::process_mouse_motion(float xrel, float yrel)
    {
        xrel *= mouse_sensitivity;
        yrel *= mouse_sensitivity;

        yaw += xrel;
        pitch -= yrel;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_camera_vectors();
    }

    /**
     * @brief Actualiza los vectores de dirección de la cámara.
     *
     * Este método recalcula los vectores de dirección (frente, derecha y arriba)
     * de la cámara según los ángulos actuales de yaw y pitch.
     */
    void Camera::update_camera_vectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        glm::vec3 world_up(0.0f, 1.0f, 0.0f);

        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }

}
