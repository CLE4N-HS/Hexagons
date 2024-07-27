#pragma once
#include "windowManager.h"
#include "tile.h"

#define STACK_START_NB_TILES 30
#define STACK_START_POS vector2f(WINDOW_LENGTH - TILE_START_SIZE - TILE_START_RADIUS, 0.f)

typedef struct {
	Tile* tile;
	int nbTiles;
}Stack;

void displayStack(Window* _window, Stack* _stack);
void createStack(Stack* _stack);
Tile getStackTopTile(Stack* _stack);
void removeStackIndex(Stack* _stack, int _index);
