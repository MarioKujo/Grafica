#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

namespace udit
{
    /**
     * @brief Clase para manejar programas de sombreado (shaders) en OpenGL.
     *
     * Permite compilar, enlazar y usar shaders de vértices y fragmentos, así como subir uniforms.
     */
    class ShaderProgram
    {
    private:
        GLuint program_id; /**< Identificador del programa de shaders en OpenGL. */

        mutable std::unordered_map<std::string, GLint> uniform_locations; /**< Cache de ubicaciones de uniforms para optimizar llamadas. */
        /**
         * @brief Muestra un error de compilación de un shader.
         * @param shader_id ID del shader que falló al compilar.
         */
        void show_compilation_error(GLuint shader_id) const;

        /**
         * @brief Muestra un error de enlace del programa.
         * @param program_id ID del programa que falló al enlazar.
         */
        void show_linkage_error(GLuint program_id) const;

        /**
        * @brief Obtiene la ubicación de un uniform en el programa, cacheándola para futuros accesos.
        * @param name Nombre del uniform.
        * @return Ubicación del uniform, o -1 si no existe.
        */
        GLint getUniformLocation(const std::string& name) const;

    public:
        /**
         * @brief Constructor que compila y enlaza los shaders.
         * @param vertex_source Código fuente del shader de vértices.
         * @param fragment_source Código fuente del shader de fragmentos.
         */
        ShaderProgram(const std::string& vertex_source, const std::string& fragment_source);

        /**
         * @brief Destructor. Libera el programa de OpenGL.
         */
        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;  /**< No se permite copiar el programa. */
        ShaderProgram& operator=(const ShaderProgram&) = delete; /**< No se permite asignar por copia. */

        /**
        * @brief Constructor de movimiento.
        * @param other Objeto ShaderProgram que será movido.
        */
        ShaderProgram(ShaderProgram&& other) noexcept;

        /**
        * @brief Operador de asignación por movimiento.
        * @param other Objeto ShaderProgram que será movido.
        * @return Referencia al objeto actual.
        */
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;
        /**
         * @brief Activa el programa para su uso en el pipeline de OpenGL.
         */
        void use() const;

        /**
         * @brief Obtiene el ID del programa en OpenGL.
         * @return ID del programa.
         */
        GLuint id() const { return program_id; }

        /**
         * @brief Sube una matriz 4x4 a un uniform del shader.
         * @param name Nombre del uniform.
         * @param mat Matriz a subir.
         */
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        /**
         * @brief Sube un vector 3D a un uniform del shader.
         * @param name Nombre del uniform.
         * @param vec Vector a subir.
         */
        void setVec3(const std::string& name, const glm::vec3& vec) const;

        /**
         * @brief Sube un valor float a un uniform del shader.
         * @param name Nombre del uniform.
         * @param value Valor flotante a subir.
         */
        void setFloat(const std::string& name, float value) const;

        /**
         * @brief Sube un valor entero a un uniform del shader.
         * @param name Nombre del uniform.
         * @param value Valor entero a subir.
         */
        void setInt(const std::string& name, int value) const;
    };
}
