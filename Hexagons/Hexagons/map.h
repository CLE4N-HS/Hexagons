#pragma once
#include "tools.h"
#include "windowManager.h"

#define MAP_X_SIZE 17
#define MAP_Y_SIZE 13

typedef struct {
	sfVector2f pos;
}Block;

typedef struct {
	Block** block;
}Map;

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);
