#include "program.hpp"
#include <fmt/format.h>

TShaderProgram::TShaderProgram(TShader<EShaderVariant::Vertex> vertex_shader,
                               TShader<EShaderVariant::Fragment> fragment_shader) {
    id_ = glCreateProgram();
    glAttachShader(id_, vertex_shader.GetId());
    glAttachShader(id_, fragment_shader.GetId());
    glLinkProgram(id_);

    GLint success = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        constexpr size_t log_buffer_size = 512z;
        char log[log_buffer_size];

        glGetProgramInfoLog(id_, log_buffer_size, NULL, log);
        throw std::runtime_error{fmt::format("Failed to link shader programm \"{}\" with error: {}", id_, log)};
    }
}