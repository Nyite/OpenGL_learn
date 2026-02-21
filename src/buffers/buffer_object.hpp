#pragma once
#include <cstddef>
#include <span>
#include <glad/glad.h>

enum class EBufferVariant { Vertex = 0, Element = 1 };

template <EBufferVariant BufferVariant>
class TBufferObject {
  public:
    template <typename T, size_t Extent>
    TBufferObject(const std::span<T, Extent>& values) {
        glGenBuffers(1, &id_);

        Bind();
        if constexpr (BufferVariant == EBufferVariant::Vertex) {
            glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(T), values.data(), GL_STATIC_DRAW);
        } else {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, values.size() * sizeof(T), values.data(), GL_STATIC_DRAW);
        }
        BindTo(0);
    }

    ~TBufferObject() {
        glDeleteBuffers(1, &id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

    void Bind() {
        BindTo(id_);
    }

  private:
    void BindTo(GLuint id) const {
        if constexpr (BufferVariant == EBufferVariant::Vertex) {
            glBindBuffer(GL_ARRAY_BUFFER, id);
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        }
    }

  private:
    GLuint id_;
};
