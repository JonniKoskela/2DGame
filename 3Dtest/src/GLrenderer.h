#pragma once
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "action/attackActionStructs.h"
#include <vector>
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
GLuint fadeDurationLocation{};
GLuint currentTimeLocation{};
GLuint attackFlagLocation{};




bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);
void renderArc();
void renderSlam();
void renderAttack(int attackType);


void genArcBuffer(std::vector<ArcVertex> arcVertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ArcVertex) * arcVertices.size(), arcVertices.data(), GL_STATIC_DRAW);
}

void genSlamBuffer(std::vector<SlamVertex> slamVertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, slamVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SlamVertex) * slamVertices.size(), slamVertices.data(), GL_STATIC_DRAW);
}