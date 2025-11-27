#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Rozmiar okna
const int WIDTH = 800;
const int HEIGHT = 600;

// Pozycja kropki (x, y) w przestrzeni NDC [-1, 1]
float dotX = 0.0f;
float dotY = 0.0f;

// Prędkość ruchu
float speed = 0.5f; // jednostki na sekundę

// Shader wierzchołków
const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 uOffset;
void main() {
    gl_Position = vec4(aPos + uOffset, 0.0, 1.0);
}
)";

// Shader fragmentów
const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main() {
    // Jasna kropka
    FragColor = vec4(1.0, 0.9, 0.2, 1.0);
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

int main() {
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

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return -1;
    }

    // Dane geometrii: mały kwadrat jako "kropka" (rozmiar ~ 0.02 w NDC)
    float size = 0.02f;
    float vertices[] = {
        -size, -size,
         size, -size,
         size,  size,
        -size,  size
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    // VAO/VBO/EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint program = createProgram();
    GLint uOffsetLoc = glGetUniformLocation(program, "uOffset");

    // Ustawienia OpenGL
    glClearColor(0.08f, 0.10f, 0.12f, 1.0f);

    // Pętla główna
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = static_cast<float>(now - lastTime);
        lastTime = now;

        // Wejście z klawiatury WASD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) dotY += speed * dt;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) dotY -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) dotX -= speed * dt;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) dotX += speed * dt;

        // Ograniczenie pozycji do widocznego obszaru [-1,1]
        float limit = 1.0f - size;
        if (dotX >  limit) dotX =  limit;
        if (dotX < -limit) dotX = -limit;
        if (dotY >  limit) dotY =  limit;
        if (dotY < -limit) dotY = -limit;

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform2f(uOffsetLoc, dotX, dotY);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
