#include "vertexFont.h"
#include "customMath.h"

void drawVertexDigit(Window* _window, int _digit, sfVector2f _pos, sfColor _color, sfVector2f _size)
{
	sfVertex tmpVertex;

	sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

	tmpVertex.position = _pos;
	tmpVertex.color = _color;

	sfVector2f halfSize = MultiplyVector2f(_size, 0.5f);

	switch (_digit)
	{
	case 0:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -_size.y)));
		break;
	case 1:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		break;
	case 2:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		break;
	case 3:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		break;
	case 4:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		break;
	case 5:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		break;
	case 6:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		break;
	case 7:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		break;
	case 8:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, tmpVertex.position);
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		break;
	case 9:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-_size.x, 0.f)));
		break;
	default:
		break;
	}

	drawAndClearVertexArray(_window);
}

void drawVertexInteger(Window* _window, int _integer, sfVector2f _pos, sfColor _color, sfVector2f _size, sfVector2f _space)
{
	sfVector2f pos = _pos;
	int absInterger = _integer;
	if (_integer < 0) {
		drawVertexSymbol(_window, VERTEX_MINUS, pos, _color, vector2f(_size.x / 2.f, _size.y));
		pos.x += _size.x;
		absInterger *= -1;
	}

	if (_integer == 0) {
		drawVertexDigit(_window, 0, pos, _color, _size);
	}
	else {
		for (int i = 9; i > 0; i--) // based on INT_MAX nb digits
		{
			int divisor = 1;
			for (int integerDigit = 1; integerDigit < i; integerDigit++)
			{
				divisor *= 10;
			}
			int digit = absInterger / divisor;
			if (digit > 0) {
				digit %= 10;
				drawVertexDigit(_window, digit, pos, _color, _size);
				pos = addVectorsf(pos, _space);
			}
		}
	}
}

void drawVertexSymbol(Window* _window, VertexSymbols _symbol, sfVector2f _pos, sfColor _color, sfVector2f _size)
{
	sfVertex tmpVertex;

	sfVertexArray_setPrimitiveType(_window->vertexArray, sfLineStrip);

	tmpVertex.position = _pos;
	tmpVertex.color = _color;

	sfVector2f halfSize = MultiplyVector2f(_size, 0.5f);

	switch (_symbol)
	{
	case VERTEX_PLUS:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(halfSize.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, _size.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, -halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(-halfSize.x, 0.f)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		break;
	case VERTEX_MINUS:
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(0.f, halfSize.y)));
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, vector2f(_size.x, 0.f)));
		break;
	default:
		break;
	}

	drawAndClearVertexArray(_window);
}
