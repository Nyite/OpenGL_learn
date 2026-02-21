#pragma once
#include <glad/glad.h>

class TVertextArray {
  public:
    TVertextArray() {
        glGenVertexArrays(1, &id_);
        glBindVertexArray(id_);
    }

    ~TVertextArray() {
        glDeleteVertexArrays(1, &id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

  private:
    GLuint id_;
};
