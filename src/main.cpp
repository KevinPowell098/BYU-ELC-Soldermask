/***************************************************
  This is our library for the Adafruit HX8357D Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

#include "TouchScreen.h"

#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "menu/menu.h"
#include "touch/touch.h"
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

// Touch sensing gating variables to reject noise
#define MIN_X -2800

// Flexible pin config
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_MISO 13  // Not used by display, but provided
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST  -1  // Use -1 if reset is not connected

// Create a custom SPI bus
SPIClass spiTFT(FSPI);  // Or VSPI — just avoid overlap with other peripherals

// Pass the custom SPI bus to the display driver
Adafruit_HX8357 tft = Adafruit_HX8357(&spiTFT, TFT_CS, TFT_DC, TFT_RST);

bool updateScreen = true;


void setup() {
  Serial.begin(115200);
  delay(500);

  spiTFT.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin();
  tft.setRotation(0);

  initFramebuffer();
  initMenu();
}

void loop() {
  // // for testing
  // static uint64_t i = 0;
  // i += 1;
  // if (i == 10000000) {
  //   if (activeTab == PAGE_ONE)   activeTab = PAGE_TWO;
  //   else if (activeTab == PAGE_TWO)   activeTab = PAGE_THREE;
  //   else if (activeTab == PAGE_THREE) activeTab = PAGE_FOUR;
  //   else if (activeTab == PAGE_FOUR)  activeTab = PAGE_ONE;
  //   updateScreen = true;
  //   i = 0;
  // };

  if (updateScreen) {
    initFramebuffer();

    switch (activeTab) {
      case PAGE_ONE:
        drawBG();
        break;
      
      case PAGE_TWO:
        drawBG();
        break;

      case PAGE_THREE:
        drawBG();
        break;

      case PAGE_FOUR:
        drawBG();
        drawSetup();
        break;
    }

    // Push buffer to screen
    pushFramebuffer(tft);
    updateScreen = false;
  }

  // test for touch input every loop, 
  // redraw screen if touch detected inside object
  updateScreen = isElementTouched();
}

