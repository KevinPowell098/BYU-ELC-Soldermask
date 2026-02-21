#define FONTS_H

#include "lcd/lcd.h"
#include <map>
#include <vector>

#pragma once

struct Font {
  const uint16_t* chars;
  const size_t length;
  const char* name;
  const int WIDTH;
  const int HEIGHT;
};

int32_t findChar(const char c, const Font& font);
uint16_t drawCharFB(int16_t x, int16_t y, const char c, uint16_t color, const Font& font);
void drawWordFB(int16_t x, int16_t y, const char* text, uint16_t color, const Font& font);
uint16_t getWordLength(const char* text, const Font& font);
