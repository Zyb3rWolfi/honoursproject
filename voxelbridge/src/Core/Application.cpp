//
// Created by antonscereskevics on 29/10/2026.
//

#include "Application.h"

#include <GLFW/glfw3.h>   // only for GLFW_KEY_* / GLFW_MOUSE_BUTTON_* constants
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

#include "../Editor/Picking.h"

Application::Application() : m_Window(WindowConfig{}) {
    if (!m_Gui.Init(m_Window.Handle(), "#version 330"))
        throw std::runtime_error("Failed to initialize ImGui");

    // Seed one 1x1x1 voxel so the voxel pipeline is visible before editing is added.
    m_World.SetVoxel(0, 0, 0, 1);
}

void Application::Run() {
    double lastTime = Window::Time();

    while (!m_Window.ShouldClose()) {
        m_Window.PollEvents();

        // Getting the DeltaTime
        const double now = Window::Time();
        const float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        OnUpdate(dt);
        OnRender();

        // Updating the ImGUI Interface
        m_Gui.BeginFrame();
        OnImGui();
        m_Gui.EndFrame();

        m_Window.SwapBuffers();


    }
}

// Main Application Update Function
void Application::OnUpdate(float dt) {
    const ImGuiIO& io = ImGui::GetIO();

    // The Movement
    if (!io.WantCaptureKeyboard) {
        const float speed = 5.0f * dt;
        if (m_Window.IsKeyDown(GLFW_KEY_W))          m_Camera.Position += m_Camera.Forward() * speed;
        if (m_Window.IsKeyDown(GLFW_KEY_S))          m_Camera.Position -= m_Camera.Forward() * speed;
        if (m_Window.IsKeyDown(GLFW_KEY_D))          m_Camera.Position += m_Camera.Right() * speed;
        if (m_Window.IsKeyDown(GLFW_KEY_A))          m_Camera.Position -= m_Camera.Right() * speed;
        if (m_Window.IsKeyDown(GLFW_KEY_SPACE))      m_Camera.Position.y += speed;
        if (m_Window.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) m_Camera.Position.y -= speed;
    }

    // Moving around the area
    double mx, my;
    m_Window.GetCursorPosition(mx, my);

    const bool looking = m_Window.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)
                         && !io.WantCaptureMouse;
    if (looking && m_WasLooking) {
        m_Camera.Yaw   += static_cast<float>(mx - m_LastMouseX) * 0.1f;
        m_Camera.Pitch -= static_cast<float>(my - m_LastMouseY) * 0.1f;
        m_Camera.Pitch  = glm::clamp(m_Camera.Pitch, -89.0f, 89.0f);
    }
    m_WasLooking = looking;
    m_LastMouseX = mx;
    m_LastMouseY = my;

    m_Angle += dt;

    glm::vec2 ndc = m_Window.cursorInNDC();
    glm::mat4 view = m_Camera.View();
    glm::mat4 projection = m_Camera.Projection(m_Window.AspectRatio());

    m_ray = m_Picking.ScreenToRay(ndc, view, projection);
    m_RayHit = m_Picking.IntersectRayPlaneY(m_ray, 0, hitPoint);

    // This is responsible for detecting which cell we ar ehovering over.
    if (m_RayHit) {
        m_HoverCell.x= static_cast<int>(std::floor(hitPoint.x));
        m_HoverCell.y = static_cast<int>(std::floor(hitPoint.z));
    }

    // This is responsible for actually setting the voxel on that selected grid
    if (m_Window.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse && m_RayHit && m_CurrentTool == Tool::Add) {
        const int fx = static_cast<int>(std::floor(hitPoint.x));
        const int fy = static_cast<int>(std::floor(hitPoint.z));
        m_World.SetVoxel(fx, 0, fy, 1);
    }
    if (m_Window.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !io.WantCaptureMouse && m_RayHit && m_CurrentTool == Tool::Subtract) {
        const int fx = static_cast<int>(std::floor(hitPoint.x));
        const int fy = static_cast<int>(std::floor(hitPoint.z));
        m_World.SetVoxel(fx, 0, fy, 0);
    }
}


// Responsible for rendering the application
void Application::OnRender() {
    m_Renderer.Clear(glm::vec3(0.15f, 0.15f, 0.2f));
    m_Renderer.BeginScene(m_Camera, m_Window.AspectRatio());

    // Rebuild only chunks whose voxel data changed, then draw every resident mesh.
    for (const ChunkCoord& coord : m_World.DirtyChunks()) {
        MeshData data = m_Mesher.BuildChunkMesh(m_World, coord);
        auto [it, inserted] = m_ChunkMeshes.try_emplace(coord);
        it->second.SetData(data.vertices, data.indices);
        it->second.SetPrimitive(Mesh::Primitive::Triangle);
        if (Chunk* chunk = m_World.GetChunk(coord)) {
            chunk->ClearDirty();
        }
    }

    for (const auto& [coord, mesh] : m_ChunkMeshes) {
        const WorldPos origin = ChunkOrigin(coord);
        const glm::mat4 model = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(static_cast<float>(origin.x),
                      static_cast<float>(origin.y),
                      static_cast<float>(origin.z)));
        m_Renderer.Submit(mesh, model);
    }

    if (m_ShowGrid) {
        m_Renderer.SubmitLines(m_grid);
    }

    if (m_RayHit && m_ShowHit) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_HoverCell.x, 0.01f, m_HoverCell.y));
        m_Renderer.SubmitLines(m_highlighted, model);
    }
}

void Application::OnImGui() {
    std::size_t totalVoxels = 0;
    for (const auto& [coord, chunk] : m_World.Chunks()) {
        totalVoxels += static_cast<std::size_t>(chunk.FilledCount());
    }
    if (m_ShowDemoWindow) {
        ImGui::Begin("Debug Menu", &m_ShowDemoWindow);
        ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::Text("Window: %d x %d", m_Window.Width(), m_Window.Height());
        ImGui::Text("Ray: %.3f", glm::dot(m_ray.direction, m_Camera.Forward()) );
        ImGui::Text("Cell Selected: %.i, %.i", m_HoverCell.x, m_HoverCell.y);

        ImGui::Text("Camera: %.1f, %.1f, %.1f",
                    m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);
        ImGui::Text("WASD move, Space/Shift up/down, hold RMB to look");
        ImGui::Checkbox("Show grid", &m_ShowGrid);
        ImGui::Checkbox("Show Selected Grid", &m_ShowHit);

        ImGui::Text("Created voxels: %zu", totalVoxels);
        ImGui::Text("Chunks: %zu", m_World.ChunkCount());
        ImGui::Separator();
        if (m_World.Chunks().empty()) {
            ImGui::TextDisabled("No voxels created");
        } else {
            for (const auto& [coord, chunk] : m_World.Chunks()) {
                ImGui::BulletText("Chunk (%d, %d, %d): %d voxels",
                                  coord.x, coord.y, coord.z, chunk.FilledCount());
            }
        }
        ImGui::End();
    }

    ImGui::Begin("Tools");
    ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui:ImGui::Text("Current Tool: %s", m_CurrentTool == Tool::Add ? "Add" : "Subtract");
    if (ImGui::Button("Add")) {
        m_CurrentTool = Tool::Add;

    }
    if (ImGui::Button("Subtract")) {
        m_CurrentTool = Tool::Subtract;
    }
    ImGui::End();
}