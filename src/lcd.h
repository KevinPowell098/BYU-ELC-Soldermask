// lcd.h
#ifndef LCD_H
#define LCD_H

#include <Adafruit_HX8357.h>

extern uint16_t* framebuffer;   // pointer to the buffer

void initFramebuffer();

void drawPixelFB(int16_t x, int16_t y, uint16_t color);
void drawRectFB(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillScreenFB(uint16_t color);

void pushFramebuffer(Adafruit_HX8357& tft);

#endif
