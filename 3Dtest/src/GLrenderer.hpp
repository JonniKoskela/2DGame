#pragma once

//----glm----
#include <glm.hpp>
#include <matrix.hpp>
#include <chrono>
#include <gtc/matrix_transform.hpp>

#include "GLrenderer.h"
#include <math.h>
#include "input.h"
#include "camera.h"
#include "attacks.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
static Vec2 screenSize{ 960.0f, 540.0f };
const char* TEXTURE_PATH = "assets/textureAtlas.png";





bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);
void renderArc();





bool glInit(BumpAllocator* bump)
{
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

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
        0, screenSize.x, 0, -screenSize.y);
    glUniformMatrix4fv(orthoID, 1, GL_FALSE, &orthoProjection.data[0][0]);

    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR a: " << error << "\n";
    }
	{
		glUseProgram(shaderProgram);

		compileArcShaders(bump);
		glUseProgram(arcShader);

		glUniform3f(glGetUniformLocation(arcShader, "arcColor"), 1.0f, 1.0f, 1.0f);
		arcShaderProjection = glGetUniformLocation(arcShader, "orthoProjection");
		currentTimeLocation = glGetUniformLocation(arcShader, "currentTime");
		fadeDurationLocation = glGetUniformLocation(arcShader, "fadeDuration");
		glUniform1f(fadeDurationLocation, arcFadeTime);


		error = glGetError();
		if (error)
		{
			std::cout << "GLERROR arc: " << error << "\n";
		}
	}

	//generate arc buffer

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}




void openGLRender()
{
		GLenum error{};
		OrtographicCamera camera = renderData->gameCamera;

		Matrix4f orthoProjection = orthographicProjection(
			camera.position.x - camera.dimensions.x / 2.0f,
			camera.position.x + camera.dimensions.x / 2.0f,
			camera.position.y + camera.dimensions.y / 2.0f,
			camera.position.y - camera.dimensions.y / 2.0f);
	{

		glUseProgram(shaderProgram);
		glViewport(0, 0, screenSize.x, screenSize.y);
		glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
		glClearDepth(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform2fv(screenSizeID, 1, &screenSize.x);
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
	}
	if(drawArc)
	{
		glUseProgram(arcShader);
        glUniformMatrix4fv(arcShaderProjection, 1, GL_FALSE, &orthoProjection.data[0][0]);
		renderArc();
	}

	error = glGetError();
	if(error)
	{
		std::cout << "GLERROR l1: " << error << "\n";
	}
}



void renderArc() {
	arcTimer += getCurrentTime();
	//std::cout << arcTimer << "\n";
	glUniform1f(currentTimeLocation, arcTimer);

	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ArcVertex), (void*)0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, arcVertexCapacity);
}



















bool compileShaders(BumpAllocator* bump)
{
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
		if (!success)
		{
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



bool compileArcShaders(BumpAllocator* bump)
{
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	arcShader = glCreateProgram();

	int fileSize = 0;
	char* vertShader = readFile(const_cast<char*>("assets/arc.vert"), &fileSize, bump);
	char* fragShader = readFile(const_cast<char*>("assets/arc.frag"), &fileSize, bump);

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
	glAttachShader(arcShader, vertShaderID);

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
	glAttachShader(arcShader, fragShaderID);
	glLinkProgram(arcShader);
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR c.4: " << error << "\n";
	}

	{
		int success{};
		char infoLog[512];
		glGetProgramiv(arcShader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(arcShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	glUseProgram(arcShader);

	glDetachShader(arcShader, vertShaderID);
	glDetachShader(arcShader, fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	return true;
}
