#pragma once
#include <stdexcept>
#include <fmt/format.h>
#include <glad/glad.h>
#include <stb/stb_image.h>

class TTexture {
  public:
    TTexture(const char* file_name, GLenum image_format, GLenum texture_unit) {
        int wigth, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load(file_name, &wigth, &height, &channels, 0);
        if (!image) {
            throw std::runtime_error{fmt::format("Failed to load texture {} from file {}", texture_unit, file_name)};
        }

        glGenTextures(1, &id_);
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wigth, height, 0, image_format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
    }

    ~TTexture() noexcept {
        glDeleteTextures(1, &id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

    void Bind() const {
        glActiveTexture(texture_unit_);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

  private:
    GLuint id_;
    GLenum texture_unit_;
};
