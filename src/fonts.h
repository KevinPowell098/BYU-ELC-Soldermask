#define FONTS_H

#include <lcd.h>
#pragma once

size_t drawCharFB(int16_t x, int16_t y, char c, uint16_t color);
void drawWordFB(int16_t x, int16_t y, char* text, uint16_t color);