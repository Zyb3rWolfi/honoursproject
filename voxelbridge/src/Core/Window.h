//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_WINDOW_H
#define VOXELBRIDGE_WINDOW_H
#include <string>
struct GLFWwindow;
struct WindowConfig {
    int     width = 1260;
    int     height = 1260;
    std::string title = "VoxelBridge";
    bool vsync = true;
    int glMajor = 3;
    int glMinor = 3;
};

class Window {
public:
    explicit Window(const WindowConfig& config = {});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    GLFWwindow* Handle() const { return m_Window; };

    int Width() const { return m_Width; };
    int Height() const { return m_Height; };
    float AspectRatio() const {
        return m_Height > 0 ? static_cast<float>(m_Width) / static_cast<float>(m_Height) : 1.0f;
    }

    void SetTitle(const std::string& title);

    static double Time();
private:

    GLFWwindow* m_Window = nullptr;
    int m_Width = 0;
    int m_Height = 0;
};


#endif //VOXELBRIDGE_WINDOW_H
