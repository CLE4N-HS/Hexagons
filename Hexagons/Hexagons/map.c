#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"

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

}

void displayMap(Window* _window)
{
	sfVertexArray_setPrimitiveType(mapVertexArray, sfTriangleFan);

	sfVertex tmpVertex;

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			int currentDivison = TILE_TOP_LEFT;

			for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++)
			{
				tmpVertex.color = getDivisonColor(map.tile[j][i].div[currentDivison].type);

				tmpVertex.position = map.tile[j][i].div[currentDivison].cornerPos;
				sfVertexArray_append(mapVertexArray, tmpVertex);

				tmpVertex.position = map.tile[j][i].pos;
				//sfVertexArray_append(mapVertexArray, tmpVertex);

				tmpVertex.position = map.tile[j][i].div[++currentDivison].cornerPos;
				//sfVertexArray_append(mapVertexArray, tmpVertex);
			}

			tmpVertex.position = map.tile[j][i].div[TILE_TOP_LEFT].cornerPos;
			tmpVertex.color = getDivisonColor(map.tile[j][i].div[currentDivison - 1].type);
			//sfVertexArray_append(mapVertexArray, tmpVertex);

			sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);

			sfVertexArray_clear(mapVertexArray);
		}
	}
}

void createMap()
{
	map.tile = (Tile**)calloc(MAP_Y_SIZE, sizeof(Tile));

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		map.tile[j] = (Tile*)calloc(MAP_X_SIZE, sizeof(Tile));
	}

	resetMap();
}

void resetMap()
{
	float tileApothem = getTileApothem();
	float tileApothemD2 = tileApothem / 2.f;

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			sfVector2f tilePos = vector2f((float)i * (TILE_SIZE / 2.f + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothem + TILE_OFFSET_POS));

			if (i % 2 == 0)
				tilePos.y += tileApothemD2;

			createTile(&map.tile[j][i], tilePos);
		}
	}
}
