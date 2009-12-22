#pragma once

#include "vector2.h"
#include "fastmath.h"

bool RayCircleIntersection(const Vector2& rayStart, const Vector2& rayDirection, const Vector2& circleCenter, float circleRadius, float maxRange, Vector2& intersection);