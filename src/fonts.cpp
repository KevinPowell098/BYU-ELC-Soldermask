#include "fonts.h"
#include "font_stan7.h"
#include "font_stan9.h"
#include <cmath>

#define TFT_WIDTH  320
#define TFT_HEIGHT 480


size_t drawCharFB(int16_t x, int16_t y, char c, uint16_t color, const Font& font) {
  auto it = font.chars->find(c);
  if (it == font.chars->end()) {
    drawRectFB(x, y, font.WIDTH, font.HEIGHT, color);
    return font.WIDTH;
  }

  const auto& bitmap = it->second;
  size_t h = bitmap.size();

  size_t width;

  for (size_t i = 0; i < h; i++) {
    width = bitmap[i].size();

    for (size_t j = 0; j < width; j++) {
      if (bitmap[i][j]) drawPixelFB(x + j, y + i, color);
    }
  }

  return width;
}

void drawWordFB(int16_t x, int16_t y, char* text, uint16_t color, const Font& font) {
  const uint8_t CHAR_W = font.WIDTH;
  const uint8_t CHAR_H = font.HEIGHT;
  const uint8_t MARGIN = std::ceil(CHAR_W / 4.0);

  uint16_t text_x = x;
  size_t last_w;
  size_t len = strlen(text);

  for (size_t i = 0; i < len; i++) {
    if (text_x + CHAR_W + MARGIN >= TFT_WIDTH) {
      text_x = x;
      y += CHAR_H + MARGIN;

      if (y > TFT_HEIGHT) return;
    }

    last_w = drawCharFB(text_x, y, text[i], color, font);
    text_x += last_w + MARGIN;
  }
}