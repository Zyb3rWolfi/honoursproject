//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_APPLICATION_H
#define VOXELBRIDGE_APPLICATION_H

#include "Window.h"
#include "../Gfx/Renderer.h"
#include "../Voxel/Mesher.h"
#include "../Voxel/World.h"

#include <unordered_map>
#include "../ImGUI/ImGuiWrapper.h"
#include "../Core/Window.h"
#include "../Editor/Picking.h"
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
    Picking      m_Picking;
    Ray m_ray;
    Mesh m_grid = Mesh::CreateGrid(64,16);
    Mesh m_highlighted = Mesh::CreateSquareOutline(glm::vec3(255));
    World m_World;
    Mesher m_Mesher;
    std::unordered_map<ChunkCoord, Mesh> m_ChunkMeshes;
    bool m_ShowGrid = true;

    // Test-scene / input state (will move into Input and Editor classes later)
    float  m_Angle       = 0.0f;
    double m_LastMouseX  = 0.0;
    double m_LastMouseY  = 0.0;
    bool   m_WasLooking  = false;
    bool   m_ShowDemoWindow = true;

    PickResult m_Pick;
    bool m_WasLeftDown = false;
    bool   m_ShowHit = true;


    enum class Tool {
        Add,
        Subtract
    };
    Tool m_CurrentTool = Tool::Add;
};

#endif //VOXELBRIDGE_APPLICATION_H