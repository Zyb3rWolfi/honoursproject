cmake_minimum_required(VERSION 4.2)

project(VoxelBridge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# ── Executable ─────────────────────────────────────────────────────────────────
add_executable(VoxelBridge src/main.cpp
        src/main.cpp
        src/ImGUI/ImGuiWrapper.cpp
        src/ImGUI/ImGuiWrapper.h
        src/Core/Application.cpp
        src/Core/Application.h
        src/Core/Window.cpp
        src/Core/Window.h)

target_link_libraries(VoxelBridge PRIVATE
    glfw
    glad::glad
    imgui::imgui
)
