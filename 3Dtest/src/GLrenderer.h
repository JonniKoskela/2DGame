#pragma once
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "attacks.hpp"
#include <vector>
GLuint orthoID{};
GLuint screenSizeID{};
GLuint transformSBO{};
GLuint shaderProgram{};
GLuint texture{};
GLuint arcVBO{};
GLuint objectTypeID{};


GLuint arcShader{};
GLuint arcShaderProjection{};
GLuint fadeDurationLocation{};
GLuint currentTimeLocation{};


void renderArc(std::vector<ArcVertex> arcVertices)
{
	glGenBuffers(1, &arcVBO);
	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ArcVertex) * arcVertices.size(), arcVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, arcVBO);
}
