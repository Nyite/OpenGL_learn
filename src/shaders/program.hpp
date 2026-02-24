#pragma once
#include "shader.hpp"
#include <glad/glad.h>

class TShaderProgram {
  public:
    TShaderProgram(TShader<EShaderVariant::Vertex> vertex_shader, TShader<EShaderVariant::Fragment> fragment_shader);

    ~TShaderProgram() noexcept {
        glDeleteProgram(id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

    void SetUnifiorm(const char* name, GLint value) {
        glUniform1i(glGetUniformLocation(id_, name), value);
    }

    void SetUnifiorm(const char* name, GLfloat value) {
        glUniform1f(glGetUniformLocation(id_, name), value);
    }

    void SetUnifiorm(const char* name, const GLfloat *value) {
        glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, value);
    }

    void Use() const {
        glUseProgram(id_);
    }

  private:
    GLuint id_;
};
