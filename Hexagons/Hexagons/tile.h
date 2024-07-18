#pragma once
#include "windowManager.h"

#define TILE_SIZE 128.f
#define TILE_OFFSET_POS 2.f

#define TILE_RADIUS TILE_SIZE / 2.f

#define TILE_START_POS vector2f(TILE_RADIUS, getTileSqrApothem() / 2.f);

#define TILE_HOVER_COLOR color(255, 255, 255)
#define TILE_VOID_COLOR  color( 30,  30,  30)

typedef enum {
	TILE_MIDDLE = -1,

	TILE_TOP_LEFT = 0,
	TILE_TOP,
	TILE_TOP_RIGHT,
	TILE_BOTTOM_RIGHT,
	TILE_BOTTOM,
	TILE_BOTTOM_LEFT,

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
	TileType middleDivType;
}Tile;

void initTile();
void updateTile(Window* _window, Tile** _tile);
void displayTile(Window* _window, Tile** _tile);
void drawTile(Window* _window, Tile _tile);
void createTile(Tile* _tile, sfVector2f _pos, TileState _state);
void setDivisionCornerPos(Tile* _tile);
sfColor getDivisonColor(TileType _type);
float getTileSqrApothem();
sfBool isPointInHexagonTile(sfVector2f _point, Tile _tile);
void setTileState(Tile* _tile, TileState _state);

void debugRandomiseDivisionsTypes(Tile* _tile);
