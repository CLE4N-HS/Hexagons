#include "customMath.h"

float Lerp(float _a, float _b, float _t)
{
	return (_b - _a) * _t + _a;
}

sfVector2f PolarCoords(sfVector2f _startPos, float _distance, float _angle)
{
	return vector2f(_startPos.x + (_distance * cosf(_angle)), _startPos.y + (_distance * sinf(_angle)));
}

float getAngleBetweenVectors(sfVector2f _v1, sfVector2f _v2)
{
    return (atan2f(_v2.y, _v2.x) - (atan2f(_v1.y, _v1.x)));
}

float getVectorsAngle(sfVector2f _v1, sfVector2f _v2)
{
    return atan2f(_v2.y - _v1.y, _v2.x - _v1.x);
}

float triangleSign(sfVector2f p1, sfVector2f p2, sfVector2f p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

sfBool isPointInTriangle(sfVector2f pt, sfVector2f v1, sfVector2f v2, sfVector2f v3)
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

float getSqrMagnitude(sfVector2f _v1, sfVector2f _v2)
{
    return (((_v2.x - _v1.x) * (_v2.x - _v1.x)) + ((_v2.y - _v1.y) * (_v2.y - _v1.y)));
}
