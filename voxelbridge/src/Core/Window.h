//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_WINDOW_H
#define VOXELBRIDGE_WINDOW_H

#include <string>

struct GLFWwindow;

struct WindowConfig {
    int         width   = 1280;
    int         height  = 720;
    std::string title   = "VoxelBridge";
    bool        vsync   = true;
    int         glMajor = 3;
    int         glMinor = 3;
};

class Window {
public:
    explicit Window(const WindowConfig& config = {});
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    GLFWwindow* Handle() const { return m_Window; }

    // Framebuffer size in pixels
    int   Width()  const { return m_Width; }
    int   Height() const { return m_Height; }
    float AspectRatio() const {
        return m_Height > 0 ? static_cast<float>(m_Width) / static_cast<float>(m_Height) : 1.0f;
    }

    // Input helpers so other code doesn't need to call GLFW directly.
    // Pass GLFW_KEY_* / GLFW_MOUSE_BUTTON_* constants.
    bool IsKeyDown(int key) const;
    bool IsMouseButtonDown(int button) const;
    void GetCursorPosition(double& x, double& y) const;

    void SetTitle(const std::string& title);

    static double Time();

private:
    GLFWwindow* m_Window = nullptr;
    int m_Width  = 0;
    int m_Height = 0;
};

#endif //VOXELBRIDGE_WINDOW_H