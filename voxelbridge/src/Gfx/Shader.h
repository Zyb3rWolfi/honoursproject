//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_SHADER_H
#define VOXELBRIDGE_SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>


class Shader {
public:
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void Bind() const;

    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
    int  GetLocation(const std::string& name) const;
    void Release();

    unsigned int m_Id = 0;
    mutable std::unordered_map<std::string, int> m_LocationCache;
};


#endif //VOXELBRIDGE_SHADER_H
