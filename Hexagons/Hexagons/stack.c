#include "stack.h"

void displayStack(Window* _window, Stack* _stack)
{
	for (int i = 0; i < _stack->nbTiles; i++)
	{
		drawTile(_window, &_stack->tile[i]);
	}
}

void createStack(Stack* _stack)
{
	_stack->nbTiles = STACK_START_NB_TILES;

	_stack->tile = (Tile*)realloc(_stack->tile, _stack->nbTiles * sizeof(Tile));

	sfVector2f tilePos = STACK_START_POS;

	for (int i = 0; i < _stack->nbTiles; i++)
	{
		createTile(&_stack->tile[i], tilePos, TILE_STATE_PLACED);
		tilePos.y += TILE_START_SIZE;
		debugRandomiseDivisionsTypes(&_stack->tile[i]);
	}
}
