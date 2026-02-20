#pragma once
#include "shader.hpp"
#include <glad/glad.h>

class TShaderProgram {
  public:
    TShaderProgram(TShader<EShaderVariant::Vertex> vertex_shader, TShader<EShaderVariant::Fragment> fragment_shader);
    constexpr GLuint GetId() const noexcept {
        return id_;
    }

    ~TShaderProgram() noexcept {
        glDeleteProgram(id_);
    }

  private:
    GLuint id_;
};
