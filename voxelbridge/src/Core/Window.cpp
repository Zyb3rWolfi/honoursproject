//
// Created by antonscereskevics on 29/10/2026.
//

#include "Window.h"
#include <glad/glad.h>   // must come before GLFW
#include <GLFW/glfw3.h>

#include <cstdio>
#include <stdexcept>

static void glfw_error_callback(int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Window::Window(const WindowConfig& config) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(config.vsync ? 1 : 0);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialise GLAD");
    }
    // Track framebuffer size and keep the viewport in sync
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* w, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        self->m_Width  = width;
        self->m_Height = height;
        glViewport(0, 0, width, height);
    });

    glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
    glViewport(0, 0, m_Width, m_Height);
}

    Window::~Window() {
        if (m_Window)
            glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }
    void Window::PollEvents()  const { glfwPollEvents(); }
    void Window::SwapBuffers() const { glfwSwapBuffers(m_Window); }

    void Window::SetTitle(const std::string& title) {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    double Window::Time() { return glfwGetTime(); }


