#pragma once
#include "windowManager.h"

typedef enum {
	KEY_STATE_HAS_RELEASED,
	KEY_STATE_RELEASED,
	KEY_STATE_PRESSED,
	KEY_STATE_HAS_PRESSED
}KeyState;

void initKeyboard();
void updateKeyboard(Window* _window);
sfBool isKeyPressed(sfKeyCode _key);
KeyState getKeyState(sfKeyCode _key);
