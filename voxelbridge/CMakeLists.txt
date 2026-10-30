cmake_minimum_required(VERSION 4.2)

project(VoxelBridge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# ── Executable ─────────────────────────────────────────────────────────────────
add_executable(VoxelBridge src/main.cpp
        src/main.cpp
        src/ImGUI/ImGuiWrapper.cpp
        src/ImGUI/ImGuiWrapper.h
        src/Core/Application.cpp
        src/Core/Application.h
        src/Core/Window.cpp
        src/Core/Window.h
        src/Gfx/Shader.cpp
        src/Gfx/Shader.h
        src/Gfx/Renderer.cpp
        src/Gfx/Renderer.h
        src/Gfx/Mesh.cpp
        src/Gfx/Mesh.h
        src/Gfx/Vertex.h
        src/Gfx/Camera.h)

target_link_libraries(VoxelBridge PRIVATE
    glfw
    glad::glad
    glm::glm
    imgui::imgui
)
