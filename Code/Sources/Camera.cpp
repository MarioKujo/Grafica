#include "../Headers/Camera.hpp"

namespace udit
{

    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movement_speed, float mouse_sensitivity)
        : position(position), up(up), yaw(yaw), pitch(pitch), movement_speed(movement_speed), mouse_sensitivity(mouse_sensitivity)
    {
        update_camera_vectors();
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

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

    void Camera::process_mouse_motion(int xrel, int yrel)
    {
        float xoffset = static_cast<float>(xrel) * mouse_sensitivity;
        float yoffset = static_cast<float>(yrel) * mouse_sensitivity;

        yaw += xoffset;
        pitch -= yoffset;

        // Limitar el ángulo vertical
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_camera_vectors();
    }

    void Camera::update_camera_vectors()
    {
        // Calcular el nuevo vector frontal
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);

        // También recalcular los vectores derecha y arriba
        right = glm::normalize(glm::cross(this->front, up)); // Normalizar el vector cruzado
        up = glm::normalize(glm::cross(right, this->front));
    }

}