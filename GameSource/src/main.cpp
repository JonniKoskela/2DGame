#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cmath>

//----utility functions----
#include "utils.h"
#include "fileUtils.h"
#include "game.hpp"
#include "renderInterface.h"
#include "GLrenderer.hpp"
#include "input.hpp"
//----cpp----


constexpr float winWidth = 960.0f;
constexpr float winHeight = 540.0f;

//----------------

GLFWwindow* window;
BumpAllocator bump{};


int main(void)
{
    bump = makeBumpAllocator(MB(20));
    if (!glfwInit())
        return -1;

	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		window = glfwCreateWindow(winWidth, winHeight, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, keyCallback);
		//glfwSetCursorPosCallback(window, cursor_position_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		printf("%s\n", glGetString(GL_VERSION));
	}


	initializeInputs();
	setupGame();


    glInit(&bump);

    while (!glfwWindowShouldClose(window))
    {
        mainGameLoop();
        openGLRender();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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