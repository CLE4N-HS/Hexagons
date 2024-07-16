#pragma once
#include "tools.h"
#include "math.h"

#define PI 3.1415927f

float Lerp(float _a, float _b, float _t);
sfVector2f PolarCoords(sfVector2f _startPos, float _distance, float _angle);
