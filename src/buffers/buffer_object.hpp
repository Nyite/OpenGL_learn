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

        if constexpr (BufferVariant == EBufferVariant::Vertex) {
            glBindBuffer(GL_ARRAY_BUFFER, id_);
            glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(T), values.data(), GL_STATIC_DRAW);
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, values.size() * sizeof(T), values.data(), GL_STATIC_DRAW);
        }
    }

    ~TBufferObject() {
        glDeleteBuffers(1, &id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

  private:
    GLuint id_;
};
