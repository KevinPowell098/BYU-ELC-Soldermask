#include "touch.h"
#include "TouchScreen.h"

// These are the four touchscreen analog pins
#define YP 16  // must be an analog pin, use "An" notation!
#define XM 17  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 8   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 9
#define TFT_CS 10

Box boxes[MAX_BOXES];
uint16_t boxCount = 0;

void AddBoxToArray(Box b) {
  if (boxCount < MAX_BOXES) {
    boxes[boxCount++] = b;
  } else {
    // TODO: print error
  }
}

void AddBoxToArray(Box b, void (*func)(), int16_t page) {
  if (boxCount < MAX_BOXES) {
    b.operation = func;
    b.page = page;
    boxes[boxCount++] = b;
  } else {
    // TODO: print error
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
      if (boxes[i].page == activeTab || boxes[i].page == -1) {
        boxes[i].operation();
        isElementTouched = true;
      }
    }
  }

  return isElementTouched;
}

bool senseTouch() {
  // Retrieve a point 
  TSPoint p = ts.getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {

    /*
    Serial.print("touch data | pressure: ");
    Serial.print(p.z);
    Serial.print(", x: ");
    Serial.print(p.x);
    Serial.print(", y: ");
    Serial.println(p.y);
    */

    return false;
  }
  
  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, TFT_WIDTH);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, TFT_HEIGHT);

  // Serial.print("touch sensed at: ");
  // Serial.print(p.x);
  // Serial.print(", ");
  // Serial.println(p.y);

  // returns true if a valid element was touched
  // so that the screen will be updated
  return selectElement(p.x, p.y);
}


// // Previous version of touchscreen in main
// #include <SPI.h>
// #include "Adafruit_GFX.h"
// #include "Adafruit_HX8357.h"
// #include "TouchScreen.h"

// #include "lcd/lcd.h"
// #include "fonts/fonts.h"
// #include "menu/menu.h"
// #include "touch/touch.h"

// // Flexible pin config
// #define TFT_MOSI 11
// #define TFT_SCLK 12
// #define TFT_MISO 13  // Not used by display, but provided
// #define TFT_CS   10
// #define TFT_DC    9
// #define TFT_RST  -1  // Use -1 if reset is not connected

// // These are the four touchscreen analog pins
// #define YP 4   // must be an analog pin, use "An" notation!
// #define XP 5   // must be an analog pin, use "An" notation!
// #define YM 6   // can be a digital pin
// #define XM 7   // can be a digital pin

// // The pin assignments are weird, so this is to correct them so that the wiring can be clean
// #define NEW_YP XM
// #define NEW_XP YP
// #define NEW_YM XP
// #define NEW_XM YM

// // This is calibration data for the raw touch data to the screen coordinates
// #define TS_MINX -2600
// #define TS_MINY -2400
// #define TS_MAXX 660
// #define TS_MAXY 420

// // Create a custom SPI bus
// SPIClass spiTFT(FSPI);  // Or VSPI — just avoid overlap with other peripherals

// // Pass the custom SPI bus to the display driver
// Adafruit_HX8357 tft = Adafruit_HX8357(&spiTFT, TFT_CS, TFT_DC, TFT_RST);

// // For better pressure precision, we need to know the resistance
// // between X+ and X- Use any multimeter to read it
// // For the one we're using, its 300 ohms across the X plate
// TouchScreen ts = TouchScreen(NEW_XP, NEW_YP, NEW_XM, NEW_YM, 300);

// bool updateScreen = true;
// enum pages {one, two, three, four};
// pages activePage = four;
// uint16_t activeTab = static_cast<int>(activePage);