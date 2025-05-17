#include "../Headers/ShaderProgram.hpp"
#include <fstream>
#include <sstream>

namespace udit {

    ShaderProgram::ShaderProgram(const std::string& vertex_source, const std::string& fragment_source)
    {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_code = vertex_source.c_str();
        const char* fragment_code = fragment_source.c_str();

        glShaderSource(vertex_shader, 1, &vertex_code, nullptr);
        glShaderSource(fragment_shader, 1, &fragment_code, nullptr);

        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader);

        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader);

        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader);
        glAttachShader(program_id, fragment_shader);
        glLinkProgram(program_id);

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(program_id);
    }

    void ShaderProgram::use() const {
        glUseProgram(program_id);
    }

    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
        GLint loc = glGetUniformLocation(program_id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec) const {
        GLint loc = glGetUniformLocation(program_id, name.c_str());
        glUniform3fv(loc, 1, glm::value_ptr(vec));
    }

    void ShaderProgram::setFloat(const std::string& name, float value) const {
        GLint loc = glGetUniformLocation(program_id, name.c_str());
        glUniform1f(loc, value);
    }

    void ShaderProgram::show_compilation_error(GLuint shader_id) {
        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            std::string info_log(log_length, ' ');
            glGetShaderInfoLog(shader_id, log_length, nullptr, &info_log[0]);
            std::cerr << "Shader compilation error:\n" << info_log << std::endl;
        }
    }

    void ShaderProgram::show_linkage_error(GLuint program_id) {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            std::string info_log(log_length, ' ');
            glGetProgramInfoLog(program_id, log_length, nullptr, &info_log[0]);
            std::cerr << "Shader program linkage error:\n" << info_log << std::endl;
        }
    }
}
