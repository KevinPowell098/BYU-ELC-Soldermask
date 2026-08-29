#include "touch.h"
#include "TouchScreen.h"
#include "lcd/lcd.h"
#include "menu/vars.h"

// Touch sensing pins
#define YP 7   // must be an analog pin, use "An" notation!
#define XM 4   // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 6   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX -2500
#define TS_MINY -2500
#define TS_MAXX -500
#define TS_MAXY 660

// Gating to reject noise
#define MIN_X -2800

// The display uses hardware SPI, plus #9 & #10
#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 9
#define TFT_CS 10


Box boxes[MAX_BOXES];
uint16_t boxCount = 0;

void AddBoxToArray(Box& b) {
  if (isInit) {
    if (boxCount < MAX_BOXES) {
      boxes[boxCount++] = b;
    } else {
      Serial.println("WARNING: array 'boxes' is full, additional elements cannot be added");
    }
  }
}

void AddBoxToArray(Box& b, void (*func)(), int16_t page) {
  if (isInit) {
    if (boxCount < MAX_BOXES) {
      b.operation = func;
      b.page = page;
      boxes[boxCount++] = b;
    } else {
      Serial.println("WARNING: array 'boxes' is full, additional elements cannot be added");
    }
  }
}

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

bool selectElement(int16_t x, int16_t y) {
  bool isElementTouched = false;
  for (uint16_t i = 0; i < boxCount; i++) {
    if (boxes[i]._contains(x, y)) {
      // TODO - add check that current page matches
      if (boxes[i].page == activePage || boxes[i].page == -1) {
        boxes[i].operation();
        isElementTouched = true;
      }
    }
  }

  return isElementTouched;
}

bool isElementTouched() {
  // Retrieve a point 
  TSPoint p = ts.getPoint();

  // To be considered a valid press, it must:
  // 1. have a non-zero pressure
  // 2. be within reasonable x bounds
  // False touches usually have p.x of -3070 or -3072
  if (p.z && p.x > MIN_X) {
    // Scale from arbitrary range to tft.width using the calibration #'s
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, TFT_WIDTH);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, TFT_HEIGHT);

    return selectElement(p.x, p.y);
  }

  return false;
}
