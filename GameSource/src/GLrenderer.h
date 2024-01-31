#pragma once
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "action/attackActionStructs.h"
#include "GLrenderer_attackQueue.hpp"

GLuint orthoID{};
GLuint screenSizeID{};
GLuint transformSBO{};
GLuint shaderProgram{};
GLuint texture{};
GLuint objectTypeID{};

GLuint arcVBO{};
GLuint slamVBO{};

GLuint arcShader{};
GLuint arcShaderProjection{};
GLuint arcFadeDurationLocation{};
GLuint slamFadeDurationLocation{};
GLuint currentTimeLocation{};
GLuint attackFlagLocation{};
GLuint slamDurationLocation{};

static bool renderSlamBool{ false };



void arcRenderData(std::vector<attackVertex> arcVertices, float arcTimer)
{
	glUniform1f(arcFadeDurationLocation, 0.7f);
	glUniform1f(currentTimeLocation, arcTimer);
	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(attackVertex) * arcVertices.size(), arcVertices.data(), GL_STATIC_DRAW);
	GLenum error = glGetError();
	if (error)
	{
		std::cout << "GLERROR SLAMRENDERDATA " << "\n";
	}
}

void slamRenderData(std::vector<attackVertex> slamVertices, float slamTimer)
{
	glUniform1f(slamFadeDurationLocation, 1.5f);
	glUniform1f(slamDurationLocation, slamTimer);
	glBindBuffer(GL_ARRAY_BUFFER, slamVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(attackVertex) * slamVertices.size(), slamVertices.data(), GL_DYNAMIC_DRAW);

	GLenum error = glGetError();
	if (error)
	{
		std::cout << "GLERROR SLAMRENDERDATA " << "\n";
	}
}

