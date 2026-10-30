//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_MESH_H
#define VOXELBRIDGE_MESH_H
#include "Vertex.h"

class Mesh {
public:
    Mesh();
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<std::uint32_t>& indices);
    ~Mesh();

    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void SetData(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);

    void Draw() const;

    std::uint32_t IndexCount() const { return m_IndexCount; }

    static Mesh CreateCube(const glm::vec3& color = glm::vec3(0.8f));
private:
    void Release();

    unsigned int m_Vao = 0;
    unsigned int m_Vbo = 0;
    unsigned int m_Ebo = 0;
    std::uint32_t m_IndexCount = 0;

};


#endif //VOXELBRIDGE_MESH_H
