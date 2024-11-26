#pragma once
#include "tools.h"
#include "math.h"

#define PI 3.1415927f
#define DEG2RAD 0.017453292f
#define RAD2DEG 57.29578f

float Lerp(float _a, float _b, float _t);
sfVector2f PolarCoords(sfVector2f _startPos, float _distance, float _angle);
float getAngleBetweenVectors(sfVector2f _v1, sfVector2f _v2);
float getVectorsAngle(sfVector2f _v1, sfVector2f _v2);
float triangleSign(sfVector2f p1, sfVector2f p2, sfVector2f p3);
sfBool isPointInTriangle(sfVector2f pt, sfVector2f v1, sfVector2f v2, sfVector2f v3);
float getSqrMagnitude(sfVector2f _v1, sfVector2f _v2);