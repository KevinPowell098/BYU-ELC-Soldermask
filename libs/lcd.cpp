#include "lcd.h"
#include <esp_heap_caps.h>

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

uint16_t* framebuffer = nullptr;

void initFramebuffer() {
  if (!framebuffer) {
    framebuffer = new uint16_t[TFT_WIDTH * TFT_HEIGHT];
  }
  memset(framebuffer, 0, TFT_WIDTH * TFT_HEIGHT * sizeof(uint16_t));
}

void drawPixelFB(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= TFT_WIDTH || y < 0 || y >= TFT_HEIGHT) return;
  framebuffer[y * TFT_WIDTH + x] = color;
}

void drawRectFB(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  int16_t end_x = x + w;
  int16_t end_y = y + h;
  for (int i = x; i < end_x; i++) {
    for (int j = y; j < end_y; j++) {
      drawPixelFB(i, j, color);
    }
  }
}

void fillScreenFB(uint16_t color) {
  drawRectFB(0, 0, TFT_WIDTH, TFT_HEIGHT, color);
}

void pushFramebuffer(Adafruit_HX8357& tft) {
    tft.drawRGBBitmap(0, 0, framebuffer, TFT_WIDTH, TFT_HEIGHT);
}
