#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static unsigned int CompileStage(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<size_t>(length > 1 ? length : 1));
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader compile error:\n") + log.data());
    }
    return shader;
}

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    unsigned int vs = CompileStage(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs;
    try {
        fs = CompileStage(GL_FRAGMENT_SHADER, fragmentSource);
    } catch (...) {
        glDeleteShader(vs);
        throw;
    }

    m_Id = glCreateProgram();
    glAttachShader(m_Id, vs);
    glAttachShader(m_Id, fs);
    glLinkProgram(m_Id);

    glDeleteShader(vs);
    glDeleteShader(fs);

    int success = 0;
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success) {
        int length = 0;
        glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<size_t>(length > 1 ? length : 1));
        glGetProgramInfoLog(m_Id, length, nullptr, log.data());
        glDeleteProgram(m_Id);
        m_Id = 0;
        throw std::runtime_error(std::string("Shader link error:\n") + log.data());
    }
}

Shader::~Shader() { Release(); }

Shader::Shader(Shader&& other) noexcept
    : m_Id(std::exchange(other.m_Id, 0)),
      m_LocationCache(std::move(other.m_LocationCache)) {}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        Release();
        m_Id = std::exchange(other.m_Id, 0);
        m_LocationCache = std::move(other.m_LocationCache);
    }
    return *this;
}

void Shader::Release() {
    if (m_Id != 0) {
        glDeleteProgram(m_Id);
        m_Id = 0;
    }
    m_LocationCache.clear();
}

void Shader::Bind() const { glUseProgram(m_Id); }

int Shader::GetLocation(const std::string& name) const {
    auto it = m_LocationCache.find(name);
    if (it != m_LocationCache.end())
        return it->second;

    int location = glGetUniformLocation(m_Id, name.c_str());
    m_LocationCache[name] = location;   // -1 is cached too (uniform optimised out)
    return location;
}

// NOTE: the shader must be bound (Bind()) before calling the setters.
void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(GetLocation(name), value);
}
void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(GetLocation(name), value);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetLocation(name), 1, glm::value_ptr(value));
}
void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}