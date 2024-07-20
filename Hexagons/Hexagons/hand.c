#include "hand.h"
#include "mouseManager.h"
#include "customMath.h"

void updateHand(Window* _window, Hand* _hand)
{
	
}

void displayHand(Window* _window, Hand* _hand)
{

}

void createHand(Hand* _hand)
{
	_hand->state = HAND_STATE_LOOKING;
	_hand->nbTiles = HAND_START_NB_TILES;
	_hand->mouseHoverIndex = -1;

	sfVector2f tilePos = HAND_START_POS;

	for (int i = 0; i < _hand->nbTiles; i++)
	{
		createTile(&_hand->tile[i], tilePos, TILE_STATE_PLACED);
		debugRandomiseDivisionsTypes(&_hand->tile[i]);
		tilePos.x += TILE_START_SIZE * 1.5f;
	}
}
