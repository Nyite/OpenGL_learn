#pragma once
#include <fstream>
#include <filesystem>
#include <fmt/format.h>
#include <glad/glad.h>

enum class EShaderVariant { Vertex = 0, Fragment = 1 };

template <EShaderVariant ShaderVariant>
class TShader {
  public:
    TShader(const char* file_path);

    ~TShader() noexcept {
        glDeleteShader(id_);
    }

    constexpr GLuint GetId() const noexcept {
        return id_;
    }

  private:
    void Compile();

  private:
    GLuint id_;
};

template <EShaderVariant ShaderVariant>
TShader<ShaderVariant>::TShader(const char* file_path) {
    std::ifstream input_file(file_path, std::ios::in);
    if (!input_file) {
        throw std::runtime_error{fmt::format("Failed to read shader from file \"{}\"", file_path)};
    }

    auto size = std::filesystem::file_size(file_path);
    std::string text(size, '\0');
    input_file.read(text.data(), static_cast<long>(size));

    if constexpr (ShaderVariant == EShaderVariant::Vertex) {
        id_ = glCreateShader(GL_VERTEX_SHADER);
    } else {
        id_ = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* text_ptr = text.data();
    glShaderSource(id_, 1, &text_ptr, NULL);
    Compile();
}

template <EShaderVariant ShaderVariant>
inline void TShader<ShaderVariant>::Compile() {
    glCompileShader(id_);

    GLint success;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (!success) {
        constexpr size_t log_buffer_size = 512z;

        char log[log_buffer_size];
        glGetShaderInfoLog(id_, log_buffer_size, NULL, log);
        throw std::runtime_error{fmt::format("Failed to compile shader \"{}\" with error: {}", id_, log)};
    }
}
