#pragma once
#include "utils.h"
#include <iostream >
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "input.h"

constexpr int KEYCOUNT = 3;

extern GLFWwindow* window;
extern const float winHeight;
extern const float winWidth;



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void updateKeyState()
{
	for (size_t i = 0; i < GAME_INPUT_COUNT; ++i)
	{
		for (size_t j = 0; j < KEYCOUNT; ++j)
		{
			int state = glfwGetKey(window, gameState.keyMappings[i].keys[j].KeyCode);
			if (state == GLFW_PRESS)
			{
				gameState.keyMappings[i].keys[j].State = KEY_DOWN;
			}
			else 
			{
				gameState.keyMappings[i].keys[j].State = KEY_UP;
			}

		}
	}
}

bool pollAction(GameInputType inputType, KeyState state)
{
	KeyMapping mapping = gameState.keyMappings[inputType];

	for (size_t i = 0; i < mapping.keys.size(); i++)
	{
		if (mapping.keys[i].State == state)
		{
			return true;
		}
	}
	return false;
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mPos.x = (xpos - (winWidth / 2)) / (winWidth / renderData->gameCamera.dimensions.x);
	mPos.y = -(ypos  - winHeight / 2) / (winHeight / renderData->gameCamera.dimensions.y);
}

void initializeInputs() {
	gameState.keyMappings[MOVE_DOWN].keys.push_back(Key(GLFW_KEY_S));
	gameState.keyMappings[MOVE_UP].keys.push_back(Key(GLFW_KEY_W));
	gameState.keyMappings[MOVE_RIGHT].keys.push_back(Key(GLFW_KEY_D));
	gameState.keyMappings[MOVE_LEFT].keys.push_back(Key(GLFW_KEY_A));
	gameState.keyMappings[ATTACK_1].keys.push_back(Key(GLFW_KEY_1));
}