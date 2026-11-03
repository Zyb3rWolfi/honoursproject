//
// Created by antonscereskevics on 29/10/2026.
//

#include "Mesh.h"

#include <glad/glad.h>

#include <cstddef>   // offsetof
#include <utility>


Mesh::Mesh() {
    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);
    glGenBuffers(1, &m_Ebo);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, color)));

    glBindVertexArray(0);

}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
    : Mesh() {
    SetData(vertices, indices);
}

Mesh::~Mesh() { Release(); }

Mesh::Mesh(Mesh&& other) noexcept
    : m_Vao(std::exchange(other.m_Vao, 0)),
      m_Vbo(std::exchange(other.m_Vbo, 0)),
      m_Ebo(std::exchange(other.m_Ebo, 0)),
      m_IndexCount(std::exchange(other.m_IndexCount, 0)),
        primitive_(other.primitive_){}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        Release();
        m_Vao = std::exchange(other.m_Vao, 0);
        m_Vbo = std::exchange(other.m_Vbo, 0);
        m_Ebo = std::exchange(other.m_Ebo, 0);
        m_IndexCount = std::exchange(other.m_IndexCount, 0),
        primitive_ = other.primitive_;
    }
    return *this;
}

void Mesh::Release() {
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
    if (m_Ebo) glDeleteBuffers(1, &m_Ebo);
    m_Vao = m_Vbo = m_Ebo = 0;
    m_IndexCount = 0;
}

// Setting the Data for the mesg
void Mesh::SetData(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices) {
    glBindVertexArray(m_Vao);   // keeps the EBO binding correct

    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t)),
                 indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    m_IndexCount = static_cast<std::uint32_t>(indices.size());
}

// Responsible for drawing the selected mesh
void Mesh::Draw() const {
    if (m_IndexCount == 0)
        return;
    glBindVertexArray(m_Vao);
    glDrawElements(primitive_ == Primitive::Lines ? GL_LINES : GL_TRIANGLES,
                   static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// This is the mesh for a simple cube
Mesh Mesh::CreateCube(const glm::vec3& color) {
    // Each face: normal n and two tangent axes u, v with cross(u, v) == n,
    // which gives counter-clockwise winding seen from outside.
    struct Face { glm::vec3 n, u, v; };
    const Face faces[6] = {
        {{ 0, 0, 1}, { 1, 0, 0}, { 0, 1, 0}},   // +Z
        {{ 0, 0,-1}, {-1, 0, 0}, { 0, 1, 0}},   // -Z
        {{ 1, 0, 0}, { 0, 0,-1}, { 0, 1, 0}},   // +X
        {{-1, 0, 0}, { 0, 0, 1}, { 0, 1, 0}},   // -X
        {{ 0, 1, 0}, { 1, 0, 0}, { 0, 0,-1}},   // +Y
        {{ 0,-1, 0}, { 1, 0, 0}, { 0, 0, 1}},   // -Y
    };

    // Reserving the vertices
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    vertices.reserve(24);
    indices.reserve(36);

    for (const Face& f : faces) {
        const glm::vec3 c = f.n * 0.5f;
        const auto base = static_cast<std::uint32_t>(vertices.size());

        vertices.push_back({c - f.u * 0.5f - f.v * 0.5f, f.n, color});
        vertices.push_back({c + f.u * 0.5f - f.v * 0.5f, f.n, color});
        vertices.push_back({c + f.u * 0.5f + f.v * 0.5f, f.n, color});
        vertices.push_back({c - f.u * 0.5f + f.v * 0.5f, f.n, color});

        for (std::uint32_t i : {0u, 1u, 2u, 0u, 2u, 3u})
            indices.push_back(base + i);
    }

    return Mesh(vertices, indices);
}

// This is responsible for rendering the grid
// For every whole number from -64 to 64, it would add two lines, one parralell to each axis.

Mesh Mesh::CreateGrid(int halfExtent, int majorEvery) {

    const glm::vec3 minor(0.30f), majoor(0.50f); // Grey values
    const glm::vec3 axisX(0.85f, 0.25f, 0.25f), axisZ(0.25f, 0.45f, 0.90f); // Red values and blue
    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    const float e = static_cast<float>(halfExtent);

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    // Helper: one line = vertices + 2 indeces

    auto addLine = [&](glm::vec3 a, glm::vec3 b, glm::vec3 color) {
        const auto base = static_cast<std::uint32_t>(vertices.size());
        vertices.push_back({a, up, color});
        vertices.push_back({b, up, color});
        indices.push_back(base);
        indices.push_back(base + 1);
    };

    for (int i = -halfExtent; i <= halfExtent; i++) {
        if (i==0) continue;
        const glm::vec3& c = (i % majorEvery == 0) ? majoor : minor;
        const float f = static_cast<float>(i);
        addLine({f, 0.0f, -e}, {f, 0.0f, e}, c);    // a line running along Z at x = i
        addLine({-e, 0.0f, f}, {e, 0.0f, f}, c);    // a line running along X at z = i
    }
    addLine({-e, 0.0f, 0.0f}, {e, 0.0f, 0.0f}, axisX);   // X axis, red
    addLine({0.0f, 0.0f, -e}, {0.0f, 0.0f, e}, axisZ);   //

    Mesh mesh(vertices, indices);
    mesh.SetPrimitive(Primitive::Lines);
    return mesh;
}

Mesh Mesh::CreateSquareOutline(const glm::vec3 &color) {

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    const glm::vec3 p0(0, 0, 0), p1(1, 0, 0), p2(1, 0, 1), p3(0, 0, 1);

    auto addLine = [&](glm::vec3 a, glm::vec3 b) {
        const auto base = static_cast<std::uint32_t>(vertices.size());
        vertices.push_back({a, up, color});
        vertices.push_back({b, up, color});
        indices.push_back(base);
        indices.push_back(base + 1);
    };

    addLine(p0, p1);
    addLine(p1, p2);
    addLine(p2, p3);
    addLine(p3, p0);
    Mesh mesh(vertices, indices);
    mesh.SetPrimitive(Primitive::Lines);
    return mesh;
}
