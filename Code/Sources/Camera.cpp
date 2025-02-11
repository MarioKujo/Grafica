// Este código es de dominio público
// andrmatgonros@gmail.com
#include "../Headers/Camera.hpp"

namespace udit
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : position(position), up(up), yaw(yaw), pitch(pitch), movement_speed(2.5f), mouse_sensitivity(0.1f)
    {
        start_camera_control();
        update_camera_vectors(); // Calcula los vectores iniciales de la cámara.
    }

    Camera::Camera(): position(glm::vec3(0.f, 0.f, 0.f)), up(glm::vec3(0.f, 0.f, 0.f)), yaw (-90.f), pitch(0.f)
    {
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        // Genera la matriz de vista utilizando la posición, dirección (front) y orientación (up).
        return glm::lookAt(position, position + front, up);
    }

    void Camera::process_keyboard(SDL_Scancode scancode)
	{
		switch (scancode)
		{
		    case SDL_SCANCODE_W:
		    {
			    position += front * movement_speed;
			    break;
		    }
		    case SDL_SCANCODE_S:
		    {
			    position -= front * movement_speed;
			    break;
		    }
		    case SDL_SCANCODE_A:
		    {
			    position -= right * movement_speed;
			    break;
		    }
		    case SDL_SCANCODE_D:
		    {
			    position += right * movement_speed;
			    break;
		    }
		}
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
