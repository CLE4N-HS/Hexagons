#include "tile.h"
#include "customMath.h"

sfColor divisionColor[TILE_NB_MAX_TYPES];

float tileApothem;

void initTile()
{
	divisionColor[TILE_TYPE_EMPTY]   = color(255, 255, 255);
	divisionColor[TILE_TYPE_LAND]	 = color(255, 255, 255);
	divisionColor[TILE_TYPE_CITY]	 = color(255, 255, 255);
	divisionColor[TILE_TYPE_FARM]	 = color(255, 255, 255);
	divisionColor[TILE_TYPE_FOREST]	 = color(255, 255, 255);
	divisionColor[TILE_TYPE_WATER]	 = color(255, 255, 255);
	divisionColor[TILE_TYPE_RAILWAY] = color(255, 255, 255);

	tileApothem = sqrtf(3.f) * TILE_SIZE / 2.f;
}

void createTile(Tile* _tile, sfVector2f _pos)
{
	_tile->pos = _pos;
	_tile->state = TILE_STATE_VOID;

	setDivisionCornerPos(_tile);

	for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
	{
		_tile->div[i].type = TILE_TYPE_EMPTY;
	}
}

sfVector2f setDivisionCornerPos(Tile* _tile)
{
	_tile->div[TILE_MIDDLE].cornerPos       = _tile->pos;
	_tile->div[TILE_TOP_LEFT].cornerPos     = addVectorsf(_tile->pos, vector2f(-TILE_SIZE / 2.f, 0.f));
	_tile->div[TILE_TOP].cornerPos          = PolarCoords(_tile->pos, TILE_SIZE / 2.f, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT].cornerPos    = PolarCoords(_tile->pos, TILE_SIZE / 2.f, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT].cornerPos = addVectorsf(_tile->pos, vector2f(TILE_SIZE / 2.f, 0.f));
	_tile->div[TILE_BOTTOM].cornerPos       = PolarCoords(_tile->pos, TILE_SIZE / 2.f, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT].cornerPos  = PolarCoords(_tile->pos, TILE_SIZE / 2.f, 2.f * PI / 3.f);

	_tile->div[TILE_TOP_LEFT_INTERIOR].cornerPos     = addVectorsf(_tile->pos, vector2f(-TILE_SIZE / 4.f, 0.f));
	_tile->div[TILE_TOP_INTERIOR].cornerPos          = PolarCoords(_tile->pos, TILE_SIZE / 4.f, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT_INTERIOR].cornerPos    = PolarCoords(_tile->pos, TILE_SIZE / 4.f, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT_INTERIOR].cornerPos = addVectorsf(_tile->pos, vector2f(TILE_SIZE / 4.f, 0.f));
	_tile->div[TILE_BOTTOM_INTERIOR].cornerPos       = PolarCoords(_tile->pos, TILE_SIZE / 4.f, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT_INTERIOR].cornerPos  = PolarCoords(_tile->pos, TILE_SIZE / 4.f, 2.f * PI / 3.f);
}

sfColor getDivisonColor(TileType _type)
{
	return divisionColor[_type];
}

float getTileApothem()
{
	return tileApothem;
}
