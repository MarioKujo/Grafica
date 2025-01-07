#pragma once

#include <glm.hpp> // vec3, mat4
#include <gtc/matrix_transform.hpp> // translate, rotate, lookAt
#include <SDL.h>

namespace udit
{
    class Camera
    {
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        float yaw;
        float pitch;
        float movement_speed;
        float mouse_sensitivity;

    public:
        Camera(
            glm::vec3 start_position = glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3 start_up = glm::vec3(0.0f, 1.0f, 0.0f),
            float start_yaw = -90.0f,
            float start_pitch = 0.0f,
            float speed = 2.5f,
            float sensitivity = 0.1f
        );

        glm::mat4 get_view_matrix() const;

        void process_keyboard(const Uint8* state, float delta_time);
        void process_mouse_motion(int xrel, int yrel);

    private:
        void update_camera_vectors();
    };
}