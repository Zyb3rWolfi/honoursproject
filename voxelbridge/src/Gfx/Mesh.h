//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_MESH_H
#define VOXELBRIDGE_MESH_H
#include "Vertex.h"

class Mesh {
public:
    enum class Primitive { Triangle, Lines};

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

    void SetPrimitive(Primitive primitive) {primitive_ = primitive;};

    std::uint32_t IndexCount() const { return m_IndexCount; }

    static Mesh CreateCube(const glm::vec3& color = glm::vec3(0.8f));
    static Mesh CreateSquareOutline(const glm::vec3& color = glm::vec3(0.8f));
    static Mesh CreateGrid(int halfExtent = 64, int majorEvery = 16);
private:
    void Release();
    std::uint32_t m_indexCount = 0;
    Primitive primitive_ = Primitive::Triangle;
    unsigned int m_Vao = 0;
    unsigned int m_Vbo = 0;
    unsigned int m_Ebo = 0;
    std::uint32_t m_IndexCount = 0;

};


#endif //VOXELBRIDGE_MESH_H
