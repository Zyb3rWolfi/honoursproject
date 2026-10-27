cmake_minimum_required(VERSION 3.21)

project(VoxelBridge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# ── Executable ─────────────────────────────────────────────────────────────────
add_executable(VoxelBridge main.cpp)

target_link_libraries(VoxelBridge PRIVATE
    glfw
    glad::glad
    imgui::imgui
)
