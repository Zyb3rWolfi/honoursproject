//
// Created by antonscereskevics on 29/10/2026.
//

#include "Application.h"

#include <GLFW/glfw3.h>   // only for GLFW_KEY_* / GLFW_MOUSE_BUTTON_* constants
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

Application::Application() : m_Window(WindowConfig{}) {
    if (!m_Gui.Init(m_Window.Handle(), "#version 330"))
        throw std::runtime_error("Failed to initialize ImGui");
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
}

// Responsible for rendering the application
void Application::OnRender() {
    m_Renderer.Clear(glm::vec3(0.15f, 0.15f, 0.2f));
    m_Renderer.BeginScene(m_Camera, m_Window.AspectRatio());

    // Rendering the Cube
    const glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_Angle, glm::vec3(0.0f, 1.0f, 0.0f));
    m_Renderer.Submit(m_Cube, model);
}

void Application::OnImGui() {
    if (m_ShowDemoWindow) {
        ImGui::Begin("VoxelBridge Demo", &m_ShowDemoWindow);
        ImGui::Text("Hello from ImGui!");
        ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::Text("Window: %d x %d", m_Window.Width(), m_Window.Height());
        ImGui::Text("Camera: %.1f, %.1f, %.1f",
                    m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);
        ImGui::Text("WASD move, Space/Shift up/down, hold RMB to look");
        ImGui::End();
    }
}