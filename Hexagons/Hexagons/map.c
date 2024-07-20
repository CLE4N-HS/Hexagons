#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"
#include "tile.h"
#include "stack.h"
#include "hand.h"

typedef struct {
	sfVector2i size;
	Tile** tile;
	Stack stack;
	Hand hand;
	sfVector2i mouseHoverIndex;
}Map;
Map map;

void initMap()
{
	createMap();
}

void updateMap(Window* _window)
{
	sfVector2f mousePos = getMousePos();



	//updateTile(_window, map.tile);
	sfVector2i closestTileIndex = vector2i(-1, -1);
	float closestTileDistance = (float)INT_MAX;

	for (int j = 0; j < map.size.y; j++)
	{
		for (int i = 0; i < map.size.x; i++)
		{
			float tmpDistance = getSqrMagnitude(mousePos, map.tile[j][i].pos);
			if (tmpDistance < closestTileDistance) {
				closestTileDistance = tmpDistance;
				closestTileIndex = vector2i(i, j);
			}
		}
	}

	if (mousePos.y <= HAND_MIN_Y_POS && isIndexInMap(closestTileIndex) && isPointInHexagonTile(mousePos, map.tile[closestTileIndex.y][closestTileIndex.x])) {
		map.mouseHoverIndex = closestTileIndex;
	}
	else
		map.mouseHoverIndex = vector2i(-1, -1);

	if (isIndexInMap(map.mouseHoverIndex) && getMouseButtonState(sfMouseLeft) == MOUSE_STATE_HAS_PRESSED) {
		setTileState(&map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x], TILE_STATE_PLACED);
		debugRandomiseDivisionsTypes(&map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x]);
	}




	//updateHand(_window, &map.hand);
	int closestHandIndex = 0;
	float closestHandDistance = getSqrMagnitude(mousePos, map.hand.tile[closestHandIndex].pos);

	for (int i = 0; i < map.hand.nbTiles; i++)
	{
		float tmpDistance = getSqrMagnitude(mousePos, map.hand.tile[i].pos);
		if (tmpDistance < closestHandDistance) {
			closestHandDistance = tmpDistance;
			closestHandIndex = i;
		}
	}

	MouseButtonState mouseLeftState = getMouseButtonState(sfMouseLeft);

	switch (map.hand.state)
	{
	case HAND_STATE_LOOKING:
		if (isPointInHexagonTile(mousePos, map.hand.tile[closestHandIndex])) {
			map.hand.mouseHoverIndex = closestHandIndex;
			if (mouseLeftState >= MOUSE_STATE_PRESSED) { // selecting the tile
				map.hand.state = HAND_STATE_MOVING;
				map.hand.tile[map.hand.mouseHoverIndex].radius = TILE_START_RADIUS * HAND_TILE_HOVER_SCALE;
			}
			else { // just looking / hovering the tile
				setTilePosAndRadius(&map.hand.tile[map.hand.mouseHoverIndex], vector2f(map.hand.tile[map.hand.mouseHoverIndex].pos.x, HAND_START_POS.y - TILE_START_RADIUS * HAND_TILE_HOVER_SCALE / 2.f + 9.f), TILE_START_RADIUS * HAND_TILE_HOVER_SCALE); // TODO +9.f is a fraud
			}
		}
		else {
			if (map.hand.mouseHoverIndex >= 0) {
				map.hand.tile[map.hand.mouseHoverIndex].radius = TILE_START_RADIUS;
				setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], vector2f(map.hand.tile[map.hand.mouseHoverIndex].pos.x, HAND_START_POS.y));
			}

			map.hand.mouseHoverIndex = -1;
		}
		break;
	case HAND_STATE_MOVING:
		setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], mousePos);

		if (mouseLeftState == MOUSE_STATE_RELEASED) {
			map.hand.state = HAND_STATE_LOOKING;
			map.hand.tile[map.hand.mouseHoverIndex].radius = TILE_START_RADIUS;

			setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], vector2f(map.hand.tile[map.hand.mouseHoverIndex].pos.x, HAND_START_POS.y));
		}
		else if (mousePos.y < HAND_MIN_Y_POS) {
			map.hand.state = HAND_STATE_PLACING;
			map.hand.tile[map.hand.mouseHoverIndex].radius = TILE_START_RADIUS;
		}
		break;
	case HAND_STATE_PLACING:
		if (mouseLeftState == MOUSE_STATE_RELEASED) {
			map.hand.state = HAND_STATE_LOOKING;
			map.hand.tile[map.hand.mouseHoverIndex].radius = TILE_START_RADIUS;
			if (isIndexInMap(map.mouseHoverIndex)) {
				setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x].pos);
				map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x] = map.hand.tile[map.hand.mouseHoverIndex];
			}
			else {
				setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], vector2f(map.hand.tile[map.hand.mouseHoverIndex].pos.x, HAND_START_POS.y));
			}
		}
		else if (mousePos.y >= HAND_MIN_Y_POS) {
			map.hand.state = HAND_STATE_MOVING;
		}
		else {
			setTilePos(&map.hand.tile[map.hand.mouseHoverIndex], mousePos);
		}
		break;
	default:
		break;
	}
}

void displayMap(Window* _window)
{
	//displayTile(_window, map.tile);
	for (int j = 0; j < map.size.y; j++)
	{
		for (int i = 0; i < map.size.x; i++)
		{
			drawTile(_window, &map.tile[j][i]);
		}
	}

	if (isIndexInMap(map.mouseHoverIndex)) {
		drawTileHover(_window, &map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x]);
	}






	//displayStack(_window, &map.stack);
	for (int i = 0; i < map.stack.nbTiles; i++)
	{
		drawTile(_window, &map.stack.tile[i]);
	}





	//displayHand(_window, &map.hand);
	for (int i = 0; i < map.hand.nbTiles; i++)
	{
		drawTile(_window, &map.hand.tile[i]);
	}

	if (map.hand.mouseHoverIndex >= 0) {
		drawTileHover(_window, &map.hand.tile[map.hand.mouseHoverIndex]);
	}

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
	createHand(&map.hand);
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
			//sfVector2f tilePos = addVectorsf(tileStartPos, vector2f((float)i * (TILE_START_SIZE + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothemD2 + TILE_OFFSET_POS)));
			//
			//if (j % 2 == 0)
			//	tilePos.x += TILE_START_SIZE * 0.75f + TILE_OFFSET_POS;
			//
			//createTile(&map.tile[j][i], tilePos);

			sfVector2f tilePos = addVectorsf(tileStartPos, vector2f((float)i * (TILE_START_RADIUS + TILE_OFFSET_POS) * 1.5f, (float)j * (tileSqrApothem + TILE_OFFSET_POS)));
			
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
