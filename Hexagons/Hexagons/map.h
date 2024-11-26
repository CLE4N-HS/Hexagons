#pragma once
#include "windowManager.h"

#define MAP_START_SIZE vector2i(19, 8)

void initMap();
void updateMap(Window* _window);
void displayMap(Window* _window);
void createMap();
void resetMap();
sfBool isIndexInMap(sfVector2i _index);
sfVector2i getMapSize();
sfVector2i getMapMouseHoverIndex();
void setMapMouseHoverIndex(sfVector2i _mouseHoverIndex);
