#define FONTS_H

#include <lcd.h>
#include <map>
#include <vector>

#pragma once

struct Font {
  const std::map<char, std::vector<std::vector<bool>>>* chars;
  const char* name;
  const int WIDTH;
  const int HEIGHT;
};

size_t drawCharFB(int16_t x, int16_t y, char c, uint16_t color, const Font& font);
void drawWordFB(int16_t x, int16_t y, char* text, uint16_t color, const Font& font);