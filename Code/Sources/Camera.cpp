/**
 * @file Camera.cpp
 * @brief Implementación de la clase `Camera`, que gestiona la cámara para el movimiento y rotación
 *        en un entorno 3D usando entradas del teclado y del ratón.
 *
 * @author Andrés González
 * @date 2025
 * @note Public domain
 */

#include "../Headers/Camera.hpp"

namespace udit
{
    /**
     * @brief Constructor de la clase `Camera`.
     *
     * Inicializa los parámetros de la cámara como la posición, dirección de movimiento, velocidad
     * de movimiento, sensibilidad del ratón, entre otros. Después de la inicialización, actualiza
     * los vectores de la cámara.
     *
     * @param position Posición inicial de la cámara en el espacio 3D.
     * @param up Vector que define la dirección "arriba" de la cámara.
     * @param yaw Ángulo de rotación horizontal de la cámara.
     * @param pitch Ángulo de rotación vertical de la cámara.
     * @param movement_speed Velocidad de movimiento de la cámara.
     * @param mouse_sensitivity Sensibilidad del ratón para ajustar el movimiento de la cámara.
     */
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), up(up), yaw(yaw), pitch(pitch), movement_speed(2.5f), mouse_sensitivity(0.1f)
    {
        start_camera_control();
        update_camera_vectors();
    }

    /**
     * @brief Devuelve la matriz de vista de la cámara.
     *
     * Calcula y retorna la matriz de vista (view matrix) de la cámara utilizando la posición, la
     * dirección de la cámara (front), y el vector "up".
     *
     * @return glm::mat4 La matriz de vista calculada.
     */
    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    /**
     * @brief Procesa la entrada del teclado para mover la cámara.
     *
     * Este método ajusta la posición de la cámara en función de las teclas presionadas. La cámara
     * se mueve hacia adelante, atrás, izquierda o derecha basándose en la entrada del teclado y
     * la velocidad de movimiento.
     *
     * @param keyboard_state Estado actual del teclado (una matriz de teclas presionadas).
     * @param delta_time El tiempo transcurrido entre el último frame y el frame actual, utilizado
     *                   para ajustar el movimiento a la tasa de refresco.
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

    void Camera::start_camera_control()
    {
        // Capturar el ratón y ocultarlo
        SDL_SetRelativeMouseMode(SDL_TRUE);  // El ratón no puede salir de la ventana
        SDL_ShowCursor(SDL_FALSE);           // Ocultar el ratón
    }
    /**
     * @brief Procesa el movimiento del ratón para actualizar la orientación de la cámara.
     *
     * Este método ajusta los ángulos de rotación de la cámara (yaw y pitch) en función del movimiento
     * del ratón. El movimiento relativo del ratón (xrel, yrel) es utilizado para cambiar los valores
     * de los ángulos de la cámara, afectando su orientación en el espacio 3D.
     *
     * @param xrel Movimiento del ratón en el eje X.
     * @param yrel Movimiento del ratón en el eje Y.
     */
    void Camera::process_mouse_motion(float xrel, float yrel)
    {
        xrel *= mouse_sensitivity;
        yrel *= mouse_sensitivity;

        yaw += xrel;
        pitch -= yrel;

        // Limitar el ángulo vertical (pitch) para evitar que la cámara se invierta
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Actualizar los vectores de la cámara
        update_camera_vectors();
    }

    /**
     * @brief Actualiza los vectores de la cámara (front, right, up) basados en los ángulos de rotación.
     *
     * Este método recalcula el vector frontal (front), el vector derecho (right) y el vector hacia arriba
     * (up) de la cámara, basándose en los ángulos de rotación actuales de la cámara (yaw, pitch). Los vectores
     * son normalizados para asegurar una dirección adecuada.
     */
    void Camera::update_camera_vectors()
    {
        // Calcular el nuevo vector frontal
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        // Asegurarse de que el vector "up" global no se vea afectado por las rotaciones
        glm::vec3 world_up(0.0f, 1.0f, 0.0f);  // Fijo, siempre apunta hacia arriba en el espacio global

        // Calcular el vector derecho (right) usando un "up" global fijo
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));  // Recalcular el vector "up" basándose en el "right"
    }

}
