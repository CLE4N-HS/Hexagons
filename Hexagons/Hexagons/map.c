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
				for (int iIntDiv = TILE_TOP_LEFT_INTERIOR; iIntDiv <= TILE_BOTTOM_LEFT_INTERIOR; iIntDiv++) // interior division
				{
					tmpVertex.position = map.tile[j][i].div[iIntDiv].cornerPos;
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
	map.tile = (Tile**)calloc(MAP_Y_SIZE, sizeof(Tile));

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		map.tile[j] = (Tile*)calloc(MAP_X_SIZE, sizeof(Tile));
	}

	resetMap();
}

void resetMap()
{
	float tileApothemSquared = getTileApothem();
	float tileApothemD2 = tileApothemSquared / 2.f;

	for (int j = 0; j < MAP_Y_SIZE; j++)
	{
		for (int i = 0; i < MAP_X_SIZE; i++)
		{
			sfVector2f tilePos = vector2f((float)i * (TILE_SIZE + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothemD2 + TILE_OFFSET_POS));

			if (j % 2 == 0)
				tilePos.x += TILE_SIZE * 0.75f + TILE_OFFSET_POS;

			createTile(&map.tile[j][i], tilePos);

			//sfVector2f tilePos = vector2f((float)i * (TILE_SIZE / 2.f + TILE_OFFSET_POS) * 1.5f, (float)j * (tileApothemSquared + TILE_OFFSET_POS));
			//
			//if (i % 2 == 0)
			//	tilePos.y += tileApothemD2;
			//
			//createTile(&map.tile[j][i], tilePos);
		}
	}
}
