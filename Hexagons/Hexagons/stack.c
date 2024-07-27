#include "stack.h"

void displayStack(Window* _window, Stack* _stack)
{
	// TODO draw nbTiles
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

Tile getStackTopTile(Stack* _stack)
{
	return _stack->tile[0];
}

void removeStackIndex(Stack* _stack, int _index)
{
	for (int i = _index; i < _stack->nbTiles - 1; i++)
	{
		_stack->tile[i] = _stack->tile[i + 1];
	}

	_stack->tile = (Tile*)realloc(_stack->tile, (_stack->nbTiles--) * sizeof(Tile));
}
