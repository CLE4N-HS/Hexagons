#include "customMath.h"

float Lerp(float _a, float _b, float _t)
{
	return (_b - _a) * _t + _a;
}

sfVector2f PolarCoords(sfVector2f _startPos, float _distance, float _angle)
{
	return vector2f(_startPos.x + (_distance * cosf(_angle)), _startPos.y + (_distance * sinf(_angle)));
}

float triangleSign(sfVector2f p1, sfVector2f p2, sfVector2f p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

sfBool PointInTriangle(sfVector2f pt, sfVector2f v1, sfVector2f v2, sfVector2f v3)
{
    float d1, d2, d3;
    sfBool has_neg, has_pos;

    d1 = triangleSign(pt, v1, v2);
    d2 = triangleSign(pt, v2, v3);
    d3 = triangleSign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

sfBool isPointInHexagon(sfVector2f _point, sfVector2f _center, sfVector2f _corners[6])
{
    for (int i = 0; i < 6; i++)
    {
        if (PointInTriangle(_point, _center, _corners[i], (i == 6 ? _corners[0] : _corners[i + 1])))
            return sfTrue;
    }

    return sfFalse;
}
