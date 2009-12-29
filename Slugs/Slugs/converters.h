#pragma once

#include <string>

#include "color.h"

// Converts a bool to its string representation
std::string BoolToString(bool value);

// Converts a string to a Color
Color ColorFromString(const std::string& string);