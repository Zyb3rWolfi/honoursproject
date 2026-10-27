cmake_minimum_required(VERSION 3.20)
project(untitled CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(untitled src/main.cpp)

# Add your src folder AND the path where the imgui backend headers reside
target_include_directories(untitled PRIVATE
        src
        ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/backends
)

target_link_libraries(untitled PRIVATE
        glfw
        glad::glad
        glm::glm
        imgui::imgui
)