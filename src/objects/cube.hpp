#pragma once
#include "glm/fwd.hpp"
#include "shapes.hpp"
#include "material.hpp"
#include "../buffers/buffer_object.hpp"
#include "../buffers/vertex_array.hpp"
#include "../shaders/program.hpp"
#include <span>

// Mesh data: VAO -> VBO(vtx, norms, tex), EBO
// Transforms: model matrix
// Shader program&

class TCube {
  public:
    TCube(TShaderProgram& shader) : VBO_(std::span{CubeVerticies}), modelMx(1.0f), shader_(shader) {
        VAO_.Bind();
        VBO_.Bind();

        // verticies positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    };

    void Draw(const glm::mat4& projectionMx, const glm::mat4& viewMx) const {
        shader_.Use();
        shader_.SetUnifiormMatrix("projection", glm::value_ptr(projectionMx));
        shader_.SetUnifiormMatrix("view", glm::value_ptr(viewMx));
        shader_.SetUnifiormMatrix("model", glm::value_ptr(modelMx));

        VAO_.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Bind() {
        VAO_.Bind();
    }

  public:
    glm::mat4 modelMx;

  private:
    TVertextArray VAO_;
    TBufferObject<EBufferVariant::Vertex> VBO_;
    TShaderProgram& shader_;
};
