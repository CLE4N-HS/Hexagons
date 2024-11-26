#include "keyboardManager.h"

typedef struct {
	KeyState keyState[sfKeyCount];
}Keyboard;
Keyboard keyboard;

void initKeyboard()
{
	for (int i = 0; i < sfKeyCount; i++)
	{
		keyboard.keyState[i] = KEY_STATE_RELEASED;
	}
}

void updateKeyboard(Window* _window)
{
	for (int i = 0; i < sfKeyCount; i++)
	{
		if (sfKeyboard_isKeyPressed(i)) {
			if (keyboard.keyState[i] <= KEY_STATE_RELEASED)
				keyboard.keyState[i] = KEY_STATE_HAS_PRESSED;
			else
				keyboard.keyState[i] = KEY_STATE_PRESSED;
		}
		else {
			if (keyboard.keyState[i] >= KEY_STATE_PRESSED)
				keyboard.keyState[i] = KEY_STATE_HAS_RELEASED;
			else
				keyboard.keyState[i] = KEY_STATE_RELEASED;
		}
	}
}

sfBool isKeyPressed(sfKeyCode _key)
{
	return (keyboard.keyState[_key] >= KEY_STATE_PRESSED ? sfTrue : sfFalse);
}

KeyState getKeyState(sfKeyCode _key)
{
	return keyboard.keyState[_key];
}
