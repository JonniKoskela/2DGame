#pragma once

static Vec2 mPos{};


enum GameInputType
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	JUMP,

	MOUSE_LEFT,
	MOUSE_RIGHT,

	PAUSE,
	SHIFT,
	ATTACK_1,
	ATTACK_2,
	GAME_INPUT_COUNT
};

enum KeyState
{
	KEY_DOWN,
	KEY_UP
};



class Key
{
public:
	int KeyCode{};
	KeyState State{};

	Key(int keyCode)
	{
		KeyCode = keyCode;
	}
};

class KeyMapping
{
public:
	std::vector<Key> keys{};

	KeyMapping()
	{
		keys.reserve(3);
	}
};