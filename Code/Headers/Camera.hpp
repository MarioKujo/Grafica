#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SDL.h>

namespace udit
{
    /**
     * @class Camera
     * @brief Clase que representa una cámara en un espacio 3D, controlada mediante teclado y ratón.
     *
     * Permite calcular la matriz de vista y moverse/orientarse en un entorno tridimensional usando SDL.
     */
    class Camera
    {
    private:
        glm::vec3 position;         /**< Posición actual de la cámara en el espacio 3D. */
        glm::vec3 front;            /**< Vector de dirección hacia donde la cámara está mirando. */
        glm::vec3 up;               /**< Vector hacia arriba relativo a la cámara. */
        glm::vec3 right;            /**< Vector hacia la derecha relativo a la cámara. */
        glm::vec3 world_up;         /**< Vector global hacia arriba (normalmente (0,1,0)). */

        float yaw;                  /**< Ángulo de rotación horizontal (eje Y). */
        float pitch;                /**< Ángulo de rotación vertical (eje X). */
        float movement_speed;       /**< Velocidad de desplazamiento de la cámara. */
        float mouse_sensitivity;    /**< Sensibilidad de rotación con el movimiento del ratón. */

    public:
        /**
         * @brief Constructor con parámetros para inicializar la cámara con valores específicos.
         *
         * @param start_position Posición inicial de la cámara.
         * @param start_up Vector de orientación inicial hacia arriba.
         * @param start_yaw Valor inicial del ángulo yaw.
         * @param start_pitch Valor inicial del ángulo pitch.
         */
        Camera(
            glm::vec3 start_position,
            glm::vec3 start_up,
            float start_yaw,
            float start_pitch
        );

        /**
         * @brief Constructor por defecto. Inicializa la cámara con valores neutros.
         */
        Camera();

        /**
         * @brief Genera y retorna la matriz de vista de la cámara.
         *
         * @return glm::mat4 Matriz de vista utilizada para transformar el mundo en coordenadas de cámara.
         */
        glm::mat4 get_view_matrix() const;

        /**
         * @brief Obtiene la posición actual de la cámara en el espacio.
         *
         * @return glm::vec3 Posición actual.
         */
        glm::vec3 get_position() const;

        /**
         * @brief Procesa la entrada del teclado para mover la cámara.
         *
         * @param state Puntero al estado actual del teclado proporcionado por SDL_GetKeyboardState().
         */
        void process_keyboard(const Uint8* state);

        /**
         * @brief Configura el control de la cámara activando el modo de ratón relativo.
         *
         * Oculta el cursor y permite capturar el movimiento del ratón.
         */
        void start_camera_control();

        /**
         * @brief Procesa el movimiento del ratón para rotar la cámara.
         *
         * @param xrel Movimiento relativo en el eje X del ratón.
         * @param yrel Movimiento relativo en el eje Y del ratón.
         */
        void process_mouse_motion(float xrel, float yrel);

    private:
        /**
         * @brief Actualiza los vectores de orientación de la cámara (front, right, up).
         *
         * Este método debe llamarse cada vez que cambien los ángulos yaw o pitch.
         */
        void update_camera_vectors();
    };
}
