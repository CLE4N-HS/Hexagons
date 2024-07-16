#pragma once
#include "windowManager.h"
#include "tile.h"

#define MAP_X_SIZE 17
#define MAP_Y_SIZE 13

typedef struct {
	Tile** tile;
}Map;

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);
void createMap();
void resetMap();
