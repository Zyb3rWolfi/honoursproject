//
// Created by antonscereskevics on 29/10/2026.
//

#include "Application.h"

#include <chrono>
#include <stdexcept>
#include <glad/glad.h>
#include <imgui.h>

Application::Application() : m_Window(WindowConfig{}) {
    if (!m_Gui.Init(m_Window.Handle(), "#version 330"))
        throw std::runtime_error("Failed to initialize GLFW");
}

void Application::Run() {
    double lastTime = Window::Time();

    while (!m_Window.ShouldClose()) {
        m_Window.PollEvents();

        const double now = Window::Time();
        const float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        OnUpdate(dt);
        OnRender();

        m_Gui.BeginFrame();
        OnImGui();
        m_Gui.EndFrame();

        m_Window.SwapBuffers();
    }
}

void Application::OnUpdate(float dt) {

}

void Application::OnRender() {
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


}

void Application::OnImGui() {
    if (m_ShowDemoWindow) {
        ImGui::Begin("VoxelBridge Demo", &m_ShowDemoWindow);
        ImGui::Text("Hello from ImGui!");
        ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::Text("Window: %d x %d", m_Window.Width(), m_Window.Height());
        ImGui::End();
    }
}
