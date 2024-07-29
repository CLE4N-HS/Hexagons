#include "tile.h"
#include "customMath.h"
#include "mouseManager.h"
#include "map.h"

sfColor divisionColor[TILE_NB_MAX_TYPES];

float tileSqrApothem; // a define would be clearer but slightly less optimised

void initTile()
{
	divisionColor[TILE_TYPE_EMPTY]   = color(255, 255, 255);
	divisionColor[TILE_TYPE_LAND]	 = color( 50, 200,  50);
	divisionColor[TILE_TYPE_CITY]	 = color(200, 100,  50);
	divisionColor[TILE_TYPE_FARM]	 = color(255, 200,  50);
	divisionColor[TILE_TYPE_FOREST]	 = color( 25, 100,  25);
	divisionColor[TILE_TYPE_WATER]	 = color(150, 200, 150);
	divisionColor[TILE_TYPE_RAILWAY] = color(150, 100,  50);

	tileSqrApothem = TILE_START_RADIUS / tanf(PI / 6.f);
}

void updateTile(Window* _window, Tile** _tile)
{
	
}

void displayTile(Window* _window, Tile** _tile)
{
	sfVector2i mapSize = getMapSize();

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			drawTile(_window, &_tile[j][i]);
		}
	}
}

void drawTile(Window* _window, Tile* _tile)
{
	sfVertex tmpVertex;

	switch (_tile->state)
	{
	case TILE_STATE_VOID:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

		tmpVertex.color = TILE_VOID_COLOR;
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = _tile->div[iDiv].cornerPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);
		}
		tmpVertex.position = _tile->div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);
		break;
	case TILE_STATE_ATTACHABLE:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

		tmpVertex.color = color(200, 200, 200);
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = _tile->div[iDiv].cornerPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);
		}
		tmpVertex.position = _tile->div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);
		break;
	case TILE_STATE_PLACED:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfTriangleFan);

		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exteriors divisions
		{
			tmpVertex.color = getDivisonColor(_tile->div[iDiv].type);

			tmpVertex.position = _tile->div[iDiv].cornerPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position = _tile->pos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position = _tile->div[(iDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iDiv + 1)].cornerPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
			sfVertexArray_clear(_window->vertexArray);
		}

		tmpVertex.color = getDivisonColor(_tile->middleDivType);
		for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
		{
			tmpVertex.position = _tile->div[iIntDiv].interiorCornerPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);
		}

		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);
		break;
	default:
		break;
	}

	if (_tile->state == TILE_STATE_PLACED)
		drawTileEnvironement(_window, _tile);
}

void drawTileHover(Window* _window, Tile* _tile)
{
	sfVertex tmpVertex;

	sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

	tmpVertex.color = TILE_HOVER_COLOR;
	for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
	{
		tmpVertex.position = _tile->div[iDiv].cornerPos;
		sfVertexArray_append(_window->vertexArray, tmpVertex);
	}
	tmpVertex.position = _tile->div[TILE_TOP_LEFT].cornerPos;
	sfVertexArray_append(_window->vertexArray, tmpVertex);

	sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
	sfVertexArray_clear(_window->vertexArray);
}

void drawTileEnvironement(Window* _window, Tile* _tile)
{
	sfVertex tmpVertex;

	float tileScale = getTileScale(_tile);

	for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
	{
		sfVector2f tmpStartPos = addVectorsf(_tile->div[iIntDiv].interiorCornerPos, MultiplyVector2f(newVector2f(_tile->div[iIntDiv].interiorCornerPos, _tile->div[iIntDiv].cornerPos), 0.5f));
		tmpStartPos = addVectorsf(tmpStartPos, MultiplyVector2f(newVector2f(tmpStartPos, addVectorsf(_tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].interiorCornerPos, MultiplyVector2f(newVector2f(_tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].interiorCornerPos, _tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].cornerPos), 0.5f))), 0.5f));

		// always more confusing :
		tmpStartPos = addVectorsf(addVectorsf(_tile->div[iIntDiv].interiorCornerPos, MultiplyVector2f(newVector2f(_tile->div[iIntDiv].interiorCornerPos, _tile->div[iIntDiv].cornerPos), 0.5f)), MultiplyVector2f(newVector2f(addVectorsf(_tile->div[iIntDiv].interiorCornerPos, MultiplyVector2f(newVector2f(_tile->div[iIntDiv].interiorCornerPos, _tile->div[iIntDiv].cornerPos), 0.5f)), addVectorsf(_tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].interiorCornerPos, MultiplyVector2f(newVector2f(_tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].interiorCornerPos, _tile->div[(iIntDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iIntDiv + 1)].cornerPos), 0.5f))), 0.5f));
		//

		switch (_tile->div[iIntDiv].type)
		{
		case TILE_TYPE_LAND:
			break;

		case TILE_TYPE_CITY:
			sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

			tmpVertex.color = TILE_ENV_LINES_COLOR;

			tmpVertex.position = addVectorsf(tmpStartPos, vector2f(-10.f * tileScale, 0.f));
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position = PolarCoords(tmpVertex.position, 20.f * tileScale, -PI / 3.f);
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position = PolarCoords(tmpVertex.position, 20.f * tileScale, PI / 3.f);
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position.x -= 20.f * tileScale;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position.y += 15.f * tileScale;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position.x += 20.f * tileScale;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpVertex.position.y -= 15.f * tileScale;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
			sfVertexArray_clear(_window->vertexArray);
			break;

		case TILE_TYPE_FARM:
			sfVertexArray_setPrimitiveType(_window->vertexArray, sfTriangleFan);

			tmpVertex.color = TILE_ENV_FARM_BORDER_COLOR;

			int nextNeighbourDiv = getNextDivIndex(iIntDiv);
			int previousNeighbourDiv = getPreviousDivIndex(iIntDiv);

			if (_tile->middleDivType != TILE_TYPE_FARM) {
				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[iIntDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[nextNeighbourDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[nextNeighbourDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].interiorCornerPos, _tile->div[nextNeighbourDiv].cornerPos)));

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[iIntDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].interiorCornerPos, _tile->div[iIntDiv].cornerPos)));

				drawAndClearVertexArray(_window);
			}

			if (_tile->div[nextNeighbourDiv].type != TILE_TYPE_FARM) {
				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[nextNeighbourDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[nextNeighbourDiv].cornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[nextNeighbourDiv].cornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].cornerPos, _tile->div[iIntDiv].cornerPos)));

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[nextNeighbourDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].interiorCornerPos, _tile->div[iIntDiv].interiorCornerPos)));

				drawAndClearVertexArray(_window);
			}

			if (_tile->div[previousNeighbourDiv].type != TILE_TYPE_FARM) {
				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[iIntDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[iIntDiv].cornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[iIntDiv].cornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].cornerPos, _tile->div[nextNeighbourDiv].cornerPos)));

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[iIntDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].interiorCornerPos, _tile->div[nextNeighbourDiv].interiorCornerPos)));

				drawAndClearVertexArray(_window);
			}

			appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[iIntDiv].cornerPos);

			appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[nextNeighbourDiv].cornerPos);

			appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[nextNeighbourDiv].cornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].cornerPos, _tile->div[nextNeighbourDiv].interiorCornerPos)));

			appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[iIntDiv].cornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].cornerPos, _tile->div[iIntDiv].interiorCornerPos)));

			drawAndClearVertexArray(_window);
			break;

		case TILE_TYPE_FOREST:
			sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

			tmpVertex.color = TILE_ENV_LINES_COLOR;

			tmpStartPos.y += _tile->radius / 4.f;
			tmpVertex.position = tmpStartPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			tmpStartPos.y -= 12.f * tileScale;
			tmpVertex.position = tmpStartPos;
			sfVertexArray_append(_window->vertexArray, tmpVertex);

			sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
			sfVertexArray_clear(_window->vertexArray);

			tmpStartPos.y -= 8.f * tileScale;

			for (float angle = 0.f; angle < PI * 2.f; angle += PI / 6.f)
			{
				tmpVertex.position = PolarCoords(tmpStartPos, 8.f * tileScale, angle);
				sfVertexArray_append(_window->vertexArray, tmpVertex);
			}
			sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
			sfVertexArray_clear(_window->vertexArray);
			break;

		case TILE_TYPE_WATER:
			break;

		case TILE_TYPE_RAILWAY:
			break;

		default:
			break;
		}


	}

	// TODO middle div

	sfVector2f tmpStartPos = _tile->pos;

	switch (_tile->middleDivType)
	{
	case TILE_TYPE_LAND:
		break;

	case TILE_TYPE_CITY:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

		tmpVertex.color = TILE_ENV_LINES_COLOR;

		tmpVertex.position = addVectorsf(tmpStartPos, vector2f(-10.f * tileScale, 0.f));
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position = PolarCoords(tmpVertex.position, 20.f * tileScale, -PI / 3.f);
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position = PolarCoords(tmpVertex.position, 20.f * tileScale, PI / 3.f);
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position.x -= 20.f * tileScale;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position.y += 15.f * tileScale;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position.x += 20.f * tileScale;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpVertex.position.y -= 15.f * tileScale;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);
		break;

	case TILE_TYPE_FARM:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfTriangleFan);

		tmpVertex.color = TILE_ENV_FARM_BORDER_COLOR;

		for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
		{
			if (_tile->div[iIntDiv].type != TILE_TYPE_FARM) {
				int nextNeighbourDiv = getNextDivIndex(iIntDiv);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[iIntDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, _tile->div[nextNeighbourDiv].interiorCornerPos);

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[nextNeighbourDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].interiorCornerPos, PolarCoords(_tile->div[nextNeighbourDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE* tileScale, getVectorsAngle(_tile->div[nextNeighbourDiv].interiorCornerPos, _tile->div[getNextDivIndex(nextNeighbourDiv)].interiorCornerPos)))));

				appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, PolarCoords(_tile->div[iIntDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].interiorCornerPos, PolarCoords(_tile->div[iIntDiv].interiorCornerPos, TILE_ENV_FARM_BORDER_SIZE * tileScale, getVectorsAngle(_tile->div[iIntDiv].interiorCornerPos, _tile->div[getPreviousDivIndex(iIntDiv)].interiorCornerPos)))));

				drawAndClearVertexArray(_window);
			}
		}
		break;

	case TILE_TYPE_FOREST:
		sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

		tmpVertex.color = TILE_ENV_LINES_COLOR;

		tmpStartPos.y += _tile->radius / 4.f;
		tmpVertex.position = tmpStartPos;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		tmpStartPos.y -= 12.f * tileScale;
		tmpVertex.position = tmpStartPos;
		sfVertexArray_append(_window->vertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);

		tmpStartPos.y -= 8.f * tileScale;

		for (float angle = 0.f; angle < PI * 2.f; angle += PI / 6.f)
		{
			tmpVertex.position = PolarCoords(tmpStartPos, 8.f * tileScale, angle);
			sfVertexArray_append(_window->vertexArray, tmpVertex);
		}
		sfRenderTexture_drawVertexArray(_window->renderTexture, _window->vertexArray, NULL);
		sfVertexArray_clear(_window->vertexArray);
		break;

	case TILE_TYPE_WATER:
		break;

	case TILE_TYPE_RAILWAY:
		break;

	default:
		break;
	}


}

void createTile(Tile* _tile, sfVector2f _pos, TileState _state)
{
	_tile->pos = _pos;
	_tile->state = _state;
	_tile->radius = TILE_START_RADIUS;

	resetDivisionCornerPos(_tile);

	_tile->middleDivType = TILE_TYPE_EMPTY;
	for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
	{
		_tile->div[i].type = TILE_TYPE_EMPTY;
	}
}

void resetDivisionCornerPos(Tile* _tile)
{
	_tile->div[TILE_TOP_LEFT].cornerPos             = addVectorsf(_tile->pos, vector2f(-_tile->radius, 0.f));
	_tile->div[TILE_TOP].cornerPos                  = PolarCoords(_tile->pos, _tile->radius, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT].cornerPos            = PolarCoords(_tile->pos, _tile->radius, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT].cornerPos         = addVectorsf(_tile->pos, vector2f(_tile->radius, 0.f));
	_tile->div[TILE_BOTTOM].cornerPos               = PolarCoords(_tile->pos, _tile->radius, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT].cornerPos          = PolarCoords(_tile->pos, _tile->radius, 2.f * PI / 3.f);
																			
	_tile->div[TILE_TOP_LEFT].interiorCornerPos     = addVectorsf(_tile->pos, vector2f(-_tile->radius / 3.f, 0.f));
	_tile->div[TILE_TOP].interiorCornerPos          = PolarCoords(_tile->pos, _tile->radius / 3.f, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT].interiorCornerPos    = PolarCoords(_tile->pos, _tile->radius / 3.f, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT].interiorCornerPos = addVectorsf(_tile->pos, vector2f(_tile->radius / 3.f, 0.f));
	_tile->div[TILE_BOTTOM].interiorCornerPos       = PolarCoords(_tile->pos, _tile->radius / 3.f, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT].interiorCornerPos  = PolarCoords(_tile->pos, _tile->radius / 3.f, 2.f * PI / 3.f);
}

sfColor getDivisonColor(TileType _type)
{
	return divisionColor[_type];
}

float getTileSqrApothem()
{
	return tileSqrApothem;
}

sfBool isPointInHexagonTile(sfVector2f _point, Tile _tile)
{
	float sqrRadius = _tile.radius * _tile.radius;
	float sqrMagnitude = getSqrMagnitude(_point, _tile.pos);

	if (sqrMagnitude <= sqrRadius * 0.75f) // inner radius
		return sfTrue;

	if (sqrMagnitude > sqrRadius) // outer radius
		return sfFalse;


	for (int i = TILE_TOP_LEFT; i <= TILE_BOTTOM_LEFT; i++) // check all triangles
	{
		if (isPointInTriangle(_point, _tile.pos, _tile.div[i].cornerPos, (i == TILE_BOTTOM_LEFT ? _tile.div[TILE_TOP_LEFT].cornerPos : _tile.div[i + 1].cornerPos)))
			return sfTrue;
	}

	return sfFalse;
}

void setTileState(Tile* _tile, TileState _state)
{
	_tile->state = _state;
}

void setTilePos(Tile* _tile, sfVector2f _pos)
{
	_tile->pos = _pos;
	resetDivisionCornerPos(_tile);
}

void setTileRadius(Tile* _tile, float _radius)
{
	_tile->radius = _radius;
	resetDivisionCornerPos(_tile);
}

void setTilePosAndRadius(Tile* _tile, sfVector2f _pos, float _radius)
{
	_tile->pos = _pos;
	_tile->radius = _radius;
	resetDivisionCornerPos(_tile);
}

void rotateTile(Tile* _tile, sfBool _clockwise)
{
	TileType tmpDivisionType = (_clockwise ? _tile->div[TILE_NB_MAX_DIVISIONS - 1].type : _tile->div[TILE_TOP_LEFT].type);

	if (_clockwise) {
		for (int i = TILE_NB_MAX_DIVISIONS - 1; i >= 0; i--)
		{
			_tile->div[i].type = (i == 0 ? tmpDivisionType : _tile->div[i - 1].type);
		}
	}
	else {
		for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
		{
			_tile->div[i].type = (i == TILE_NB_MAX_DIVISIONS - 1 ? tmpDivisionType : _tile->div[i + 1].type);
		}
	}
}

float getTileScale(Tile* _tile)
{
	return _tile->radius / (TILE_START_RADIUS);
}

int getNextDivIndex(int _currentIndex)
{
	return (_currentIndex >= TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : _currentIndex + 1);
}

int getPreviousDivIndex(int _currentIndex)
{
	return (_currentIndex <= TILE_TOP_LEFT ? TILE_BOTTOM_LEFT : _currentIndex - 1);
}

void debugRandomiseDivisionsTypes(Tile* _tile) // TODO to remove
{
	_tile->middleDivType = 1 + rand() % (TILE_NB_MAX_TYPES - 1);
	for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
	{
		_tile->div[i].type = 1 + rand() % (TILE_NB_MAX_TYPES - 1);
	}
}
