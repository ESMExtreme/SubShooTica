#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Rozmiar okna
const int WIDTH = 800;
const int HEIGHT = 600;

// Pozycja kropki (x, y) w przestrzeni NDC [-1, 1]
float dotX = 0.0f;
float dotY = 0.0f;

// Prędkość ruchu
float speed = 0.5f; // jednostki na sekundę

// Shader wierzchołków (zmodyfikowany: dodane uScale)
const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 uOffset;
uniform vec2 uScale;
void main() {
    gl_Position = vec4(aPos * uScale + uOffset, 0.0, 1.0);
}
)";

// Shader fragmentów (zmodyfikowany: kolor jako uniform)
const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main() {
    FragColor = uColor;
}
)";

// Tworzenie programu shaderów
GLuint createProgram() {
    auto compile = [](GLenum type, const char* src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint ok;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << std::endl;
        }
        return shader;
    };

    GLuint vs = compile(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentShaderSrc);

     // Linkowanie programu
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(prog, 1024, nullptr, log);
        std::cerr << "Program link error:\n" << log << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

enum AppState { STATE_MENU, STATE_SETTINGS, STATE_GAME };

// Prosty helper do rysowania prostokąta przy użyciu obecnych danych (VAO/VBO/indices)
void drawRect(GLuint prog, GLint locOffset, GLint locScale, GLint locColor,
              float cx, float cy, float halfW, float halfH, const float color[4], float baseSize) {
    glUseProgram(prog);
    glUniform2f(locOffset, cx, cy);
    // skala względem "baseSize" (w pliku mamy vertexy w zakresie [-baseSize, baseSize])
    glUniform2f(locScale, halfW / baseSize, halfH / baseSize);
    glUniform4f(locColor, color[0], color[1], color[2], color[3]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {
    // Inicjalizacja GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    // Kontekst OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Kropka WASD - OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); // vsync

    if (glewInit() != GLEW_OK) { // Inicjalizacja GLEW
        std::cerr << "Failed to init GLEW\n";
        return -1;
    }

    // Dane geometrii: mały kwadrat jako "kropka" (rozmiar ~ 0.02 w NDC)
    float size = 0.02f; //jendostronny rozmiar kropki w jednostce NDC (base half-size)
    float vertices[] = { // x,    y
        -size, -size,
         size, -size,
         size,  size,
        -size,  size
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    // 1. Tworzenie i aktywowanie VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);      // Utwórz VAO
    glBindVertexArray(VAO);           // Aktywuj VAO (od teraz wszystko się do niego wiąże)

    // 2. Tworzenie VBO i EBO
    glGenBuffers(1, &VBO);            // Utwórz bufor wierzchołków
    glGenBuffers(1, &EBO);            // Utwórz bufor indeksów

    // 3. Wysyłanie danych wierzchołków do karty graficznej
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Aktywuj VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Prześlij dane: rozmiar, dane, GL_STATIC_DRAW = dane się nie zmieniają

    // 4. Wysyłanie indeksów
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // Aktywuj EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 5. Konfiguracja - jak czytać dane z VBO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // (0=slot, 2=2 komponenty XY, GL_FLOAT=typ, 2*sizeof=skok między wierzchołkami, offset=0)

    // 6. Włączenie atrybutu
    glEnableVertexAttribArray(0);  // Włącz slot 0

    GLuint program = createProgram();
    GLint uOffsetLoc = glGetUniformLocation(program, "uOffset");
    GLint uScaleLoc  = glGetUniformLocation(program, "uScale");
    GLint uColorLoc  = glGetUniformLocation(program, "uColor");

    // ImGui - inicjalizacja
    const char* glsl_version = "#version 330 core";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Ustawienia OpenGL
    glClearColor(0.08f, 0.10f, 0.12f, 1.0f);

    // MENU state
    AppState state = STATE_MENU;
    int menuIndex = 0;
    const int menuCount = 4; // Start, Settings, Dev Mode, Volume
    bool devMode = false;
    float volume = 0.5f; // 0..1

    // Pętla główna
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        // Poll events early for ImGui
        glfwPollEvents();

        // Input: nawigacja menu lub sterowanie w grze (stare klawisze dalej działają)
        if (state == STATE_MENU || state == STATE_SETTINGS) {
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                static double last = 0;
                if (now - last > 0.15) { menuIndex = (menuIndex + 1) % menuCount; last = now; }
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                static double last = 0;
                if (now - last > 0.15) { menuIndex = (menuIndex - 1 + menuCount) % menuCount; last = now; }
            }
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) {
                static double last = 0;
                if (now - last > 0.2) {
                    if (menuIndex == 0) { state = STATE_GAME; }
                    else if (menuIndex == 1) { state = STATE_SETTINGS; }
                    else if (menuIndex == 2) { devMode = !devMode; }
                    else if (menuIndex == 3) { state = STATE_SETTINGS; }
                    last = now;
                }
            }
            if (state == STATE_SETTINGS) {
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) volume = std::max(0.0f, volume - 0.5f * dt);
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) volume = std::min(1.0f, volume + 0.5f * dt);
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) state = STATE_MENU;
            }
        } else if (state == STATE_GAME) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) dotY += speed * dt;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) dotY -= speed * dt;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) dotX -= speed * dt;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) dotX += speed * dt;
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) state = STATE_MENU;
        }

        // Ograniczenie pozycji do widocznego obszaru [-1,1]
        float limit = 1.0f - size;
        if (dotX >  limit) dotX =  limit;
        if (dotX < -limit) dotX = -limit;
        if (dotY >  limit) dotY =  limit;
        if (dotY < -limit) dotY = -limit;

        glClear(GL_COLOR_BUFFER_BIT);

        // Rysowanie tła (prostokąty menu / kropka)
        glBindVertexArray(VAO);
        if (state == STATE_MENU || state == STATE_SETTINGS) {
            float ys[4] = { 0.4f, 0.1f, -0.2f, -0.5f };
            float halfW = 0.7f;
            float halfH = 0.09f;
            float baseColor[4] = {0.2f, 0.2f, 0.25f, 1.0f};
            float highlight[4] = {0.18f, 0.6f, 0.95f, 1.0f};
            for (int i = 0; i < 4; ++i) {
                const float *col = (i == menuIndex) ? highlight : baseColor;
                drawRect(program, uOffsetLoc, uScaleLoc, uColorLoc, 0.0f, ys[i], halfW, halfH, col, size);
            }

            if (state == STATE_SETTINGS) {
                float barCX = 0.0f;
                float barCY = -0.8f;
                float barHalfW = 0.6f;
                float barHalfH = 0.03f;
                float background[4] = {0.12f, 0.12f, 0.14f, 1.0f};
                float fill[4] = {0.2f, 0.8f, 0.2f, 1.0f};
                drawRect(program, uOffsetLoc, uScaleLoc, uColorLoc, barCX, barCY, barHalfW, barHalfH, background, size);
                drawRect(program, uOffsetLoc, uScaleLoc, uColorLoc, -barHalfW + barHalfW * volume, barCY, barHalfW * volume, barHalfH, fill, size);
            }

            // Ustaw tytuł okna (opcjonalnie)
            std::string title = "Menu - selected: ";
            if (menuIndex == 0) title += "Start";
            if (menuIndex == 1) title += "Settings";
            if (menuIndex == 2) title += (std::string("Dev mode: ") + (devMode ? "ON" : "OFF"));
            if (menuIndex == 3) title += ("Volume: " + std::to_string(int(volume * 100)) + "%");
            glfwSetWindowTitle(window, title.c_str());
        } else if (state == STATE_GAME) {
            float dotColor[4] = {1.0f, 0.9f, 0.2f, 1.0f};
            drawRect(program, uOffsetLoc, uScaleLoc, uColorLoc, dotX, dotY, size, size, dotColor, size);
            if (devMode) {
                std::string info = "Game - pos(" + std::to_string(dotX) + "," + std::to_string(dotY) + ") vol:" + std::to_string(int(volume*100)) + "%";
                glfwSetWindowTitle(window, info.c_str());
            } else {
                glfwSetWindowTitle(window, "Kropka WASD - OpenGL");
            }
        }

        // --- ImGui frame ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Jeśli jesteśmy w menu, pokaż ImGui z tekstami / przyciskami
        if (state == STATE_MENU || state == STATE_SETTINGS) {
            ImGui::Begin("Menu"); // okno ImGui
            if (ImGui::Button("Start game")) { state = STATE_GAME; }
            if (ImGui::Button("Settings")) { state = STATE_SETTINGS; }
            if (ImGui::Checkbox("Dev mode", &devMode)) { /* toggle already handled */ }
            ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
            ImGui::Text("Use Up/Down + Enter or click buttons");
            ImGui::End();

            if (state == STATE_SETTINGS) {
                ImGui::Begin("Settings");
                ImGui::Text("Adjust volume or ESC to return");
                ImGui::SliderFloat("Volume (duplicate)", &volume, 0.0f, 1.0f);
                if (ImGui::Button("Back")) state = STATE_MENU;
                ImGui::End();
            }
        } else if (state == STATE_GAME) {
            ImGui::Begin("Game HUD");
            ImGui::Text("WASD - move, ESC - back");
            ImGui::Text("Position: %.3f, %.3f", dotX, dotY);
            ImGui::Text("Volume: %d%%", int(volume*100));
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // -------------------

        glfwSwapBuffers(window);
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
