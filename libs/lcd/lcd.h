// lcd.h
#ifndef LCD_H
#define LCD_H

#include <Adafruit_HX8357.h>
#include <functional>
#include "helpers/helpers.h"
#pragma once

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

extern uint16_t* framebuffer;   // pointer to the buffer

class Box {
  private:
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;

  public:
    // page number 0-3 to represent menu page
    int16_t page;

    void (*operation)();   // function pointer

    Box() : operation(nullptr) {}
    
    Box(int16_t _x1, int16_t _y1, int16_t _x2, int16_t _y2) {
      x1 = _x1;
      y1 = _y1;
      x2 = _x2;
      y2 = _y2;

      page = -1;
      operation = nullptr;
    }

    // update class members safely
    void _set_x1(int16_t _x1) { 
      if (_x1 <= x2) {
        x1 = clampInt(_x1, 0, TFT_WIDTH); 
      } else {
        x1 = x2;
        x2 = clampInt(_x1, 0, TFT_WIDTH); 
      }
    }

    void _set_y1(int16_t _y1) {
      if (_y1 <= y2) {
        y1 = clampInt(_y1, 0, TFT_HEIGHT);
      } else {
        y1 = y2;
        y2 = clampInt(_y1, 0, TFT_HEIGHT);
      }
    }

    void _set_x2(int16_t _x2) { 
      if (_x2 >= x1) {
        x2 = clampInt(_x2, 0, TFT_WIDTH); 
      } else {
        x2 = x1;
        x1 = clampInt(_x2, 0, TFT_WIDTH); 
      }
    }

    void _set_y2(int16_t _y2) {
      if (_y2 >= y1) {
        y2 = clampInt(_y2, 0, TFT_HEIGHT);
      } else {
        y2 = y1;
        y1 = clampInt(_y2, 0, TFT_HEIGHT);
      }
    }

    // return class values
    int16_t _get_x1() { return x1; }
    int16_t _get_y1() { return y1; }
    int16_t _get_x2() { return x2; }
    int16_t _get_y2() { return y2; }

    // test if item is touched
    bool _contains(int16_t x, int16_t y) {
      return (x >= x1 && x <= x2 && 
              y >= y1 && y <= y2);
    }

    void run(int value) {
      if (operation) {
        operation();
      }
    }
};

void initFramebuffer();

uint16_t getColorFromGradient(const uint16_t* colors, size_t color_cnt, uint16_t place);

void drawPixelFB(uint16_t x, uint16_t y, uint16_t color);
void drawLineFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t weight, uint16_t color);
void drawArcFB(uint16_t x, uint16_t y, uint16_t r, uint16_t a, uint16_t b, uint16_t weight, uint16_t color);
Box drawRectOutlineFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t weight, uint16_t color);
Box drawRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
Box drawGradRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t* color, size_t color_cnt);
void fillScreenFB(uint16_t color);

void pushFramebuffer(Adafruit_HX8357& tft);

constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0x1F) << 11) |
         ((g & 0x3F) << 5)  |
         (b & 0x1F);
}

Box drawThreePointTriangleFB(uint16_t x1u, uint16_t y1u,
                              uint16_t x2u, uint16_t y2u,
                              uint16_t x3u, uint16_t y3u,
                              uint16_t r_in,
                              uint16_t* colors, size_t color_cnt);

#endif
