#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"

typedef struct {
	sfVector2f startPos;
	sfVector2i mouseHoverIndex;
	Tile** tile;
	sfVertexArray* vertexArray;
}Map;
Map map;

void initMap()
{
	map.vertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(map.vertexArray, sfLineStrip);

	createMap();
}

void updateMap(Window* _window)
{
	sfVector2f mousePos = getMousePos();

	sfVector2i closestIndex = NULLVECTOR2I;
	float closestDistance = getSqrMagnitude(mousePos, map.tile[closestIndex.y][closestIndex.x].pos);

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			float tmpDistance = getSqrMagnitude(mousePos, map.tile[j][i].pos);
			if (tmpDistance < closestDistance) {
				closestDistance = tmpDistance;
				closestIndex = vector2i(i, j);
			}
		}
	}


	if (isIndexInMap(closestIndex) && isPointInHexagonTile(mousePos, map.tile[closestIndex.y][closestIndex.x])) {
		map.mouseHoverIndex = closestIndex;
	}
	else
		map.mouseHoverIndex = vector2i(-1, -1);

	if (isIndexInMap(map.mouseHoverIndex) && getMouseButtonState(sfMouseLeft) == MOUSE_STATE_HAS_PRESSED) {
		debugRandomiseDivisionsTypes(&map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x]);
	}
}

void displayMap(Window* _window)
{
	sfVertex tmpVertex;

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			// just so i know a function is not forbidden here
			switch (map.tile[j][i].state)
			{
			case TILE_STATE_VOID:
				sfVertexArray_setPrimitiveType(map.vertexArray, sfLineStrip);

				tmpVertex.color = TILE_VOID_COLOR;
				for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
				{
					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(map.vertexArray, tmpVertex);
				}
				tmpVertex.position = map.tile[j][i].div[TILE_TOP_LEFT].cornerPos;
				sfVertexArray_append(map.vertexArray, tmpVertex);

				sfRenderTexture_drawVertexArray(_window->renderTexture, map.vertexArray, NULL);
				sfVertexArray_clear(map.vertexArray);
				break;
			case TILE_STATE_ATTACHABLE:
				sfVertexArray_setPrimitiveType(map.vertexArray, sfLineStrip);

				tmpVertex.color = color(200, 200, 200);
				for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
				{
					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(map.vertexArray, tmpVertex);
				}
				tmpVertex.position = map.tile[j][i].div[TILE_TOP_LEFT].cornerPos;
				sfVertexArray_append(map.vertexArray, tmpVertex);

				sfRenderTexture_drawVertexArray(_window->renderTexture, map.vertexArray, NULL);
				sfVertexArray_clear(map.vertexArray);
				break;
			case TILE_STATE_PLACED:
				sfVertexArray_setPrimitiveType(map.vertexArray, sfTriangleFan);

				for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exteriors divisions
				{
					tmpVertex.color = getDivisonColor(map.tile[j][i].div[iDiv].type);

					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(map.vertexArray, tmpVertex);

					tmpVertex.position = map.tile[j][i].pos;
					sfVertexArray_append(map.vertexArray, tmpVertex);

					tmpVertex.position = map.tile[j][i].div[(iDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iDiv + 1)].cornerPos;
					sfVertexArray_append(map.vertexArray, tmpVertex);

					// do i need this ?? : tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					// do i need this ?? : sfVertexArray_append(map.vertexArray, tmpVertex);

					sfRenderTexture_drawVertexArray(_window->renderTexture, map.vertexArray, NULL);
					sfVertexArray_clear(map.vertexArray);
				}

				tmpVertex.color = getDivisonColor(map.tile[j][i].div[TILE_MIDDLE].type);
				for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
				{
					tmpVertex.position = map.tile[j][i].div[iIntDiv].interiorCornerPos;
					sfVertexArray_append(map.vertexArray, tmpVertex);
				}

				sfRenderTexture_drawVertexArray(_window->renderTexture, map.vertexArray, NULL);
				sfVertexArray_clear(map.vertexArray);
				break;
			default:
				break;
			}
		}
	}

	if (isIndexInMap(map.mouseHoverIndex)) {
		sfVertexArray_setPrimitiveType(map.vertexArray, sfLineStrip);

		tmpVertex.color = TILE_HOVER_COLOR;
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x].div[iDiv].cornerPos;
			sfVertexArray_append(map.vertexArray, tmpVertex);
		}
		tmpVertex.position = map.tile[map.mouseHoverIndex.y][map.mouseHoverIndex.x].div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(map.vertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, map.vertexArray, NULL);
		sfVertexArray_clear(map.vertexArray);
	}
}

void createMap()
{
	map.startPos = MAP_START_POS;

	map.tile = (Tile**)calloc(MAP_Y_SIZE, sizeof(Tile));

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		map.tile[j] = (Tile*)calloc(MAP_X_SIZE, sizeof(Tile));
	}

	resetMap();
}

void resetMap()
{
	map.mouseHoverIndex = vector2i(-1, -1);

	float tileSqrApothem = getTileSqrApothem();
	float halfTileSqrApothem = tileSqrApothem / 2.f;

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			//sfVector2f tilePos = addVectorsf(map.startPos, vector2f((float)i * (TILE_SIZE + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothemD2 + TILE_OFFSET_POS)));
			//
			//if (j % 2 == 0)
			//	tilePos.x += TILE_SIZE * 0.75f + TILE_OFFSET_POS;
			//
			//createTile(&map.tile[j][i], tilePos);

			sfVector2f tilePos = addVectorsf(map.startPos, vector2f((float)i * (TILE_RADIUS + TILE_OFFSET_POS) * 1.5f, (float)j * (tileSqrApothem + TILE_OFFSET_POS)));
			
			if (i % 2 == 0)
				tilePos.y += halfTileSqrApothem;
			
			createTile(&map.tile[j][i], tilePos);
		}
	}
}

sfBool isIndexInMap(sfVector2i _index)
{
	return ((_index.x >= 0 && _index.x < MAP_X_SIZE && _index.y >= 0 && _index.y < MAP_Y_SIZE) ? sfTrue : sfFalse);
}
