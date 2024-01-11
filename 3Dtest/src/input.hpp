#pragma once
#include "utils.h"
#include "game.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern Vec2 mPos;


namespace input
{
	
	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_S)
		{
			playerPos.y += 1.0f;
		}
		if (key == GLFW_KEY_A)
		{
			playerPos.x -= 1.0f;
		}
	}
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		mPos.x = xpos;
		mPos.y = winHeight - ypos;
	}
}