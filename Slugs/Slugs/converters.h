#pragma once

#include <sfml/system.hpp>

#include <string>
#include <sstream>
#include <vector>

#include "color.h"
#include "vec2.h"

// Converts a bool to its string representation
std::string BoolToString(bool value);

// Splits a string into elements
std::vector<std::string> StringSplit(const std::string& string, char delimiter);

// Splits a string into a command and it's arguments
std::vector<std::string> SplitCommand(std::string& command, const std::string& string, char delimiter);

// Converts a Vec2f to a Vec2i
Vec2i ConvertVector(const Vec2f& v);

// Converts a Vec2i to a Vec2f
Vec2f ConvertVector(const Vec2i& v);

// Converts an SFML Key code to a char
bool SFMLKeyToChar(sf::Key::Code key, bool shift, char& c);