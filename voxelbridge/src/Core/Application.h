//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_APPLICATION_H
#define VOXELBRIDGE_APPLICATION_H

#include "Window.h"
#include "../Gfx/Renderer.h"
#include "../ImGUI/ImGuiWrapper.h"
#include "../Core/Window.h"
#include "../ImGUI/ImGuiWrapper.h"
#include "../Gfx/Camera.h"
#include "../Gfx/Mesh.h"
#include "../Gfx/Renderer.h"

class Application {
public:
    Application();
    ~Application() = default;

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    void Run();

private:
    void OnUpdate(float dt);
    void OnRender();
    void OnImGui();

    // Declaration order = construction order; destruction runs in reverse.
    // Anything that owns GL objects must come AFTER m_Window so it is
    // destroyed while the GL context still exists.
    Window       m_Window;
    ImGuiWrapper m_Gui;
    Renderer     m_Renderer;
    Camera       m_Camera;
    Mesh         m_Cube = Mesh::CreateCube(glm::vec3(0.9f, 0.4f, 0.2f));

    // Test-scene / input state (will move into Input and Editor classes later)
    float  m_Angle       = 0.0f;
    double m_LastMouseX  = 0.0;
    double m_LastMouseY  = 0.0;
    bool   m_WasLooking  = false;
    bool   m_ShowDemoWindow = true;
};

#endif //VOXELBRIDGE_APPLICATION_H