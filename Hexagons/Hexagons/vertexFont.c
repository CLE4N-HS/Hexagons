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
		appendNewVertexArrayPos(_window->vertexArray, &tmpVertex, addVectorsf(tmpVertex.position, halfSize));
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
	int absInterger = abs(_integer);
	for (int i = 9; i > 0; i--) // based on INT_MAX nb digits
	{
		int divisor = 1;
		for (int digit = 0; digit < i; digit++)
		{
			divisor *= 10;
		}
		int digit = absInterger / divisor;
		if (digit > 0) {
			drawVertexDigit(_window, digit, pos, _color, _size);
			pos = addVectorsf(pos, _space);
		}
	}
}
