//
// Created by antonscereskevics on 29/10/2026.
//

#include "ImGuiWrapper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImGuiWrapper::~ImGuiWrapper() {
    Shutdown();
}

bool ImGuiWrapper::Init(GLFWwindow* window, const char* glsl_version) {
    if (m_Initialized) {
        return true;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        return false;
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
        return false;

    m_Initialized = true;
    return true;
}

void ImGuiWrapper::Shutdown()
{
    if (!m_Initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_Initialized = false;
}

void ImGuiWrapper::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}