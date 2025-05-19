// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Camera.hpp"

namespace udit
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position),
        world_up(up),
        yaw(yaw),
        pitch(pitch),
        movement_speed(1.f),
        mouse_sensitivity(0.1f)
    {
        start_camera_control();     // Activa el modo de control con ratón relativo
        update_camera_vectors();    // Inicializa front, right y up con los ángulos dados
    }

    Camera::Camera()
        : position(0.f),
        front(0.f, 0.f, -1.f),
        up(0.f, 1.f, 0.f),
        right(1.f, 0.f, 0.f),
        world_up(0.f, 1.f, 0.f),
        yaw(-90.f),
        pitch(0.f),
        movement_speed(1.f),
        mouse_sensitivity(0.1f)
    {
        update_camera_vectors();    // Inicializa los vectores en base a los ángulos por defecto
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        // Retorna la matriz de vista calculada a partir de posición, dirección y orientación
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 Camera::get_position() const
    {
        return position;
    }

    void Camera::process_keyboard(const Uint8* state)
    {
        // Movimiento hacia adelante/atrás
        if (state[SDL_SCANCODE_W])
            position += front * movement_speed;
        if (state[SDL_SCANCODE_S])
            position -= front * movement_speed;

        // Movimiento lateral izquierda/derecha
        if (state[SDL_SCANCODE_A])
            position -= right * movement_speed;
        if (state[SDL_SCANCODE_D])
            position += right * movement_speed;
    }

    void Camera::start_camera_control()
    {
        // Habilita el modo relativo del mouse y oculta el cursor
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_ShowCursor(SDL_FALSE);
    }

    void Camera::process_mouse_motion(float xrel, float yrel)
    {
        // Ajusta la sensibilidad del movimiento del mouse
        xrel *= mouse_sensitivity;
        yrel *= mouse_sensitivity;

        // Modifica los ángulos yaw y pitch con el movimiento
        yaw += xrel;
        pitch -= yrel;

        // Limita el pitch para evitar giros verticales excesivos (gimbal lock)
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        // Actualiza los vectores front, right y up según los nuevos ángulos
        update_camera_vectors();
    }

    void Camera::update_camera_vectors()
    {
        // Calcula el nuevo vector frontal desde yaw y pitch
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);

        // Calcula los vectores right y up a partir de front y world_up
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
}
