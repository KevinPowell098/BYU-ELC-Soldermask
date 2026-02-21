#include "fonts.h"
#include <cmath>

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#define W_OFFSET 1
#define H_OFFSET 2
#define C_OFFSET 3
#define W_UINT16 16
#define MASK     0x01U

#define W_SPACE  0.5
#define W_MARGIN 0.25


// function to return start position of a letter, -1 if not found
int32_t findChar(const char c, const Font& font) {
  // iterate through every letter in the font
  for (int16_t i = 0; i < font.length;) {
    // return position of char if found 
    if (font.chars[i] == c) {
      return i;
    }
    // increment position by height of letter data otherwise
    i += (font.chars[i + H_OFFSET] + C_OFFSET);
  }

  return -1;
}

// function to draw letter into buffer, return width of letter
uint16_t drawCharFB(int16_t x, int16_t y, const char c, uint16_t color, const Font& font) {
  // check if char does not exist in font array
  int16_t pos = findChar(c, font);
  if (pos == -1) {
    // return portion of font width if char is a space
    if (c == ' ') return ceil(font.WIDTH * W_SPACE);

    // otherwise print default shape and then return font width
    drawRectFB(x, y, font.WIDTH, font.HEIGHT, color);
    return font.WIDTH;
  }

  uint16_t width  = font.chars[pos + W_OFFSET];
  uint16_t height = font.chars[pos + H_OFFSET];

  pos += C_OFFSET;
  for (uint16_t c_y = 0; c_y < height; c_y++) {
    for (uint16_t c_x = 0; c_x < width; c_x ++) {
      if (font.chars[pos] & (MASK << (W_UINT16 - (c_x + 1)))) {
        drawPixelFB(x + c_x, y + c_y, color);
      }
    }
    pos++;
  }

  return width;
}

// function to draw a string of chars in to the buffer
void drawWordFB(int16_t x, int16_t y, const char* text, uint16_t color, const Font& font) {
  const uint8_t CHAR_W = font.WIDTH;
  const uint8_t CHAR_H = font.HEIGHT;
  const uint8_t MARGIN = std::ceil(CHAR_W * W_MARGIN);

  uint16_t text_x = x;
  uint16_t last_w;
  uint16_t len = strlen(text);

  for (uint16_t i = 0; i < len; i++) {
    if (text_x + CHAR_W + MARGIN >= TFT_WIDTH) {
      text_x = x;
      y += CHAR_H + MARGIN;

      if (y > TFT_HEIGHT) return;
    }

    last_w = drawCharFB(text_x, y, text[i], color, font);
    text_x += last_w + MARGIN;
  }
}

uint16_t getWordLength(const char* text, const Font& font) {
  uint16_t len = strlen(text);
  uint16_t offset = 1;
  int32_t pos;
  uint16_t length = 0;

  const uint8_t CHAR_W = font.WIDTH;
  const uint8_t MARGIN = std::ceil(CHAR_W * W_MARGIN);
  
  for (uint16_t i = 0; i < len; i++) {
    pos = findChar(text[i], font);
    if (text[i] == ' ') {
      length += font.WIDTH;
    } else if (pos >= 0) {
      // Add character widths
      length += font.chars[pos + offset];

      // Account for spaces
      if (i) length += MARGIN;
    }
  }

  return length;
}