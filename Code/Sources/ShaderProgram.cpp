#include "../Headers/ShaderProgram.hpp"
namespace udit
{

    // Compila los shaders de la escena
    GLuint ShaderProgram::compile_shaders(string vertex_shader, string fragment_shader)
    {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_shaders_code[] = { vertex_shader.c_str() };
        const char* fragment_shaders_code[] = { fragment_shader.c_str() };
        const GLint vertex_shaders_size[] = { (GLint)vertex_shader.size() };
        const GLint fragment_shaders_size[] = { (GLint)fragment_shader.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        GLuint program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);
        glLinkProgram(program_id);

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return program_id;
    }
    // Muestra errores de compilación del shader
    void ShaderProgram::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint info_log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length > 0) {
            info_log.resize(info_log_length);
            glGetShaderInfoLog(shader_id, info_log_length, nullptr, &info_log[0]);
            cerr << "Shader compilation error: " << endl << info_log << endl;
        }
    }

    // Muestra errores de enlace del programa de shaders
    void ShaderProgram::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint info_log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length > 0) {
            info_log.resize(info_log_length);
            glGetProgramInfoLog(program_id, info_log_length, nullptr, &info_log[0]);
            cerr << "Shader program linkage error: " << endl << info_log << endl;
        }
    }
}