#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdio>
#include <cstdlib>

// ── Error callback ─────────────────────────────────────────────────────────────
static void glfw_error_callback(int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// ── Shader helper ──────────────────────────────────────────────────────────────
static unsigned int compile_shader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::fprintf(stderr, "Shader compilation error:\n%s\n", info);
        std::exit(1);
    }
    return shader;
}

static unsigned int create_program(const char* vs_source, const char* fs_source) {
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vs_source);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fs_source);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, nullptr, info);
        std::fprintf(stderr, "Shader link error:\n%s\n", info);
        std::exit(1);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// ── Main ───────────────────────────────────────────────────────────────────────
int main() {
    glfwSetErrorCallback(glfw_error_callback);

    // ── Initialise GLFW ──────────────────────────────────────────────────────
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialise GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "VoxelBridge", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // ── Load GL function pointers via GLAD ───────────────────────────────────
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to initialise GLAD\n");
        return 1;
    }

    // ── ImGui setup ──────────────────────────────────────────────────────────
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ── Full-screen quad shader ──────────────────────────────────────────────
    const char* vs_source = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoords;
        out vec2 vTexCoords;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            vTexCoords = aTexCoords;
        }
    )";

    const char* fs_source = R"(
        #version 330 core
        in  vec2 vTexCoords;
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )";

    unsigned int shaderProgram = create_program(vs_source, fs_source);

    // Quad vertices: 2 triangles covering NDC space
    float vertices[] = {
         // positions   // texcoords
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f,

         1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ── Colour uniform location ──────────────────────────────────────────────
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    // ── ImGui demo state ─────────────────────────────────────────────────────
    bool showDemoWindow = true;

    // ── Render loop ──────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ── ImGui: new frame ─────────────────────────────────────────────────
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Demo window
        if (showDemoWindow) {
            ImGui::Begin("VoxelBridge Demo", &showDemoWindow);
            ImGui::Text("Hello from ImGui!");
            ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("This is a boilerplate OpenGL + ImGui project.");
            ImGui::End();
        }

        // ── Clear & draw background quad ─────────────────────────────────────
        glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform3f(colorLoc, 0.15f, 0.15f, 0.2f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ── ImGui: render ────────────────────────────────────────────────────
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // ── Cleanup ──────────────────────────────────────────────────────────────
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
