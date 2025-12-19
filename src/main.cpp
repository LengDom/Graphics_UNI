#include <iostream>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
// GLM for transforms
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// =======================
// Global OpenGL objects
// =======================
unsigned int VAO, VBO, EBO;
unsigned int shaderProgram;

// =======================
// Shader helper functions
// =======================
std::string LoadShader(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int CompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}

// =======================
// Triangle setup
// =======================
void initTriangle()
{
    float verts[] = {
        // positions        // colours
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// =======================
// Draw
// =======================
void drawTriangle()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// =======================
// MAIN
// =======================
int main()
{
    GLFWwindow* window;

    // GLFW init
    if (!glfwInit())
    {
        return -1;
    }

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(640, 480, "Lab 2 - Triangle", NULL, NULL);
    glfwMakeContextCurrent(window);

    // GLAD init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Load shaders (use files in Shaders/)
    std::string vertSrc = LoadShader("Shaders/basic.vert");
    std::string fragSrc = LoadShader("Shaders/basic.frag");

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertSrc.c_str());
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());

    // Check compile status and print errors
    auto printShaderLog = [](unsigned int shader){
        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader compile error:\n" << infoLog << std::endl;
        }
    };
    printShaderLog(vertexShader);
    printShaderLog(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check link status
    {
        int success = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
            std::cout << "Program link error:\n" << infoLog << std::endl;
        }
    }

    // Prepare transform uniform (scale + translate) using GLM
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f);
    // translate by (0.25, -0.1) then scale x/y by 0.5
    transform = glm::translate(transform, glm::vec3(0.25f, -0.1f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
    glUseProgram(shaderProgram);
    if (transformLoc >= 0) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    } else {
        std::cout << "Warning: 'transform' uniform not found in shader." << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup geometry
    initTriangle();

    // Background colour
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        drawTriangle();

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
