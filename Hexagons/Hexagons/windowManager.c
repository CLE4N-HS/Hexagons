#include "windowManager.h"
#include "game.h"
#include "keyboardManager.h"

sfSprite* windowSprite;
sfTexture* windowTexture;

Window* setupWindow()
{
	Window* window = malloc(sizeof(Window));
	window->isFullscreen = WINDOW_START_FULLSCREEN;
	createWindow(window);
	window->renderTexture = sfRenderTexture_create(WINDOW_LENGTH, WINDOW_HEIGHT, sfFalse);
	window->isDone = sfFalse;

	return window;
}

void initWindow(Window* _window)
{
	windowSprite = sfSprite_create();
	windowTexture = sfTexture_create(WINDOW_LENGTH, WINDOW_HEIGHT);

	initTools();
	
	initGame(_window);
}

void updateWindow(Window* _window)
{
	restartClock();

	sfEvent event;

	while (sfRenderWindow_pollEvent(_window->renderWindow, &event))
	{
		if (event.type == sfEvtClosed || event.mouseButton.button == sfMouseMiddle)
			_window->isDone = sfTrue;
	}

	if (sfRenderWindow_hasFocus(_window->renderWindow)) {
		updateGame(_window);

		if (getKeyState(sfKeyF11) == KEY_STATE_HAS_PRESSED) {
			toggleFullscreen(_window);
			destroyWindow(_window);
			createWindow(_window);
		}
	}
}

void displayWindow(Window* _window)
{
	if (sfRenderWindow_hasFocus(_window->renderWindow)) {
		sfRenderWindow_clear(_window->renderWindow, sfBlack);
		sfRenderTexture_clear(_window->renderTexture, sfBlack);

		displayGame(_window);

		sfRenderTexture_display(_window->renderTexture);
		sfSprite_setTexture(windowSprite, sfRenderTexture_getTexture(_window->renderTexture), sfTrue);
		sfRenderWindow_drawSprite(_window->renderWindow, windowSprite, NULL);

		sfRenderWindow_display(_window->renderWindow);
	}
}

void createWindow(Window* _window)
{
	sfVideoMode mode = { WINDOW_LENGTH, WINDOW_HEIGHT, 32 };
	_window->renderWindow = sfRenderWindow_create(mode, "Hexagons", (_window->isFullscreen ? sfFullscreen : sfDefaultStyle), NULL);

	sfRenderWindow_setFramerateLimit(_window->renderWindow, 60);
}

void destroyWindow(Window* _window)
{
	sfRenderWindow_destroy(_window->renderWindow);
}

sfBool isDone(Window* _window)
{
	return _window->isDone;
}

void toggleFullscreen(Window* _window)
{
	_window->isFullscreen = !_window->isFullscreen;
}
