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
#include "action/attacks.hpp"
#include "game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
static Vec2 screenSize{ 1280.0f,720.0f };







bool glInit
(BumpAllocator* bump)
{
    GLenum error = glGetError();
	attackTransforms.reserve(1000);
    if (!compileShaders(bump))
    {
        std::cerr << "failed to compile shaders" << "\n";
    }
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR a: " << error << "\n";
	}
	//compile mapShader
	{
		compileMapShaders(bump);
		generateMapTexture(bump);
		fillMapBuffer();

		GLenum error = glGetError();
		if (error)
		{
			std::cout << "GLERROR map setup: general " << error << "\n";
		}
	}
	//compile attackShader and get uniforms
	{

		compileArcShaders(bump);
		glUseProgram(arcShader);
		glUniform3f(glGetUniformLocation(arcShader, "arcColor"), 1.0f, 1.0f, 1.0f);
		arcShaderProjection = glGetUniformLocation(arcShader, "orthoProjection");
		currentTimeLocation = glGetUniformLocation(arcShader, "currentTime");
		slamFadeDurationLocation = glGetUniformLocation(arcShader, "slamFadeDuration");
		attackFlagLocation = glGetUniformLocation(arcShader, "attackFlag");
		slamDurationLocation = glGetUniformLocation(arcShader, "slamDuration");
		arcFadeDurationLocation = glGetUniformLocation(arcShader, "arcFadeDuration");
		rotationMatrixLocation = glGetUniformLocation(arcShader, "rotationMatrix");

		tileMapAtlasLocation = glGetUniformLocation(mapShader,"tileMapAtlas");


		error = glGetError();
		if (error)
		{
			std::cout << "GLERROR arc: " << error << "\n";
		}
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

		{
			glUseProgram(shaderProgram);
			glGenTextures(1, &textureAtlas_01);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAtlas_01);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		{
			glUseProgram(arcShader);
			glGenTextures(1, &arcShaderTexture);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, arcShaderTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
    }
	glUseProgram(shaderProgram);
    glGenBuffers(1, &transformSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, transformSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * renderData->transforms.capacity(), renderData->transforms.data(), GL_DYNAMIC_DRAW);

	glUseProgram(arcShader);
	glGenBuffers(1, &attackTransformsSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, attackTransformsSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RenderData4xVec2) * attackTransforms.capacity(), attackTransforms.data(), GL_DYNAMIC_DRAW);

	glUseProgram(shaderProgram);
    {
        orthoID = glGetUniformLocation(shaderProgram, "orthoProjection");
        screenSizeID = glGetUniformLocation(shaderProgram, "screenSize");
    }
	glUseProgram(arcShader);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D);
    glDepthFunc(GL_GREATER);

    error = glGetError();
    if (error)
    {
        std::cout << "GLERROR c: " << error << "\n";
    }
    orthoID = glGetUniformLocation(shaderProgram, "orthoProjection");
    Matrix4f orthoProjection = orthographicProjection( 0, screenSize.x, 0, -screenSize.y );
    glUniformMatrix4fv(orthoID, 1, GL_FALSE, &orthoProjection.data[0][0]);


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
		//Matrix2f ts{};
		//ts.data[0][0] = 0;
		//ts.data[1][0] = 1;
		//ts.data[0][1] = 1;
		//ts.data[1][1] = 0;
		//orthoProjection = orthoProjection * ts;
	{

		glUseProgram(shaderProgram);
		glViewport(0, 0, screenSize.x, screenSize.y);
		glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
		glClearDepth(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		error = glGetError();
		if (error)
		{
			std::cout << "GLERROR l2: " << error << "\n";
		}
		glUniform2fv(screenSizeID, 1, &screenSize.x);
		glUniformMatrix4fv(orthoID, 1, GL_FALSE, &orthoProjection.data[0][0]);
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, transformSBO);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * renderData->transforms.capacity(), renderData->transforms.data());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transforms.size());
			error = glGetError();
			if (error)
			{
				std::cout << "GLERROR l3: " << error << "\n";
			}

			renderData->transforms.clear();
		}
		if (attackTransforms.size() > 0)
		{
			glUseProgram(arcShader);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, attackTransformsSBO);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, attackTransformsSBO);
			glUniformMatrix4fv(arcShaderProjection, 1, GL_FALSE, &orthoProjection.data[0][0]);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(RenderData4xVec2) * attackTransforms.capacity(), attackTransforms.data());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, attackTransforms.size());
			error = glGetError();
			if (error)
			{
				std::cout << "GLERROR attack: " << error << "\n";
			}
			attackTransforms.clear();
		}
		if (error)
		{
			std::cout << "GLERROR mapDraw b4: " << error << "\n";
		}
		//rendermap
		{
			glUseProgram(mapShader);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, mapDataSSBO);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mapDataSSBO);
			//glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * gameState.currentMap.tileLayer.size(), gameState.currentMap.tileLayer.data());
			error = glGetError();
			if (error)
			{
				std::cout << "GLERROR mapDraw 1: " << error << "\n";
			}
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mapDataSSBO);
			glUniformMatrix4fv(mapProjectionLocation, 1, GL_FALSE, &orthoProjection.data[0][0]);
			error = glGetError();
			if (error)
			{
				std::cout << "GLERROR mapDraw 2: " << error << "\n";
			}
			//glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t) * gameState.currentMap.tileLayer.size(), gameState.currentMap.tileLayer.data());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, gameState.currentMap.tileLayer.size());
			error = glGetError();
			if (error)
			{
				std::cout << "GLERROR mapDraw 3: " << error << "\n";
			}
		}
	}
	
	error = glGetError();
	if(error)
	{
		std::cout << "GLERROR l1: " << error << "\n";
	}
}



bool compileShaders(BumpAllocator* bump)
{
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLenum error = glGetError();
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	error = glGetError();
	shaderProgram = glCreateProgram();
	int fileSize = 0;
	const char* vertShader = readFile(const_cast<char*>("assets/quad.vert"), &fileSize, bump);
	const char* fragShader = readFile(const_cast<char*>("assets/quad.frag"), &fileSize, bump);
	error = glGetError();
	if (!vertShader || !fragShader)
	{
		std::cerr << "failed to load shaders";
		return false;
	}
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR compile.1: general " << error << "\n";
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
	const char* vertShader = readFile(const_cast<char*>("assets/arc.vert"), &fileSize, bump);
	const char* fragShader = readFile(const_cast<char*>("assets/arc.frag"), &fileSize, bump);

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



bool compileMapShaders(BumpAllocator* bump)
{
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	mapShader = glCreateProgram();

	int fileSize = 0;
	const char* vertShader = readFile(const_cast<char*>("assets/mapShader.vert"), &fileSize, bump);
	const char* fragShader = readFile(const_cast<char*>("assets/mapShader.frag"), &fileSize, bump);

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
	glAttachShader(mapShader, vertShaderID);

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
	glAttachShader(mapShader, fragShaderID);
	glLinkProgram(mapShader);
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR c.4: " << error << "\n";
	}

	{
		int success{};
		char infoLog[512];
		glGetProgramiv(mapShader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mapShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	glUseProgram(mapShader);

	glDetachShader(mapShader, vertShaderID);
	glDetachShader(mapShader, fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	return true;
}



bool fillMapBuffer()
{
	glUseProgram(mapShader);
	glGenBuffers(1, &mapDataSSBO);
	mapHeightUniform = glGetUniformLocation(mapShader,"mapHeight");
	mapWidthUniform = glGetUniformLocation(mapShader, "mapWidth");

	bool validData = (gameState.currentMap.map_height != 0 && gameState.currentMap.map_width != 0);
	assert(validData == true, "mapData not valid");

	glUniform1ui(mapHeightUniform, gameState.currentMap.map_height);
	glUniform1ui(mapWidthUniform, gameState.currentMap.map_width);
	mapProjectionLocation = glGetUniformLocation(mapShader,"orthoProjection");

	GLenum error = glGetError();
	if (error)
	{
		std::cout << "GLERROR mapBuffer 1: " << error << "\n";
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mapDataSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mapDataSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t) * gameState.currentMap.tileLayer.size(), gameState.currentMap.tileLayer.data(),GL_STATIC_DRAW);

	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR mapBuffer 2: " << error << "\n";
	}
	return true;
}



void generateMapTexture(BumpAllocator* bump)
{
	glUseProgram(mapShader);
	GLenum error;
	{
		int width, height, channels;
		char* data = (char*)stbi_load(TILEMAP_ATLAS_PATH, &width, &height, &channels, 4);
		assert(data && "stbi_load failure");
		error  = glGetError();
		if (error)
		{
			std::cout << "GLERROR generating mapBuffer:1 " << error << "\n";
		}
		{
			glActiveTexture(GL_TEXTURE1);
			glGenTextures(1, &mapTextureFile);


			glBindTexture(GL_TEXTURE_2D, mapTextureFile);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
	}
	glUniform1i(tileMapAtlasLocation, mapTextureFile);
	error = glGetError();
	if (error)
	{
		std::cout << "GLERROR generating mapBuffer: " << error << "\n";
	}

}