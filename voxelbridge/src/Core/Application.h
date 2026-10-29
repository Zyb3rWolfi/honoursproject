//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_APPLICATION_H
#define VOXELBRIDGE_APPLICATION_H
#include "Window.h"
#include "../ImGUI/ImGuiWrapper.h"


class Application {
public:
    Application();
    ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Run();
private:
    void OnUpdate(float dt);
    void OnRender();
    void OnImGui();

    Window m_Window;
    ImGuiWrapper m_Gui;
    bool m_ShowDemoWindow = true;
};


#endif //VOXELBRIDGE_APPLICATION_H
