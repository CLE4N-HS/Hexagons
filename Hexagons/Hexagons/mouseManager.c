#include "mouseManager.h"

typedef struct {
	sfVector2f pos;
	MouseButtonState buttonState[sfMouseButtonCount];
}Mouse;
Mouse mouse;

void initMouse()
{
	mouse.pos = NULLVECTOR2F;
	
	for (int i = 0; i < sfMouseButtonCount; i++)
	{
		mouse.buttonState[i] = MOUSE_STATE_RELEASED;
	}
}

void updateMouse(Window* _window)
{
	mouse.pos = V2iToV2f(sfMouse_getPositionRenderWindow(_window->renderWindow));

	for (int i = 0; i < sfMouseButtonCount; i++)
	{
		if (sfMouse_isButtonPressed(i)) {
			if (mouse.buttonState[i] <= MOUSE_STATE_RELEASED)
				mouse.buttonState[i] = MOUSE_STATE_HAS_PRESSED;
			else
				mouse.buttonState[i] = MOUSE_STATE_PRESSED;
		}
		else {
			if (mouse.buttonState[i] >= MOUSE_STATE_PRESSED)
				mouse.buttonState[i] = MOUSE_STATE_HAS_RELEASED;
			else
				mouse.buttonState[i] = MOUSE_STATE_RELEASED;
		}
	}
}

sfBool isMouseButtonPressed(sfMouseButton _button)
{
	return (mouse.buttonState[_button] >= MOUSE_STATE_PRESSED ? sfTrue : sfFalse);
}

MouseButtonState getMouseButtonState(sfMouseButton _button)
{
	return mouse.buttonState[_button];
}

sfVector2f getMousePos()
{
	return mouse.pos;
}
