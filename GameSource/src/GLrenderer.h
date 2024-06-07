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
const char* TILEMAP_ATLAS_PATH = "assets/tileMapAtlas.png";

void generateMapTexture(BumpAllocator*);
bool fillMapBuffer();
bool compileShaders(BumpAllocator*);
bool compileArcShaders(BumpAllocator*);
bool compileMapShaders(BumpAllocator*);

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
GLuint mapDataSSBO{};
GLuint mapShader{};
GLuint mapHeightUniform{};
GLuint mapWidthUniform{};
GLuint mapProjectionLocation{};
GLuint tileMapAtlasLocation{};
GLint  textureArray[5];

static bool renderSlamBool{ false };





