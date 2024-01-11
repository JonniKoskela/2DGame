#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//----glm----
#include <glm.hpp>
#include <matrix.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
constexpr float winWidth = 1000.0f;
constexpr float winHeight = 800.0f;
static Vec2 screenSize{ 1000.0f, 800.0f };
const char* TEXTURE_PATH = "assets/textureAtlas.png";


GLuint orthoID{};
GLuint screenSizeID{};
GLuint transformSBO{};
GLuint shaderProgram{};
GLuint texture{};

bool compileShaders(BumpAllocator* bump);





bool glInit(BumpAllocator* bump) {
    GLenum error{};

    if (!compileShaders(bump))
    {
        std::cerr << "failed to compile shaders" << "\n";
    }

    GLuint VAO{};
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR b: " << error << "\n";
    }
    {
        int width, height, channels;
        char* data = (char*)stbi_load(TEXTURE_PATH, &width, &height, &channels, 4);
        assert(data && "stbi_load failure");

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glGenBuffers(1, &transformSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, transformSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * renderData->transforms.capacity(), renderData->transforms.data(), GL_DYNAMIC_DRAW);

    {
        orthoID = glGetUniformLocation(shaderProgram, "orthoProjection");
        screenSizeID = glGetUniformLocation(shaderProgram, "screenSize");
    }

    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D);
    glDepthFunc(GL_GREATER);

    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR c: " << error << "\n";
    }


    orthoID = glGetUniformLocation(shaderProgram, "orthoProjection");
    Matrix4f orthoProjection = orthographicProjection(
        0, winWidth, 0, -winHeight);
    glUniformMatrix4fv(orthoID, 1, GL_FALSE, &orthoProjection.data[0][0]);

    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR a: " << error << "\n";
    }
    glUseProgram(shaderProgram);
    return true;
}


bool compileShaders(BumpAllocator* bump) {
    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    int fileSize = 0;
    char* vertShader = readFile(const_cast<char*>("assets/quad.vert"), &fileSize, bump);
    char* fragShader = readFile(const_cast<char*>("assets/quad.frag"), &fileSize, bump);

    if (!vertShader || !fragShader)
    {
        std::cerr << "failed to load shaders";
        return false;
    }
    GLenum error = glGetError();
    if (error)
    {
        std::cout << "GLERROR compile.1: " << error << "\n";
    }


    glShaderSource(vertShaderID, 1, &vertShader, 0);
    glCompileShader(vertShaderID);
    {
        int success{};
        char infoLog[512];
        glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    error = glGetError();
    glAttachShader(shaderProgram, vertShaderID);

    glShaderSource(fragShaderID, 1, &fragShader, 0);
    glCompileShader(fragShaderID);
    {
        int success{};
        char infoLog[512];
        glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShaderID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    glAttachShader(shaderProgram, fragShaderID);
    glLinkProgram(shaderProgram);
    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR c.4: " << error << "\n";
    }

    {
        int success{};
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
    glUseProgram(shaderProgram);

    glDetachShader(shaderProgram, vertShaderID);
    glDetachShader(shaderProgram, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    return true;
}

void openGLRender() {

    glUseProgram(shaderProgram);
	GLenum error{};
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform2fv(screenSizeID, 1, &screenSize.x);
	Matrix4f orthoProjection = orthographicProjection(
		-250.0f, 250.0f, -200.0f, 200.0f);
	glUniformMatrix4fv(orthoID, 1, GL_FALSE, &orthoProjection.data[0][0]);
	{
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * renderData->transforms.capacity(), renderData->transforms.data());
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transforms.size());
		error = glGetError();
		if (error)
		{
			std::cout << "GLERROR l2: " << error << "\n";
		}

		renderData->transforms.clear();
	}
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR l1: " << error << "\n";
	}
}