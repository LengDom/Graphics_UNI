#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void initTriangle()
{
    // TODO: add triangle vertex data

    // TODO: create vertex buffer

    // TODO: set buffer data to triangle vertex and setting vertex attributes

}

// TODO: draw triangle
void drawTriangle()
{

}

int main()
{
    GLFWwindow *window;

    // GLFW init
    if (!glfwInit())
    {
        return -1;
    }

    // create a GLFW window
    window = glfwCreateWindow(640, 480, "Hello OpenGL1", NULL, NULL);
    glfwMakeContextCurrent(window);

    // loading glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couuldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    // TODO: call initTriangle()


    // TODO: set the background colour using glClearColor(r, g, b, a)


    // setting the event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // TODO : clear the background colour and call drawTriangle()

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}