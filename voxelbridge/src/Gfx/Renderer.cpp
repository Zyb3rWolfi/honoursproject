//
// Created by antonscereskevics on 29/10/2026.
//

#include "Renderer.h"
#include <glad/glad.h>

// Temp shaders
static const char* kVertexSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 uViewProj;
uniform mat4 uModel;

out vec3 vNormal;
out vec3 vColor;

void main() {
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
    vNormal = mat3(uModel) * aNormal;   // fine for uniform scale only
    vColor  = aColor;
}
)";

static const char* kFragmentSource = R"(
#version 330 core
in vec3 vNormal;
in vec3 vColor;
out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.6));
    float diffuse = max(dot(normalize(vNormal), lightDir), 0.0);
    float light   = 0.3 + 0.7 * diffuse;   // ambient + diffuse
    FragColor = vec4(vColor * light, 1.0);
}
)";

// Init the renderer
Renderer::Renderer()
    : m_Shader(kVertexSource, kFragmentSource) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

// Clears the screen
void Renderer::Clear(const glm::vec3 &color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Begin our Scene
void Renderer::BeginScene(const Camera& camera, float aspectRatio) {
    m_Shader.Bind();
    m_Shader.SetMat4("uViewProj", camera.Projection(aspectRatio) * camera.View());
}

// Select the current mesh for render
void Renderer::Submit(const Mesh& mesh, const glm::mat4& model) {
    m_Shader.Bind();
    m_Shader.SetMat4("uModel", model);
    mesh.Draw();
}

