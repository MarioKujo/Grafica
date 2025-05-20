#include "../Headers/ShaderProgram.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

static std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

namespace udit {

    ShaderProgram::ShaderProgram(const std::string& vertex_path, const std::string& fragment_path) {
        std::string vertex_source = readFile(vertex_path);
        std::string fragment_source = readFile(fragment_path);

        GLint success = GL_FALSE;

        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_code = vertex_source.c_str();
        const char* fragment_code = fragment_source.c_str();

        // Compilar shader de vértice
        glShaderSource(vertex_shader, 1, &vertex_code, nullptr);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            show_compilation_error(vertex_shader);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            throw std::runtime_error("Vertex shader compilation failed.");
        }

        // Compilar shader de fragmento
        glShaderSource(fragment_shader, 1, &fragment_code, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            show_compilation_error(fragment_shader);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            throw std::runtime_error("Fragment shader compilation failed.");
        }

        // Crear programa y enlazar shaders
        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader);
        glAttachShader(program_id, fragment_shader);
        glLinkProgram(program_id);

        // Verificar enlace del programa
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            show_linkage_error(program_id);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            glDeleteProgram(program_id);
            program_id = 0;
            throw std::runtime_error("Shader program linking failed.");
        }

        // Borrar shaders ya enlazados
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }


    // Destructor libera el programa si existe
    ShaderProgram::~ShaderProgram() {
        if (program_id != 0)
            glDeleteProgram(program_id);
    }

    // Constructor move: transfiere propiedad del programa y cache de uniforms
    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
        : program_id(other.program_id), uniform_locations(std::move(other.uniform_locations))
    {
        other.program_id = 0;
    }

    // Operador de asignación move: limpia programa actual y transfiere el nuevo
    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            if (program_id != 0)
                glDeleteProgram(program_id);

            program_id = other.program_id;
            uniform_locations = std::move(other.uniform_locations);

            other.program_id = 0;
        }
        return *this;
    }

    // Activa el programa para usarlo en OpenGL
    void ShaderProgram::use() const {
        if (program_id != 0)
            glUseProgram(program_id);
    }

    // Busca la ubicación del uniform y cachea el resultado
    GLint ShaderProgram::getUniformLocation(const std::string& name) const
    {
        auto it = uniform_locations.find(name);
        if (it != uniform_locations.end())
            return it->second;

        GLint location = glGetUniformLocation(program_id, name.c_str());
        uniform_locations[name] = location;
        return location;
    }

    // Sube matriz 4x4 al uniform
    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    // Sube vector 3D al uniform
    void ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1)
            glUniform3fv(loc, 1, glm::value_ptr(vec));
    }

    // Sube float al uniform
    void ShaderProgram::setFloat(const std::string& name, float value) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1)
            glUniform1f(loc, value);
    }

    // Sube entero al uniform
    void ShaderProgram::setInt(const std::string& name, int value) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1)
            glUniform1i(loc, value);
    }

    // Muestra errores de compilación del shader
    void ShaderProgram::show_compilation_error(GLuint shader_id) const {
        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            std::string info_log(log_length, ' ');
            glGetShaderInfoLog(shader_id, log_length, nullptr, &info_log[0]);
            std::cerr << "Shader compilation error:\n" << info_log << std::endl;
        }
    }

    // Muestra errores de enlace del programa
    void ShaderProgram::show_linkage_error(GLuint program_id) const {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            std::string info_log(log_length, ' ');
            glGetProgramInfoLog(program_id, log_length, nullptr, &info_log[0]);
            std::cerr << "Shader program linkage error:\n" << info_log << std::endl;
        }
    }
}
