#pragma once

#include "debug.h"
#include "vec2.h"
#include "fastmath.h"

// Calculates the launch angle needed to hit a target
bool CalculateLaunchDirection(const Vec2f& from, const Vec2f& to, float speed, float absGravity, Vec2f& direction, bool chooseHigh = false);
bool CalculateLaunchDirection2(const Vec2f& from, const Vec2f& to, float speed, float absGravity, Vec2f& directionLow, Vec2f& directionHigh);