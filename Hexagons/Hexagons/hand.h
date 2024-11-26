#pragma once
#include "windowManager.h"
#include "tile.h"

#define HAND_NB_MAX_TILES 3

#define HAND_START_NB_TILES HAND_NB_MAX_TILES
#define HAND_START_POS vector2f(WINDOW_LENGTH / 2.f - TILE_START_SIZE, WINDOW_HEIGHT - TILE_START_RADIUS)

#define HAND_TILE_HOVER_SCALE 2.f

#define HAND_MIN_Y_POS (WINDOW_HEIGHT - TILE_START_SIZE * 2.f)

typedef enum {
	HAND_STATE_LOOKING,
	HAND_STATE_MOVING,
	HAND_STATE_PLACING
}HandState;

typedef struct {
	HandState state;
	Tile tile[HAND_NB_MAX_TILES];
	int nbTiles;
	int mouseHoverIndex;
}Hand;

void updateHand(Window* _window, Hand* _hand);
void displayHand(Window* _window, Hand* _hand);
void createHand(Hand* _hand);
