#include "tile.h"
#include "customMath.h"
#include "mouseManager.h"
#include "map.h"

sfVertexArray* tileVertexArray;

sfColor divisionColor[TILE_NB_MAX_TYPES];

float tileSqrApothem; // a define would be clearer but slightly less optimised

void initTile()
{
	tileVertexArray = sfVertexArray_create();

	divisionColor[TILE_TYPE_EMPTY]   = color(255, 255, 255);
	divisionColor[TILE_TYPE_LAND]	 = color(100, 100,  50);
	divisionColor[TILE_TYPE_CITY]	 = color(200, 100,  50);
	divisionColor[TILE_TYPE_FARM]	 = color(255, 200,  50);
	divisionColor[TILE_TYPE_FOREST]	 = color( 50,  50,  50);
	divisionColor[TILE_TYPE_WATER]	 = color(150, 200, 150);
	divisionColor[TILE_TYPE_RAILWAY] = color(150, 100,  50);

	tileSqrApothem = TILE_RADIUS / tanf(PI / 6.f);
}

void updateTile(Window* _window, Tile** _tile)
{
	sfVector2f mousePos = getMousePos();

	sfVector2i closestIndex = NULLVECTOR2I;
	float closestDistance = getSqrMagnitude(mousePos, _tile[closestIndex.y][closestIndex.x].pos);

	sfVector2i mapSize = getMapSize();

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			float tmpDistance = getSqrMagnitude(mousePos, _tile[j][i].pos);
			if (tmpDistance < closestDistance) {
				closestDistance = tmpDistance;
				closestIndex = vector2i(i, j);
			}
		}
	}

	sfVector2i mapMouseHoverIndex = getMapMouseHoverIndex();

	if (isIndexInMap(closestIndex) && isPointInHexagonTile(mousePos, _tile[closestIndex.y][closestIndex.x])) {
		setMapMouseHoverIndex(closestIndex);
	}
	else
		setMapMouseHoverIndex(vector2i(-1, -1));

	mapMouseHoverIndex = getMapMouseHoverIndex();

	if (isIndexInMap(mapMouseHoverIndex) && getMouseButtonState(sfMouseLeft) == MOUSE_STATE_HAS_PRESSED) {
		setTileState(&_tile[mapMouseHoverIndex.y][mapMouseHoverIndex.x], TILE_STATE_PLACED);
		debugRandomiseDivisionsTypes(&_tile[mapMouseHoverIndex.y][mapMouseHoverIndex.x]);
	}
}

void displayTile(Window* _window, Tile** _tile)
{
	sfVertex tmpVertex;
	sfVector2i mapSize = getMapSize();

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			drawTile(_window, _tile[j][i]);
		}
	}

	sfVector2i mapMouseHoverIndex = getMapMouseHoverIndex();

	if (isIndexInMap(mapMouseHoverIndex)) {
		sfVertexArray_setPrimitiveType(tileVertexArray, sfLineStrip);

		tmpVertex.color = TILE_HOVER_COLOR;
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = _tile[mapMouseHoverIndex.y][mapMouseHoverIndex.x].div[iDiv].cornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);
		}
		tmpVertex.position = _tile[mapMouseHoverIndex.y][mapMouseHoverIndex.x].div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(tileVertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, tileVertexArray, NULL);
		sfVertexArray_clear(tileVertexArray);
	}
}

void drawTile(Window* _window, Tile _tile)
{
	sfVertex tmpVertex;

	switch (_tile.state)
	{
	case TILE_STATE_VOID:
		sfVertexArray_setPrimitiveType(tileVertexArray, sfLineStrip);

		tmpVertex.color = TILE_VOID_COLOR;
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = _tile.div[iDiv].cornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);
		}
		tmpVertex.position = _tile.div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(tileVertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, tileVertexArray, NULL);
		sfVertexArray_clear(tileVertexArray);
		break;
	case TILE_STATE_ATTACHABLE:
		sfVertexArray_setPrimitiveType(tileVertexArray, sfLineStrip);

		tmpVertex.color = color(200, 200, 200);
		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exterior contour
		{
			tmpVertex.position = _tile.div[iDiv].cornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);
		}
		tmpVertex.position = _tile.div[TILE_TOP_LEFT].cornerPos;
		sfVertexArray_append(tileVertexArray, tmpVertex);

		sfRenderTexture_drawVertexArray(_window->renderTexture, tileVertexArray, NULL);
		sfVertexArray_clear(tileVertexArray);
		break;
	case TILE_STATE_PLACED:
		sfVertexArray_setPrimitiveType(tileVertexArray, sfTriangleFan);

		for (int iDiv = TILE_TOP_LEFT; iDiv <= TILE_BOTTOM_LEFT; iDiv++) // exteriors divisions
		{
			tmpVertex.color = getDivisonColor(_tile.div[iDiv].type);

			tmpVertex.position = _tile.div[iDiv].cornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);

			tmpVertex.position = _tile.pos;
			sfVertexArray_append(tileVertexArray, tmpVertex);

			tmpVertex.position = _tile.div[(iDiv == TILE_BOTTOM_LEFT ? TILE_TOP_LEFT : iDiv + 1)].cornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);

			sfRenderTexture_drawVertexArray(_window->renderTexture, tileVertexArray, NULL);
			sfVertexArray_clear(tileVertexArray);
		}

		tmpVertex.color = getDivisonColor(_tile.middleDivType);
		for (int iIntDiv = TILE_TOP_LEFT; iIntDiv <= TILE_BOTTOM_LEFT; iIntDiv++) // interior division
		{
			tmpVertex.position = _tile.div[iIntDiv].interiorCornerPos;
			sfVertexArray_append(tileVertexArray, tmpVertex);
		}

		sfRenderTexture_drawVertexArray(_window->renderTexture, tileVertexArray, NULL);
		sfVertexArray_clear(tileVertexArray);
		break;
	default:
		break;
	}
}

void createTile(Tile* _tile, sfVector2f _pos, TileState _state)
{
	_tile->pos = _pos;
	_tile->state = _state;

	setDivisionCornerPos(_tile);

	_tile->middleDivType = TILE_TYPE_EMPTY;
	for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
	{
		_tile->div[i].type = TILE_TYPE_EMPTY;
	}
}

void setDivisionCornerPos(Tile* _tile)
{
	_tile->div[TILE_TOP_LEFT].cornerPos             = addVectorsf(_tile->pos, vector2f(-TILE_RADIUS, 0.f));
	_tile->div[TILE_TOP].cornerPos                  = PolarCoords(_tile->pos, TILE_RADIUS, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT].cornerPos            = PolarCoords(_tile->pos, TILE_RADIUS, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT].cornerPos         = addVectorsf(_tile->pos, vector2f(TILE_RADIUS, 0.f));
	_tile->div[TILE_BOTTOM].cornerPos               = PolarCoords(_tile->pos, TILE_RADIUS, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT].cornerPos          = PolarCoords(_tile->pos, TILE_RADIUS, 2.f * PI / 3.f);

	_tile->div[TILE_TOP_LEFT].interiorCornerPos     = addVectorsf(_tile->pos, vector2f(-TILE_SIZE / 6.f, 0.f));
	_tile->div[TILE_TOP].interiorCornerPos          = PolarCoords(_tile->pos, TILE_SIZE / 6.f, -2.f * PI / 3.f);
	_tile->div[TILE_TOP_RIGHT].interiorCornerPos    = PolarCoords(_tile->pos, TILE_SIZE / 6.f, -PI / 3.f);
	_tile->div[TILE_BOTTOM_RIGHT].interiorCornerPos = addVectorsf(_tile->pos, vector2f(TILE_SIZE / 6.f, 0.f));
	_tile->div[TILE_BOTTOM].interiorCornerPos       = PolarCoords(_tile->pos, TILE_SIZE / 6.f, PI / 3.f);
	_tile->div[TILE_BOTTOM_LEFT].interiorCornerPos  = PolarCoords(_tile->pos, TILE_SIZE / 6.f, 2.f * PI / 3.f);
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
	float sqrRadius = TILE_RADIUS * TILE_RADIUS;
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

void debugRandomiseDivisionsTypes(Tile* _tile) // TODO to remove
{
	_tile->middleDivType = rand() % TILE_NB_MAX_TYPES;
	for (int i = 0; i < TILE_NB_MAX_DIVISIONS; i++)
	{
		_tile->div[i].type = rand() % TILE_NB_MAX_TYPES;
	}
}
