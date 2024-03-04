#pragma once
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "action/attackActionStructs.h"
#include "GLrenderer_attackQueue.hpp"
#include "utils.h"
#include <array>
#include "GLRenderer_attackRenderData.h"


const char* TEXTURE_PATH = "assets/textureAtlas.png";

void generateMapBuffer(BumpAllocator*);
bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);

GLuint attackTransformsSBO{};

GLuint orthoID{};
GLuint screenSizeID{};
GLuint transformSBO{};
GLuint shaderProgram{};

GLuint textureAtlas_01{};
GLuint arcShaderTexture{};

GLuint objectTypeID{};

GLuint arcShader{};
GLuint arcShaderProjection{};
GLuint arcFadeDurationLocation{};
GLuint slamFadeDurationLocation{};
GLuint currentTimeLocation{};
GLuint attackFlagLocation{};
GLuint slamDurationLocation{};
GLuint rotationMatrixLocation{};

GLuint mapTextureFile{};
GLuint mapBuffer{};

static bool renderSlamBool{ false };





