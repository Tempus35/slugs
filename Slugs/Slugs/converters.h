#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "color.h"

// Converts a bool to its string representation
std::string BoolToString(bool value);

// Splits a string into elements
std::vector<std::string> StringSplit(const std::string& string, char delimiter);