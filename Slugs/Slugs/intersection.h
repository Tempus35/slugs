#pragma once

#include "vec2.h"
#include "fastmath.h"

bool RayCircleIntersection(const Vec2f& rayStart, const Vec2f& rayDirection, const Vec2f& circleCenter, float circleRadius, float maxRange, Vec2f& intersection);