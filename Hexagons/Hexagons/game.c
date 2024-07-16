#include "game.h"
#include "map.h"
#include "mouseManager.h"
#include "keyboardManager.h"

void initGame(Window* _window)
{
	initMap();
	initMouse();
	initKeyboard();
}

void updateGame(Window* _window)
{
	updateMouse(_window);
	updateKeyboard(_window);
	updateMap(_window);
}

void displayGame(Window* _window)
{
	displayMap(_window);
}
