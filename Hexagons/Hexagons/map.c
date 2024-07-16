#include "map.h"
#include "mouseManager.h"
#include "customMath.h"
#include "keyboardManager.h"
#include "game.h"
#include "hud.h"

Block** map;
Block** savedMap;
int mapXSize;
int mapYSize;
sfVector2i startIndex;
sfVector2i finishIndex;
sfVector2i* openList;
sfVector2i* closedList;
int nbClosedListElements;
sfVector2i* searchList;
int nbNodes;
int nbOpenListElements;
sfVector2i currentNode = {0, 0};
sfBool showOpenList;
sfBool showClosedList;
sfBool showValues;
float blockSize;

void (*mapFunctions[HUD_NB_MAX_TYPES - HUD_SEARCH])() = { search, saveMap, resetMap, defaultMap, find, toggleOpenList, toggleClosedList, toggleValues, reduceMapSize, increaseMapSize, maze, changeFrames};

// maze
int subBlocks;
sfVector2i* mazeStack;
int nbMazeStackElements;
sfVector2i* mazeOpenList;
int nbMazeOpenListElements;
sfVector2i currentIndex;
//

float timeBetweenFrames;
char mapStringBuffer[30];

sfRectangleShape* mapRectangle;
sfText* mapText;
sfVertexArray* mapVertexArray;
sfVertex mapVertex;

sfColor blockColor[TILE_NB_MAX_TYPES];

void initMap()
{
	blockSize = DEFAULT_BLOCK_SIZE;
	subBlocks = 2;
	currentIndex = NULLVECTOR2I;
	timeBetweenFrames = 0.f;
	loadMap();

	mapRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(mapRectangle, vector2f(blockSize, blockSize));
	sfRectangleShape_setOrigin(mapRectangle, vector2f(getBlockOrigin(), getBlockOrigin()));
	sfRectangleShape_setOutlineThickness(mapRectangle, BLOCK_OUTLINE_THICKNESS);
	sfRectangleShape_setOutlineColor(mapRectangle, color(128, 128, 128));

	mapText = sfText_create();
	sfText_setFillColor(mapText, color(255, 255, 255));
	sfText_setCharacterSize(mapText, 10);
	sfText_setFont(mapText, sfFont_createFromFile(FONT_PATH"Square.ttf"));

	mapVertexArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(mapVertexArray, sfLines);
	mapVertex.color = color(0, 255, 100);

	blockColor[TILE_PATH] = color(255, 255, 255);
	blockColor[TILE_WALL] = color(0, 0, 0);
	blockColor[TILE_START] = color(0, 255, 0);
	blockColor[TILE_FINISH] = color(255, 0, 0);
	blockColor[TILE_OPEN] = color(0, 0, 255);
	blockColor[TILE_CLOSED] = color(200, 50, 50);
	blockColor[TILE_CURRENT] = color(255, 0, 255);

	showOpenList = sfTrue;
	showClosedList = sfTrue;
	showValues = sfFalse;
}

void updateMap(Window* _window)
{
	sfVector2f mousePos = getMousePos();
	sfBool hasLeftCLick = leftClickUnreleased();

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			if (hasLeftCLick && map[j][i].type != TILE_START && map[j][i].type != TILE_FINISH && sfFloatRect_contains(&map[j][i].bounds, mousePos.x, mousePos.y)) {
				TileType selectedType = getSelectedType();
				if (selectedType == TILE_START || selectedType == TILE_FINISH)
					removeType(selectedType);

				map[j][i].type = selectedType;

				if (map[j][i].type == TILE_START)
					startIndex = vector2i(i, j);
				else if (map[j][i].type == TILE_FINISH)
					finishIndex = vector2i(i, j);
			}

			map[j][i].hCost = getHCost(vector2i(i, j));
			map[j][i].fCost = map[j][i].gCost + map[j][i].hCost;
		}
	}

	if (!isPressing(sfKeyLShift)) {
		if (hasPressed(sfKeyZ) || isPressing(sfKeyUp))
			changeMapSize(mapXSize, mapYSize - 1);
		if (hasPressed(sfKeyQ) || isPressing(sfKeyLeft))
			changeMapSize(mapXSize - 1, mapYSize);
		if (hasPressed(sfKeyS) || isPressing(sfKeyDown))
			changeMapSize(mapXSize, mapYSize + 1);
		if (hasPressed(sfKeyD) || isPressing(sfKeyRight))
			changeMapSize(mapXSize + 1, mapYSize);
	}

	HudTypes tmpMapFunctionType = getMapFunctionType();
	if (tmpMapFunctionType < HUD_NB_MAX_TYPES && tmpMapFunctionType != HUD_FRAMES) {
		mapFunctions[tmpMapFunctionType]();
		setMapFunctionType(HUD_NB_MAX_TYPES);
	}
}

void displayMap(Window* _window)
{
	sfMutex_lock(gameMutex);

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			sfRectangleShape_setPosition(mapRectangle, map[j][i].pos);
			sfRectangleShape_setFillColor(mapRectangle, blockColor[map[j][i].type]);

			sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
		}
	}

	sfMutex_unlock(gameMutex);

	sfRectangleShape_setPosition(mapRectangle, map[startIndex.y][startIndex.x].pos);
	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_START]);

	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);

	sfRectangleShape_setPosition(mapRectangle, map[finishIndex.y][finishIndex.x].pos);
	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_FINISH]);

	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);


	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_OPEN]);

	for (int i = 0; i < nbOpenListElements; i++)
	{
		if (showOpenList) 
			displayList(_window, openList, i);

		if (showValues)
			displayValues(_window, openList, i);
	}

	sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_CLOSED]);

	for (int i = 0; i < nbClosedListElements; i++)
	{
		if (showClosedList)
			displayList(_window, closedList, i);

		if (showValues)
			displayValues(_window, closedList, i);
	}

	sfMutex_lock(gameMutex);
	HudTypes tmpMapFunctionType = getMapFunctionType() + HUD_SEARCH;
	switch (tmpMapFunctionType)
	{
	case HUD_FIND:
	case HUD_MAZE:
		sfRectangleShape_setPosition(mapRectangle, map[currentIndex.y][currentIndex.x].pos);
		sfRectangleShape_setFillColor(mapRectangle, blockColor[TILE_CURRENT]);

		sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
		break;
	default:
		break;
	}
	sfMutex_unlock(gameMutex);

	if (nbNodes > 0) {
		displayPath(_window);

		sprintf(mapStringBuffer, "NODES : %d", nbNodes);
		sfText_setString(mapText, mapStringBuffer);
		sfText_setPosition(mapText, vector2f(WINDOW_LENGTH - 100.f, 20.f));
		sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);
	}

	displayFrames(_window);
}

void loadMap()
{
	mapXSize = 20;
	mapYSize = 10;

	map = (Block**)calloc(mapYSize, sizeof(Block*));
	savedMap = (Block**)calloc(mapYSize, sizeof(Block*));

	for (int j = 0; j < mapYSize; j++)
	{
		map[j] = (Block*)calloc(mapXSize, sizeof(Block));
		savedMap[j] = (Block*)calloc(mapXSize, sizeof(Block));
	}

	defaultMap();
}

void removeType(TileType _type)
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			if (map[j][i].type == _type)
				map[j][i].type = TILE_PATH;
		}
	}
}

int getHCost(sfVector2i _index)
{
	int xLength = abs(_index.x - finishIndex.x);
	int yLength = abs(_index.y - finishIndex.y);
	int diff = min(xLength, yLength);
	return diff * DIAGONAL_COST + (xLength - diff) * STRAIGHT_COST + (yLength - diff) * STRAIGHT_COST;
}

/// <summary>
/// 
/// currentNode -> get the lowest fCost in openList
/// if currentNode is equal to finish return complete path
/// (put currentNode in the closed list ?)
/// remove currentNode from openList
/// 
/// // maybe not
/// get all of his neighbours, put them in the openList
/// set their parent
/// set their gCost equal to the parent gCost + 10 or 14 if moved diagonally
/// //
/// 
/// </summary>

void search()
{
	if (!openList) {
		savedMap = map;
		openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
		openList[0] = startIndex;
		map[openList[0].y][openList[0].x].type = TILE_OPEN;
		map[openList[0].y][openList[0].x].gCost = 0;
		nbOpenListElements = 1;
		nbClosedListElements = 0;
		nbNodes = 0;
	}
	else {
		sfVector2i closestIndex = openList[0];
		int lowestFCost = map[closestIndex.y][closestIndex.x].fCost;
		sfVector2i* equalFCosts = NULL;
		int nbEqualFCosts = 0;
		int count = 0;
		int* counts = NULL;
		for (int i = 1; i < nbOpenListElements; i++)
		{
			if (map[openList[i].y][openList[i].x].fCost == lowestFCost) {
				nbEqualFCosts++;
				equalFCosts = realloc(equalFCosts, nbEqualFCosts * sizeof(sfVector2i));
				equalFCosts[nbEqualFCosts - 1] = openList[i];
				counts = realloc(counts, nbEqualFCosts * sizeof(int));
				counts[nbEqualFCosts - 1] = i;
			}
			else if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
				closestIndex = openList[i];
				lowestFCost = map[openList[i].y][openList[i].x].fCost;
				nbEqualFCosts = 0;
				count = i;
			}
		}

		if (nbEqualFCosts > 0) {
			closestIndex = equalFCosts[0];
			int lowestHCost = map[equalFCosts[0].y][equalFCosts[0].x].hCost;
			count = counts[0];
			for (int i = 1; i < nbEqualFCosts; i++)
			{
				if (map[equalFCosts[i].y][equalFCosts[i].x].hCost < lowestHCost) {
					lowestHCost = map[equalFCosts[i].y][equalFCosts[i].x].hCost;
					closestIndex = equalFCosts[i];
					count = counts[i];
				}
			}
		}

		removeElementInOpenList(count);
		addNodeInClosedList(closestIndex);
		

		currentNode = closestIndex;
		if (equalsVectors2i(closestIndex, finishIndex)) {
			setSearchList();
		}

		free(equalFCosts);

		sfVector2i* possibleNeighbours = getPossibleNeighbours(closestIndex);

		int nbNeighbours = 0;
		sfVector2i* neighbours = getNeighbours(possibleNeighbours, &nbNeighbours);

		setNeighboursValues(neighbours, nbNeighbours, closestIndex);


		


		free(possibleNeighbours);
		free(neighbours);
	}
}

void resetMap()
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i] = savedMap[j][i];

			if (map[j][i].type == TILE_START)
				startIndex = vector2i(i, j);
			if (map[j][i].type == TILE_FINISH)
				finishIndex = vector2i(i, j);
		}
	}

	savedMap = map;
	openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	openList[0] = startIndex;
	map[openList[0].y][openList[0].x].type = TILE_OPEN;
	map[openList[0].y][openList[0].x].gCost = 0;
	nbOpenListElements = 1;

	closedList = NULL;
	nbClosedListElements = 0;
	nbNodes = 0;
}

void saveMap()
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			savedMap[j][i] = map[j][i];
		}
	}
}

void defaultMap()
{
	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * getBlockLength() / 2.f + BLOCK_OUTLINE_THICKNESS + getBlockOrigin(), WINDOW_HEIGHT / 1.75f - (float)mapYSize * getBlockLength() / 2.f + BLOCK_OUTLINE_THICKNESS + getBlockOrigin());

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i].type = TILE_PATH;
			map[j][i].index = vector2i(i, j);
			map[j][i].gCost = INT_MAX / 2;
			map[j][i].hCost = INT_MAX;
			map[j][i].fCost = INT_MAX;
			map[j][i].parent = NULLVECTOR2I;
			map[j][i].pos = addVectorsf(offsetPos, V2iToV2f(vector2i(getBlockLength() * i, getBlockLength() * j)));
			map[j][i].bounds = FloatRect(map[j][i].pos.x - getBlockOrigin(), map[j][i].pos.y - getBlockOrigin(), blockSize, blockSize);

			savedMap[j][i] = map[j][i];
		}
	}

	map[0][0].type = TILE_START;
	savedMap[0][0] = map[0][0];
	map[mapYSize - 1][mapXSize - 1].type = TILE_FINISH;
	savedMap[mapYSize - 1][mapXSize - 1] = map[mapYSize - 1][mapXSize - 1];
	startIndex = vector2i(0, 0);
	finishIndex = vector2i(mapXSize - 1, mapYSize - 1);

	nbOpenListElements = 0;
}

sfBool isIndexInArray(sfVector2i _index)
{
	if (_index.x < 0 || _index.y < 0 || _index.x >= mapXSize || _index.y >= mapYSize)
		return sfFalse;

	return sfTrue;
}

sfVector2i* getPossibleNeighbours(sfVector2i _closestIndex)
{
	sfVector2i* possibleNeighbours = (sfVector2i*)calloc(8, sizeof(sfVector2i));
	possibleNeighbours[0] = vector2i(_closestIndex.x - 1, _closestIndex.y - 1); // Top Left
	possibleNeighbours[1] = vector2i(_closestIndex.x, _closestIndex.y - 1); // Top
	possibleNeighbours[2] = vector2i(_closestIndex.x + 1, _closestIndex.y - 1); // Top Right
	possibleNeighbours[3] = vector2i(_closestIndex.x - 1, _closestIndex.y); // Middle Left
	possibleNeighbours[4] = vector2i(_closestIndex.x + 1, _closestIndex.y); // Middle Right
	possibleNeighbours[5] = vector2i(_closestIndex.x - 1, _closestIndex.y + 1); // Bottom Left
	possibleNeighbours[6] = vector2i(_closestIndex.x, _closestIndex.y + 1); // Bottom
	possibleNeighbours[7] = vector2i(_closestIndex.x + 1, _closestIndex.y + 1); // Bottom Right

	return possibleNeighbours;
}

sfVector2i* getNeighbours(sfVector2i* _possibleNeighbours, int* _nbNeighbours)
{
	sfVector2i* neighbours = NULL;
	for (int path = 0; path < 8; path++)
	{
		if (isIndexInArray(_possibleNeighbours[path])) {
			if (!isNodeSolid(_possibleNeighbours[path]) && !isNodeInClosedList(_possibleNeighbours[path])) {
				*_nbNeighbours += 1;
				neighbours = (sfVector2i*)realloc(neighbours, *_nbNeighbours * sizeof(sfVector2i));
				neighbours[*_nbNeighbours - 1] = _possibleNeighbours[path];
			}
		}
	}
	return neighbours;
}

sfBool isNodeSolid(sfVector2i _node)
{
	if (map[_node.y][_node.x].type == TILE_WALL)
		return sfTrue;

	return sfFalse;
}

void addNodeInOpenList(sfVector2i _node)
{
	sfMutex_lock(gameMutex);
	nbOpenListElements++;
	openList = (sfVector2i*)realloc(openList, nbOpenListElements * sizeof(sfVector2i));
	openList[nbOpenListElements - 1] = _node;
	sfMutex_unlock(gameMutex);
}

sfBool isNodeInOpenList(sfVector2i _node)
{
	for (int i = 0; i < nbOpenListElements; i++)
	{
		if (equalsVectors2i(_node, openList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void setNeighboursValues(sfVector2i* _neighbours, int _nbNeighbours, sfVector2i _parent)
{
	for (int path = 0; path < _nbNeighbours; path++)
	{
		int distanceFromParent = getDistanceFromParent(_neighbours[path], _parent);

		int newPath = map[_parent.y][_parent.x].gCost + distanceFromParent;

		sfBool isShorter = (newPath < map[_neighbours[path].y][_neighbours[path].x].gCost ? sfTrue : sfFalse);
		sfBool isInOpenList = isNodeInOpenList(_neighbours[path]);

		if (isShorter || isInOpenList) {
			if (isShorter) {
				map[_neighbours[path].y][_neighbours[path].x].gCost = newPath;
				map[_neighbours[path].y][_neighbours[path].x].fCost = map[_neighbours[path].y][_neighbours[path].x].gCost + map[_neighbours[path].y][_neighbours[path].x].hCost;
				map[_neighbours[path].y][_neighbours[path].x].parent = _parent;
			}

			if (!isInOpenList)
				addNodeInOpenList(_neighbours[path]);
		}
	}
}

int getDistanceFromParent(sfVector2i _neighbour, sfVector2i _parent)
{
	sfVector2i diff = vector2i(_parent.x - _neighbour.x, _parent.y - _neighbour.y);
	if (abs(diff.x) == 1 && (abs(diff.y)) == 1)
		return DIAGONAL_COST;
	else
		return STRAIGHT_COST;
}

void addNodeInClosedList(sfVector2i _node)
{
	sfMutex_lock(gameMutex);
	nbClosedListElements++;
	closedList = (sfVector2i*)realloc(closedList, nbClosedListElements * sizeof(sfVector2i));
	closedList[nbClosedListElements - 1] = _node;
	sfMutex_unlock(gameMutex);
}

sfBool isNodeInClosedList(sfVector2i _node)
{
	for (int i = 0; i < nbClosedListElements; i++)
	{
		if (equalsVectors2i(_node, closedList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void setSearchList()
{
	sfMutex_lock(gameMutex);

	sfVector2i parent = openList[nbOpenListElements - 1];
	nbNodes = 0;
	searchList = NULL;
	while (!equalsVectors2i(parent, startIndex))
	{
		nbNodes++;
		searchList = (sfVector2i*)realloc(searchList, nbNodes * sizeof(sfVector2i));
		searchList[nbNodes - 1] = parent;
		parent = map[parent.y][parent.x].parent;
	}

	sfMutex_unlock(gameMutex);
}

void removeElementInOpenList(int _count)
{
	sfMutex_lock(gameMutex);
	for (int i = _count; i < nbOpenListElements - 1; i++)
	{
		openList[i] = openList[i + 1];
	}

	nbOpenListElements--;
	openList = (sfVector2i*)realloc(openList, nbOpenListElements * sizeof(openList));
	sfMutex_unlock(gameMutex);
}

void find()
{
	savedMap = map;
	free(openList);
	free(closedList);
	free(searchList);
	openList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	closedList = NULL;
	searchList = NULL;
	openList[0] = startIndex;
	map[openList[0].y][openList[0].x].type = TILE_OPEN;
	map[openList[0].y][openList[0].x].gCost = 0;
	nbOpenListElements = 1;
	nbClosedListElements = 0;
	nbNodes = 0;

	float findTimer = 0.f;

	while (openList != NULL) {
		//findTimer += getDeltaTimeThread2();
		findTimer += getDeltaTime();

		if (findTimer > timeBetweenFrames)
			findTimer = 0.f;
		else
			continue;

		currentIndex = openList[0];
		int lowestFCost = map[currentIndex.y][currentIndex.x].fCost;
		sfVector2i* equalFCosts = NULL;
		int nbEqualFCosts = 0;
		int count = 0;
		int* counts = NULL;
		for (int i = 1; i < nbOpenListElements; i++)
		{
			if (map[openList[i].y][openList[i].x].fCost == lowestFCost) {
				nbEqualFCosts++;
				equalFCosts = realloc(equalFCosts, nbEqualFCosts * sizeof(sfVector2i));
				equalFCosts[nbEqualFCosts - 1] = openList[i];
				counts = realloc(counts, nbEqualFCosts * sizeof(int));
				counts[nbEqualFCosts - 1] = i;
			}
			else if (map[openList[i].y][openList[i].x].fCost < lowestFCost) {
				currentIndex = openList[i];
				lowestFCost = map[openList[i].y][openList[i].x].fCost;
				nbEqualFCosts = 0;
				count = i;
			}
		}

		if (nbEqualFCosts > 0) {
			currentIndex = equalFCosts[0];
			int lowestHCost = map[equalFCosts[0].y][equalFCosts[0].x].hCost;
			count = counts[0];
			for (int i = 1; i < nbEqualFCosts; i++)
			{
				if (map[equalFCosts[i].y][equalFCosts[i].x].hCost < lowestHCost) {
					lowestHCost = map[equalFCosts[i].y][equalFCosts[i].x].hCost;
					currentIndex = equalFCosts[i];
					count = counts[i];
				}
			}
		}

		removeElementInOpenList(count);
		addNodeInClosedList(currentIndex);


		currentNode = currentIndex;
		if (equalsVectors2i(currentIndex, finishIndex)) {
			setSearchList();
			return;
		}
		if (nbOpenListElements > 0)
			setSearchList();

		free(equalFCosts);

		sfVector2i* possibleNeighbours = getPossibleNeighbours(currentIndex);

		int nbNeighbours = 0;
		sfVector2i* neighbours = getNeighbours(possibleNeighbours, &nbNeighbours);

		setNeighboursValues(neighbours, nbNeighbours, currentIndex);





		free(possibleNeighbours);
		free(neighbours);
	}
}

void changeMapSize(int _mapXSize, int _mapYSize)
{
	sfMutex_lock(gameMutex);

	if (_mapXSize > 1130 || _mapXSize < 1 || _mapYSize > 1113 || _mapYSize < 1)
		return;

	mapXSize = _mapXSize;
	mapYSize = _mapYSize;

	free(map);
	free(savedMap);

	map = (Block**)calloc(mapYSize, sizeof(Block*));
	savedMap = (Block**)calloc(mapYSize, sizeof(Block*));

	for (int j = 0; j < mapYSize; j++)
	{
		map[j] = (Block*)calloc(mapXSize, sizeof(Block));
		savedMap[j] = (Block*)calloc(mapXSize, sizeof(Block));
	}

	defaultMap();

	//map = (Block**)realloc(map, mapYSize * sizeof(Block*));
	//savedMap = (Block**)realloc(map, mapYSize * sizeof(Block*));
	//
	//for (int j = 0; j < mapYSize; j++)
	//{
	//	map[j] = (Block*)realloc(map, mapXSize * sizeof(Block));
	//	savedMap[j] = (Block*)realloc(map, mapXSize * sizeof(Block));
	//}

	sfMutex_unlock(gameMutex);
}

void toggleOpenList()
{
	showOpenList = !showOpenList;
}

void toggleClosedList()
{
	showClosedList = !showClosedList;
}

void toggleValues()
{
	showValues = !showValues;
}

void displayList(Window* _window, sfVector2i* _list, int _i)
{
	sfRectangleShape_setPosition(mapRectangle, map[_list[_i].y][_list[_i].x].pos);
	sfRenderTexture_drawRectangleShape(_window->renderTexture, mapRectangle, NULL);
}

void displayValues(Window* _window, sfVector2i* _list, int _i)
{
	sfMutex_lock(gameMutex);

	char buffer[13];
	sfFloatRect tmpTextBounds = FloatRect(0.f, 0.f, 0.f, 0.f);

	sprintf(buffer, "%d", map[_list[_i].y][_list[_i].x].gCost);
	sfText_setString(mapText, buffer);
	sfText_setCharacterSize(mapText, 10);
	sfText_setPosition(mapText, addVectorsf(map[_list[_i].y][_list[_i].x].pos, vector2f(-getBlockOrigin() / 2.f, -getBlockOrigin() / 2.f)));
	tmpTextBounds = sfText_getLocalBounds(mapText);
	sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
	sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

	sprintf(buffer, "%d", map[_list[_i].y][_list[_i].x].hCost);
	sfText_setString(mapText, buffer);
	sfText_setCharacterSize(mapText, 10);
	sfText_setPosition(mapText, addVectorsf(map[_list[_i].y][_list[_i].x].pos, vector2f(getBlockOrigin() / 2.f, -getBlockOrigin() / 2.f)));
	tmpTextBounds = sfText_getLocalBounds(mapText);
	sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
	sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

	sprintf(buffer, "%d", map[_list[_i].y][_list[_i].x].fCost);
	sfText_setString(mapText, buffer);
	sfText_setCharacterSize(mapText, 16);
	sfText_setPosition(mapText, addVectorsf(map[_list[_i].y][_list[_i].x].pos, vector2f(0.f, 10.f)));
	tmpTextBounds = sfText_getLocalBounds(mapText);
	sfText_setOrigin(mapText, vector2f(tmpTextBounds.width / 2.f, tmpTextBounds.height));
	sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);

	sfMutex_unlock(gameMutex);
}

void displayPath(Window* _window)
{
	sfVertexArray_clear(mapVertexArray);

	//mapVertex.position = map[finishIndex.y][finishIndex.x].pos;
	//sfVertexArray_append(mapVertexArray, mapVertex);
	//mapVertex.position = map[searchList[0].y][searchList[0].x].pos;
	//sfVertexArray_append(mapVertexArray, mapVertex);
	for (int i = 2; i < nbNodes; i++)
	{
		mapVertex.position = map[searchList[i].y][searchList[i].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
		mapVertex.position = map[searchList[i - 1].y][searchList[i - 1].x].pos;
		sfVertexArray_append(mapVertexArray, mapVertex);
	}
	mapVertex.position = map[searchList[nbNodes - 1].y][searchList[nbNodes - 1].x].pos;
	sfVertexArray_append(mapVertexArray, mapVertex);
	mapVertex.position = map[startIndex.y][startIndex.x].pos;
	sfVertexArray_append(mapVertexArray, mapVertex);

	sfRenderTexture_drawVertexArray(_window->renderTexture, mapVertexArray, NULL);
}

float getBlockLength()
{
	return blockSize + BLOCK_OUTLINE_THICKNESS * 2.f;
}

float getBlockOrigin()
{
	return blockSize / 2.f;
}

float getBlockSize()
{
	return blockSize;
}

void setBlockSize(float _blockSize)
{
	blockSize = _blockSize;


	sfVector2f offsetPos = vector2f(WINDOW_LENGTH / 2.f - (float)mapXSize * getBlockLength() / 2.f + BLOCK_OUTLINE_THICKNESS + getBlockOrigin(), WINDOW_HEIGHT / 1.75f - (float)mapYSize * getBlockLength() / 2.f + BLOCK_OUTLINE_THICKNESS + getBlockOrigin());

	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i].pos = addVectorsf(offsetPos, V2iToV2f(vector2i(getBlockLength() * i, getBlockLength() * j)));
			map[j][i].bounds = FloatRect(map[j][i].pos.x - getBlockOrigin(), map[j][i].pos.y - getBlockOrigin(), blockSize, blockSize);

			savedMap[j][i] = map[j][i];
		}
	}

	sfRectangleShape_setSize(mapRectangle, vector2f(blockSize, blockSize));
	sfRectangleShape_setOrigin(mapRectangle, vector2f(getBlockOrigin(), getBlockOrigin()));
}

void setupMazeMap()
{
	for (int j = 0; j < mapYSize; j++)
	{
		for (int i = 0; i < mapXSize; i++)
		{
			map[j][i].type = TILE_WALL;
		}
	}

	sfVector2i randomStartIndex = vector2i(rand() % mapXSize, rand() % mapYSize);
	map[randomStartIndex.y][randomStartIndex.x].type = TILE_START;
	map[mapYSize - 1][mapXSize - 1].type = TILE_FINISH;
	startIndex = randomStartIndex;



	free(mazeStack);
	mazeStack = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	nbMazeStackElements = 1;
	mazeStack[0] = startIndex;

	free(mazeOpenList);
	mazeOpenList = (sfVector2i*)calloc(1, sizeof(sfVector2i));
	nbMazeOpenListElements = 1;
	mazeOpenList[0] = mazeStack[0];
}

void maze()
{
	setupMazeMap();

	float mazeTimer = 0.f;

	while (mazeStack != NULL)
	{
		mazeTimer += getDeltaTimeThread2();

		if (mazeTimer > timeBetweenFrames)
			mazeTimer = 0.f;
		else
			continue;


		int currentStackElements = nbMazeStackElements - 1;
		currentIndex = mazeStack[currentStackElements];


		sfVector2i* possibleNeighbours = getPossibleMazeNeighbours(currentIndex);

		int nbNeighbours = 0;
		sfVector2i* neighbours = getMazeNeighbours(possibleNeighbours, &nbNeighbours);

		if (nbNeighbours > 0) {
			int chosenNeighbour = rand() % nbNeighbours;

			addNodeToMazeStack(neighbours[chosenNeighbour]);
			addNodeToMazeOpenList(neighbours[chosenNeighbour]);

			if (neighbours[chosenNeighbour].x != currentIndex.x) {
				sfBool isXNeighbourNegative = (neighbours[chosenNeighbour].x < currentIndex.x ? sfTrue : sfFalse);

				for (int xSubBlockIndex = 1; xSubBlockIndex <= subBlocks; xSubBlockIndex++)
				{
					map[currentIndex.y][currentIndex.x + (isXNeighbourNegative ? -xSubBlockIndex : xSubBlockIndex)].type = TILE_PATH;
				}
			}

			if (neighbours[chosenNeighbour].y != currentIndex.y) {
				sfBool isYNeighbourNegative = (neighbours[chosenNeighbour].y < currentIndex.y ? sfTrue : sfFalse);

				for (int ySubBlockIndex = 1; ySubBlockIndex <= subBlocks; ySubBlockIndex++)
				{
					map[currentIndex.y + (isYNeighbourNegative ? -ySubBlockIndex : ySubBlockIndex)][currentIndex.x].type = TILE_PATH;
				}
			}
		}
		else {
			eraseMazeStackElement(currentStackElements);
		}

	}
}

sfVector2i* getPossibleMazeNeighbours(sfVector2i _currentIndex)
{
	sfVector2i* possibleNeighbours = (sfVector2i*)calloc(4, sizeof(sfVector2i));
	possibleNeighbours[0] = vector2i(_currentIndex.x, _currentIndex.y - 1 * subBlocks); // Top
	possibleNeighbours[1] = vector2i(_currentIndex.x - 1 * subBlocks, _currentIndex.y); // Left
	possibleNeighbours[2] = vector2i(_currentIndex.x + 1 * subBlocks, _currentIndex.y); // Right
	possibleNeighbours[3] = vector2i(_currentIndex.x, _currentIndex.y + 1 * subBlocks); // Bottom

	return possibleNeighbours;
}

sfVector2i* getMazeNeighbours(sfVector2i* _possibleMazeNeighbours, int* _nbNeighbours)
{
	sfVector2i* neighbours = NULL;
	for (int path = 0; path < 4; path++)
	{
		if (isIndexInArray(_possibleMazeNeighbours[path])) {
			if (!isNodeInMazeOpenList(_possibleMazeNeighbours[path])) {
				*_nbNeighbours += 1;
				neighbours = (sfVector2i*)realloc(neighbours, *_nbNeighbours * sizeof(sfVector2i));
				neighbours[*_nbNeighbours - 1] = _possibleMazeNeighbours[path];
			}
		}
	}
	return neighbours;
}

sfBool isNodeInMazeOpenList(sfVector2i _node)
{
	for (int i = 0; i < nbMazeOpenListElements; i++)
	{
		if (equalsVectors2i(_node, mazeOpenList[i]))
			return sfTrue;
	}

	return sfFalse;
}

void addNodeToMazeStack(sfVector2i _node)
{
	sfMutex_lock(gameMutex);
	nbMazeStackElements++;
	mazeStack = (sfVector2i*)realloc(mazeStack, nbMazeStackElements * sizeof(sfVector2i));
	mazeStack[nbMazeStackElements - 1] = _node;
	sfMutex_unlock(gameMutex);
}

void addNodeToMazeOpenList(sfVector2i _node)
{
	sfMutex_lock(gameMutex);
	nbMazeOpenListElements++;
	mazeOpenList = (sfVector2i*)realloc(mazeOpenList, nbMazeOpenListElements * sizeof(sfVector2i));
	mazeOpenList[nbMazeOpenListElements - 1] = _node;
	sfMutex_unlock(gameMutex);
}

void eraseMazeStackElement(int _element)
{
	sfMutex_lock(gameMutex);
	for (int i = _element; i < nbMazeStackElements - 1; i++)
	{
		mazeStack[i] = mazeStack[i + 1];
	}

	nbMazeStackElements--;
	mazeStack = (sfVector2i*)realloc(mazeStack, nbMazeStackElements * sizeof(sfVector2i));
	sfMutex_unlock(gameMutex);
}

void reduceMapSize()
{
	setBlockSize(getBlockSize() - 2.f);
}

void increaseMapSize()
{
	setBlockSize(getBlockSize() + 2.f);
}

void changeFrames()
{
	float tmpTime = 0.f;
	while (!isPressing(sfKeyEnter))
	{
		for (sfKeyCode key = sfKeyNumpad0; key <= sfKeyNumpad9; key++)
		{
			if (hasPressed(key) && tmpTime < 1000000.f) {
				tmpTime *= 10.f;
				tmpTime += (float)(key - sfKeyNumpad0);
			}
		}

		if (hasPressed(sfKeyBackspace)) {
			int iTimeBetweenFrames = (int)tmpTime;
			iTimeBetweenFrames /= 10;
			tmpTime = (float)iTimeBetweenFrames;
		}
	}
	timeBetweenFrames = tmpTime;
}

void displayFrames(Window* _window)
{
	sprintf(mapStringBuffer, "Time Between Frames : %.0f", timeBetweenFrames);
	sfText_setString(mapText, mapStringBuffer);
	sfText_setPosition(mapText, vector2f(WINDOW_LENGTH - 300.f, 160.f));
	sfRenderTexture_drawText(_window->renderTexture, mapText, NULL);
}
