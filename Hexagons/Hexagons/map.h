#pragma once
#include "windowManager.h"
#include "tile.h"

#define MAP_X_SIZE 20
#define MAP_Y_SIZE 10

#define MAP_START_POS vector2f(TILE_RADIUS, getTileSqrApothem() / 2.f);

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);
void createMap();
void resetMap();
sfBool isIndexInMap(sfVector2i _index);
