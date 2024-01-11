
//----utility functions----
#include "utils.h"
#include "fileUtils.h"
#include "game.hpp"
#include "renderInterface.h"
#include "GLrenderer.hpp"
#include "input.hpp"
//----cpp----
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cmath>


static float deltaTime = 0.0f;


//----------------


Vec2 mPos{};
GLFWwindow* window;
static BumpAllocator bump{};


int main(void)
{
    bump = makeBumpAllocator(MB(50));
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    window = glfwCreateWindow(winWidth, winHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
   //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, input::keyCallback);
    glfwSetCursorPosCallback(window, input::cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    printf("%s\n", glGetString(GL_VERSION));

    glInit(&bump);

    while (!glfwWindowShouldClose(window))
    {
        openGLRender();
        mainGameLoop();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


















/*const char* vertexShaderSource =
"#version 460\n"

"layout(location = 0) in vec2 position;"
"uniform mat4 orthoProjection;"


"void main(){\n"
"vec4 finalPosition = orthoProjection * vec4(position,  0.0,  1.0);\n"
"gl_Position = finalPosition;\n"
"}\n";

const char* fragmentShaderSource =
"#version 460\n"
"out vec4 FragColor;\n"
"\n"
"void main(){\n"
"FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\0";
*/