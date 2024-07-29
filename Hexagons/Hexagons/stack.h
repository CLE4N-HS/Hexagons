#pragma once
#include "windowManager.h"
#include "tile.h"

#define STACK_START_NB_TILES 30
#define STACK_START_POS vector2f(WINDOW_LENGTH - TILE_START_SIZE, WINDOW_HEIGHT - TILE_START_SIZE - TILE_START_RADIUS / 2.f)

#define STACK_NB_TILES_POS addVectorsf(STACK_START_POS, vector2f(-TILE_START_RADIUS / 2.f, TILE_START_RADIUS))
#define STACK_NB_TILES_COLOR sfWhite
#define STACK_NB_TILES_SIZE vector2f(30.f, 60.f)
#define STACK_NB_TILES_SPACE vector2f(STACK_NB_TILES_SIZE.x * 1.5f, 0.f)

typedef struct {
	Tile* tile;
	int nbTiles;
}Stack;

void displayStack(Window* _window, Stack* _stack);
void createStack(Stack* _stack);
Tile getStackTopTile(Stack* _stack);
void removeStackIndex(Stack* _stack, int _index);
void drawStackNbTiles(Window* _window, Stack* _stack);
