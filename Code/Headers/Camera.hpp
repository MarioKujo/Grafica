/*@file Camera.hpp
 * @author andrmatgonros@gmail.com
 * @date 2025-01-12
 *
 * Este código es de dominio público.
 *
 * @brief Definición de la clase Camera que permite controlar una cámara 3D.
 *
 * La clase Camera gestiona la posición y orientación de una cámara en un
 * espacio tridimensional. Incluye métodos para mover la cámara mediante teclado
 * y rotarla usando el ratón. También proporciona la matriz de vista actualizada
 * según las transformaciones aplicadas a la cámara.
 */

#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SDL.h>

namespace udit
{
    /**
     * @class Camera
     * @brief Clase que representa una cámara 3D en un espacio tridimensional.
     *
     * La clase permite controlar la posición y orientación de una cámara,
     * manejar el movimiento por teclado y la rotación mediante el ratón.
     * Proporciona métodos para obtener la matriz de vista y actualizar los
     * vectores de la cámara.
     */
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
        /**
         * @brief Constructor de la clase Camera.
         *
         * Inicializa la cámara con una posición, una orientación inicial
         * y los valores de yaw y pitch.
         *
         * @param start_position Posición inicial de la cámara.
         * @param start_up Vector que define la dirección hacia arriba de la cámara.
         * @param start_yaw Ángulo de orientación inicial alrededor del eje Y.
         * @param start_pitch Ángulo de orientación inicial alrededor del eje X.
         */
        Camera(
            glm::vec3 start_position,
            glm::vec3 start_up,
            float start_yaw,
            float start_pitch
        );

        /**
         * @brief Obtiene la matriz de vista de la cámara.
         *
         * La matriz de vista es útil para la proyección de la cámara en el espacio
         * tridimensional.
         *
         * @return glm::mat4 La matriz de vista.
         */
        glm::mat4 get_view_matrix() const;

        /**
         * @brief Procesa el movimiento del teclado para mover la cámara.
         *
         * Este método ajusta la posición de la cámara basándose en las teclas
         * presionadas durante un intervalo de tiempo específico.
         *
         * @param state Puntero a un array que representa el estado de las teclas.
         * @param delta_time Tiempo transcurrido entre fotogramas.
         */
        void process_keyboard(const Uint8* state, float delta_time);

        /**
         * @brief Inicia el control de la cámara mediante entradas de ratón.
         *
         * Este método configura los parámetros necesarios para controlar la cámara
         * usando el ratón.
         */
        void start_camera_control();

        /**
         * @brief Procesa el movimiento del ratón para rotar la cámara.
         *
         * Este método ajusta los ángulos de orientación de la cámara según el movimiento
         * del ratón.
         *
         * @param xrel Desplazamiento horizontal del ratón.
         * @param yrel Desplazamiento vertical del ratón.
         */
        void process_mouse_motion(float xrel, float yrel);

    private:
        /**
         * @brief Actualiza los vectores de dirección de la cámara.
         *
         * Este método recalcula los vectores front, right y up de la cámara
         * según los ángulos de yaw y pitch actuales.
         */
        void update_camera_vectors();
    };
}
