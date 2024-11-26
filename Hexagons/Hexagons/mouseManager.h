#pragma once
#include "windowManager.h"

typedef enum {
	MOUSE_STATE_HAS_RELEASED,
	MOUSE_STATE_RELEASED,
	MOUSE_STATE_PRESSED,
	MOUSE_STATE_HAS_PRESSED
}MouseButtonState;

void initMouse();
void updateMouse(Window* _window);
sfBool isMouseButtonPressed(sfMouseButton _button);
MouseButtonState getMouseButtonState(sfMouseButton _button);
sfVector2f getMousePos();
