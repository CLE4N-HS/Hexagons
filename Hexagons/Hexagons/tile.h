#pragma once
#include "windowManager.h"

#define TILE_SIZE 128.f
#define TILE_OFFSET_POS 10.f

typedef enum {
	TILE_MIDDLE = 0,

	TILE_TOP_LEFT,
	TILE_TOP,
	TILE_TOP_RIGHT,
	TILE_BOTTOM_RIGHT,
	TILE_BOTTOM,
	TILE_BOTTOM_LEFT,

	TILE_TOP_LEFT_INTERIOR,
	TILE_TOP_INTERIOR,
	TILE_TOP_RIGHT_INTERIOR,
	TILE_BOTTOM_RIGHT_INTERIOR,
	TILE_BOTTOM_INTERIOR,
	TILE_BOTTOM_LEFT_INTERIOR,

	TILE_NB_MAX_DIVISIONS
}TileDirection;

typedef enum {
	TILE_STATE_VOID = 0,
	TILE_STATE_ATTACHABLE,
	TILE_STATE_PLACED
}TileState;

typedef enum {
	TILE_TYPE_EMPTY = 0,
	TILE_TYPE_LAND,
	TILE_TYPE_CITY,
	TILE_TYPE_FARM,
	TILE_TYPE_FOREST,
	TILE_TYPE_WATER,
	TILE_TYPE_RAILWAY,

	TILE_NB_MAX_TYPES
}TileType;

typedef struct {
	sfVector2f cornerPos;
	sfVector2f interiorCornerPos;
	TileType type;
}Division;

typedef struct {
	sfVector2f pos;
	TileState state;
	Division div[TILE_NB_MAX_DIVISIONS];
}Tile;

void initTile();
void createTile(Tile* _tile, sfVector2f _pos);
sfVector2f setDivisionCornerPos(Tile* _tile);
sfColor getDivisonColor(TileType _type);
float getTileApothem();
