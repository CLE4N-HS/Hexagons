#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"

typedef struct {
	sfVector2f startPos;
	Tile** tile;
}Map;
Map map;

sfVertexArray* mapVertexArray;

void initMap()
{
	mapVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(mapVertexArray, sfLineStrip);

	createMap();
}

void updateMap(Window* _window)
{
	sfVector2f mousePos = getMousePos();

	sfVector2i estimatedIndex = NULLVECTOR2I;

	estimatedIndex = V2fToV2i(vector2f((mousePos.x + map.startPos.x) / ((TILE_RADIUS + TILE_OFFSET_POS) * 1.5f), (mousePos.y + map.startPos.x) / (getTileSqrApothem() + TILE_OFFSET_POS)));

	printf("%d, %d\n", estimatedIndex.x, estimatedIndex.y);

	if (isIndexInMap(estimatedIndex)) {
		if (isPointInHexagonTile(mousePos, map.tile[estimatedIndex.y][estimatedIndex.x])) {
		
		}
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
				break;
			case TILE_STATE_ATTACHABLE:
				sfVertexArray_setPrimitiveType(mapVertexArray, sfLineStrip);

				tmpVertex.color = color(200, 200, 200);
				for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
				{
					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(mapVertexArray, tmpVertex);
				}
				tmpVertex.position = map.tile[j][i].div[TILE_TOP_LEFT].cornerPos;
				sfVertexArray_append(mapVertexArray, tmpVertex);

				sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
				sfVertexArray_clear(mapVertexArray);
				break;
			case TILE_STATE_PLACED:
				sfVertexArray_setPrimitiveType(mapVertexArray, sfTriangleFan);

				for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exteriors divisions
				{
					tmpVertex.color = getDivisonColor(map.tile[j][i].div[iDiv].type);

					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(mapVertexArray, tmpVertex);

					tmpVertex.position = map.tile[j][i].pos;
					sfVertexArray_append(mapVertexArray, tmpVertex);

					tmpVertex.position = map.tile[j][i].div[(iDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iDiv + 1)].cornerPos;
					sfVertexArray_append(mapVertexArray, tmpVertex);

					tmpVertex.position = map.tile[j][i].div[iDiv].cornerPos;
					sfVertexArray_append(mapVertexArray, tmpVertex);

					sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
					sfVertexArray_clear(mapVertexArray);
				}

				tmpVertex.color = getDivisonColor(map.tile[j][i].div[TILE_MIDDLE].type);
				for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
				{
					tmpVertex.position = map.tile[j][i].div[iIntDiv].interiorCornerPos;
					sfVertexArray_append(mapVertexArray, tmpVertex);
				}

				sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
				sfVertexArray_clear(mapVertexArray);
				break;
			default:
				break;
			}
		}
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
