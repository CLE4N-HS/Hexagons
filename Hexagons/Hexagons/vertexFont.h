#pragma once
#include "windowManager.h"

typedef enum {
	VERTEX_PLUS,
	VERTEX_MINUS
}VertexSymbols;

void drawVertexDigit(Window* _window, int _digit, sfVector2f _pos, sfColor _color, sfVector2f _size);
void drawVertexInteger(Window* _window, int _integer, sfVector2f _pos, sfColor _color, sfVector2f _size, sfVector2f _space);
void drawVertexSymbol(Window* _window, VertexSymbols _symbol, sfVector2f _pos, sfColor _color, sfVector2f _size);
