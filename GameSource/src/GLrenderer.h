#pragma once
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "action/attackActionStructs.h"
#include "GLrenderer_attackQueue.hpp"
#include "utils.h"
#include <array>
#include "GLRenderer_attackRenderData.h"

GLuint attackTransformsSBO{};

GLuint orthoID{};
GLuint screenSizeID{};
GLuint transformSBO{};
GLuint shaderProgram{};
GLuint texture{};
GLuint objectTypeID{};

GLuint arcVBO{};
GLuint slamVBO{};
GLuint movingArcVBO{};

GLuint arcShader{};
GLuint arcShaderProjection{};
GLuint arcFadeDurationLocation{};
GLuint slamFadeDurationLocation{};
GLuint currentTimeLocation{};
GLuint attackFlagLocation{};
GLuint slamDurationLocation{};
GLuint rotationMatrixLocation{};

static bool renderSlamBool{ false };





void arcRenderData(std::vector<Vec2> arcVertices, float arcTimer)
{
	glUniform1f(arcFadeDurationLocation, 0.7f);
	glUniform1f(currentTimeLocation, arcTimer);
	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * arcVertices.size(), arcVertices.data(), GL_STATIC_DRAW);
	GLenum error = glGetError();
	if (error)
	{
		std::cout << "GLERROR SLAMRENDERDATA " << "\n";
	}
}

void slamRenderData(std::vector<Vec2> slamVertices, float slamTimer, Vec2 originalPosition)
{
	glUniform1f(slamFadeDurationLocation, 1.5f);
	glUniform1f(slamDurationLocation, slamTimer);
	glBindBuffer(GL_ARRAY_BUFFER, slamVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * slamVertices.size(), slamVertices.data(), GL_DYNAMIC_DRAW);

	GLenum error = glGetError();
	if (error)
	{
		std::cout << "GLERROR SLAMRENDERDATA " << "\n";
	}
}


