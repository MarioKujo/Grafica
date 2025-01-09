/**
 * @file Camera.hpp
 * @brief Declaración de la clase `Camera`, que maneja la posición, orientación y movimiento de la cámara
 *        en un entorno 3D utilizando entradas de teclado y ratón.
 *
 * @author Andrés González
 * @date 2025
 * @note Public domain
 */

#pragma once

#include <glm.hpp> // vec3, mat4
#include <gtc/matrix_transform.hpp> // translate, rotate, lookAt
#include <SDL.h>

namespace udit
{
    /**
     * @class Camera
     * @brief Clase que representa una cámara en un entorno 3D con movimiento y rotación.
     *
     * Esta clase gestiona la posición, orientación, y el movimiento de la cámara. Permite el desplazamiento
     * a través del teclado y la rotación mediante el movimiento del ratón. La cámara utiliza un sistema de
     * coordenadas basado en el eje Y como "arriba" y el eje Z negativo como "frente".
     */
    class Camera
    {
    private:
        glm::vec3 position; /**< Posición actual de la cámara en el espacio 3D. */
        glm::vec3 front; /**< Dirección hacia donde mira la cámara. */
        glm::vec3 up; /**< Dirección "arriba" de la cámara. */
        glm::vec3 right; /**< Dirección "derecha" de la cámara, perpendicular a 'front' y 'up'. */
        glm::vec3 world_up; /**< Dirección "arriba" del mundo (generalmente el eje Y global). */

        float yaw; /**< Ángulo de rotación de la cámara alrededor del eje Y (horizontal). */
        float pitch; /**< Ángulo de rotación de la cámara alrededor del eje X (vertical). */
        float movement_speed; /**< Velocidad de movimiento de la cámara. */
        float mouse_sensitivity; /**< Sensibilidad del ratón para la rotación de la cámara. */

    public:
        /**
         * @brief Constructor de la clase `Camera`.
         *
         * Inicializa la cámara con una posición, orientación (yaw, pitch), velocidad de movimiento y sensibilidad
         * del ratón especificados. Luego, llama a `update_camera_vectors()` para calcular los vectores derivados
         * de la orientación actual de la cámara.
         *
         * @param start_position Posición inicial de la cámara (por defecto en (0, 0, 3)).
         * @param start_up Dirección "arriba" inicial de la cámara (por defecto en (0, 1, 0)).
         * @param start_yaw Ángulo de rotación inicial horizontal de la cámara (por defecto en -90.0f).
         * @param start_pitch Ángulo de rotación inicial vertical de la cámara (por defecto en 0.0f).
         * @param speed Velocidad de movimiento inicial de la cámara (por defecto en 2.5f).
         * @param sensitivity Sensibilidad inicial del ratón (por defecto en 0.1f).
         */
        Camera(
            glm::vec3 start_position = glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3 start_up = glm::vec3(0.0f, 1.0f, 0.0f),
            float start_yaw = -90.0f,
            float start_pitch = 0.0f,
            float speed = 2.5f,
            float sensitivity = 0.1f
        );

        /**
         * @brief Calcula y retorna la matriz de vista de la cámara.
         *
         * La matriz de vista es calculada utilizando la función `glm::lookAt`, basada en la posición,
         * la dirección "frontal" de la cámara y el vector "arriba".
         *
         * @return glm::mat4 La matriz de vista calculada.
         */
        glm::mat4 get_view_matrix() const;

        /**
         * @brief Procesa la entrada del teclado para mover la cámara.
         *
         * Este método ajusta la posición de la cámara basándose en las teclas presionadas en el teclado.
         * El movimiento de la cámara se realiza en las direcciones "frontal" (W/S) y "derecha" (A/D),
         * con una velocidad ajustada por `movement_speed` y el tiempo entre frames (`delta_time`).
         *
         * @param state Estado actual de las teclas (teclas presionadas).
         * @param delta_time El tiempo transcurrido entre el último frame y el frame actual, utilizado
         *                   para hacer el movimiento independiente de la tasa de refresco.
         */
        void process_keyboard(const Uint8* state, float delta_time);

        void start_camera_control();

        /**
         * @brief Procesa el movimiento del ratón para actualizar la orientación de la cámara.
         *
         * Este método ajusta los ángulos de rotación de la cámara (yaw, pitch) en función del movimiento
         * relativo del ratón. Los movimientos del ratón cambian la orientación de la cámara en el espacio 3D.
         *
         * @param xrel Movimiento del ratón en el eje X.
         * @param yrel Movimiento del ratón en el eje Y.
         */
        void process_mouse_motion(int xrel, int yrel);

    private:
        /**
         * @brief Actualiza los vectores de la cámara (front, right, up) basados en los ángulos de rotación.
         *
         * Este método recalcula los vectores derivados de la orientación actual de la cámara, tales como
         * el vector "frontal" (front), el vector "derecha" (right) y el vector "arriba" (up), y los normaliza.
         */
        void update_camera_vectors();
    };
}
