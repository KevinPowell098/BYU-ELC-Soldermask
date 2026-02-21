// lcd.h
#ifndef LCD_H
#define LCD_H

#include <Adafruit_HX8357.h>
#pragma once

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

extern uint16_t* framebuffer;   // pointer to the buffer

void initFramebuffer();

uint16_t getColorFromGradient(const uint16_t* colors, size_t color_cnt, uint16_t place);

void drawPixelFB(uint16_t x, uint16_t y, uint16_t color);

void drawLineFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t weight, uint16_t color);
void drawArcFB(uint16_t x, uint16_t y, uint16_t r, uint16_t a, uint16_t b, uint16_t weight, uint16_t color);
void drawRectOutlineFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t weight, uint16_t color);

void drawRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void drawGradRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t* color, size_t color_cnt);
void fillScreenFB(uint16_t color);

void pushFramebuffer(Adafruit_HX8357& tft);

constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0x1F) << 11) |
         ((g & 0x3F) << 5)  |
         (b & 0x1F);
}

#endif
