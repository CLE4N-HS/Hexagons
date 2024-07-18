#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"

typedef struct {
	sfVector2i size;
	Tile** tile;
	Stack stack;
	sfVector2i mouseHoverIndex;
}Map;
Map map;

void initMap()
{
	createMap();
}

void updateMap(Window* _window)
{
	updateTile(_window, map.tile);
}

void displayMap(Window* _window)
{
	displayTile(_window, map.tile);
	displayStack(_window, &map.stack);
}

void createMap()
{
	map.size = MAP_START_SIZE;

	map.tile = (Tile**)calloc(map.size.y, sizeof(Tile));

	for (int j = 0; j < map.size.y; j++)
	{
		map.tile[j] = (Tile*)calloc(map.size.x, sizeof(Tile));
	}

	resetMap();
	createStack(&map.stack);
}

void resetMap()
{
	map.mouseHoverIndex = vector2i(-1, -1);

	float tileSqrApothem = getTileSqrApothem();
	float halfTileSqrApothem = tileSqrApothem / 2.f;
	sfVector2f tileStartPos = TILE_START_POS;

	for (int j = 0; j < map.size.y; j++)
	{
		for (int i = 0; i < map.size.x; i++)
		{
			//sfVector2f tilePos = addVectorsf(tileStartPos, vector2f((float)i * (TILE_SIZE + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothemD2 + TILE_OFFSET_POS)));
			//
			//if (j % 2 == 0)
			//	tilePos.x += TILE_SIZE * 0.75f + TILE_OFFSET_POS;
			//
			//createTile(&map.tile[j][i], tilePos);

			sfVector2f tilePos = addVectorsf(tileStartPos, vector2f((float)i * (TILE_RADIUS + TILE_OFFSET_POS) * 1.5f, (float)j * (tileSqrApothem + TILE_OFFSET_POS)));
			
			if (i % 2 == 0)
				tilePos.y += halfTileSqrApothem;
			
			createTile(&map.tile[j][i], tilePos, TILE_STATE_VOID);
		}
	}
}

sfBool isIndexInMap(sfVector2i _index)
{
	return ((_index.x >= 0 && _index.x < map.size.x && _index.y >= 0 && _index.y < map.size.y) ? sfTrue : sfFalse);
}

sfVector2i getMapSize()
{
	return map.size;
}

sfVector2i getMapMouseHoverIndex()
{
	return map.mouseHoverIndex;
}

void setMapMouseHoverIndex(sfVector2i _mouseHoverIndex)
{
	map.mouseHoverIndex = _mouseHoverIndex;
}
