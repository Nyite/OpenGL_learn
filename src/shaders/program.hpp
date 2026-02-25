#pragma once
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.hpp"
#include "../objects/material.hpp"
#include "../objects/light.hpp"
#include <glad/glad.h>

class TShaderProgram {
  public:
    TShaderProgram(TShader<EShaderVariant::Vertex> vertex_shader, TShader<EShaderVariant::Fragment> fragment_shader);

    ~TShaderProgram() noexcept {
        glDeleteProgram(id_);
    }

    void SetMaterial(const TMaterial& material) {
        Use();
        material.diffuse_map.Bind();
        material.specular_map.Bind();

        SetUnifiorm("material.diffuse", material.diffuse_map.GetUniformIndex());
        SetUnifiorm("material.specular", material.specular_map.GetUniformIndex());
        SetUnifiorm("material.shininess", material.shine);
    }

    void SetLight(TLight light) {
        Use();
        SetUnifiorm("light.ambient",  glm::value_ptr(light.ambient));
        SetUnifiorm("light.diffuse",  glm::value_ptr(light.diffuse));
        SetUnifiorm("light.specular", glm::value_ptr(light.specular));
        SetUnifiorm("lightPos", glm::value_ptr(light.position));
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
        glUniform3fv(glGetUniformLocation(id_, name), 1, value);
    }

    void SetUnifiorm(const char* name, GLfloat x, GLfloat y, GLfloat z) {
        glm::vec3 value(x, y, z);
        glUniform3fv(glGetUniformLocation(id_, name), 1, glm::value_ptr(value));
    }

    void SetUnifiormMatrix(const char* name, const GLfloat *value) {
        glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, value);
    }

    void Use() const {
        glUseProgram(id_);
    }

  private:
    GLuint id_;
};
