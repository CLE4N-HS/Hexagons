#pragma once
#include "tools.h"

#define WINDOW_START_FULLSCREEN sfFalse

typedef struct {
	sfRenderWindow* renderWindow;
	sfRenderTexture* renderTexture;
	sfBool isDone;
	sfBool isFullscreen;
}Window;

Window* setupWindow();
void initWindow(Window* _window);
void updateWindow(Window* _window);
void displayWindow(Window* _window);
void createWindow(Window* _window);
void destroyWindow(Window* _window);
sfBool isDone(Window* _window);
void toggleFullscreen(Window* _window);
