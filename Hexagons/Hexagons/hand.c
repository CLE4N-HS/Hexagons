#include "hand.h"
#include "mouseManager.h"
#include "customMath.h"

void updateHand(Window* _window, Hand* _hand)
{
	sfVector2f mousePos = getMousePos();

	int closestIndex = 0;
	float closestDistance = getSqrMagnitude(mousePos, _hand->tile[closestIndex].pos);

	for (int i = 0; i < _hand->nbTiles; i++)
	{
		float tmpDistance = getSqrMagnitude(mousePos, _hand->tile[i].pos);
		if (tmpDistance < closestDistance) {
			closestDistance = tmpDistance;
			closestIndex = i;
		}
	}

	MouseButtonState mouseLeftState = getMouseButtonState(sfMouseLeft);

	switch (_hand->state)
	{
	case HAND_STATE_LOOKING:
		if (isPointInHexagonTile(mousePos, _hand->tile[closestIndex])) { // technically not to do every frame
			_hand->mouseHoverIndex = closestIndex;
			if (mouseLeftState >= MOUSE_STATE_PRESSED) { // selecting the tile
				_hand->state = HAND_STATE_MOVING;
			}
			else { // just looking / hovering the tile
				_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS * HAND_TILE_HOVER_SCALE;
				_hand->tile[_hand->mouseHoverIndex].pos.y = HAND_START_POS.y - TILE_START_RADIUS * HAND_TILE_HOVER_SCALE / 2.f + 10.f; // TODO +10.f is a fraud
				resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);
			}
		}
		else {
			if (_hand->mouseHoverIndex >= 0) {
				_hand->tile[_hand->mouseHoverIndex].pos.y = HAND_START_POS.y;
				_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS;
				resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);
			}

			_hand->mouseHoverIndex = -1;
		}
		break;
	case HAND_STATE_MOVING:
		_hand->tile[_hand->mouseHoverIndex].pos = mousePos;
		_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS * HAND_TILE_HOVER_SCALE;
		resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);

		if (mouseLeftState == MOUSE_STATE_RELEASED) {
			_hand->state = HAND_STATE_LOOKING;
			_hand->tile[_hand->mouseHoverIndex].pos.y = HAND_START_POS.y;
			_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS;
			resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);
		}
		else if (mousePos.y < HAND_MIN_Y_POS) {
			_hand->state = HAND_STATE_PLACING;
		}
		break;
	case HAND_STATE_PLACING:
		_hand->tile[_hand->mouseHoverIndex].pos = mousePos; // TODO a func for these lines
		_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS;
		resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);

		if (mouseLeftState == MOUSE_STATE_RELEASED) {
			_hand->state = HAND_STATE_LOOKING;
			_hand->tile[_hand->mouseHoverIndex].pos.y = HAND_START_POS.y;
			_hand->tile[_hand->mouseHoverIndex].radius = TILE_START_RADIUS;
			resetDivisionCornerPos(&_hand->tile[_hand->mouseHoverIndex]);
		}
		else if (mousePos.y >= HAND_MIN_Y_POS) {
			_hand->state = HAND_STATE_MOVING;
		}
		break;
	default:
		break;
	}
}

void displayHand(Window* _window, Hand* _hand)
{
	for (int i = 0; i < _hand->nbTiles; i++)
	{
		drawTile(_window, &_hand->tile[i]);
	}

	if (_hand->mouseHoverIndex >= 0) {
		drawTileHover(_window, &_hand->tile[_hand->mouseHoverIndex]);
	}
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
