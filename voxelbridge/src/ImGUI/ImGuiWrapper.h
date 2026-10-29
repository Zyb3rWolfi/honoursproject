//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_IMGUIWRAPPER_H
#define VOXELBRIDGE_IMGUIWRAPPER_H

struct GLFWwindow;

class ImGuiWrapper {
public:
    ImGuiWrapper() = default;
    ~ImGuiWrapper();

    ImGuiWrapper(const ImGuiWrapper&) = delete;
    ImGuiWrapper& operator=(const ImGuiWrapper&) = delete;

    bool Init(GLFWwindow* window, const char* glsl_version = "#version 330");
    void Shutdown();
    void BeginFrame();
    void EndFrame();
private:
    bool m_Initialized = false;
};


#endif //VOXELBRIDGE_IMGUIWRAPPER_H
