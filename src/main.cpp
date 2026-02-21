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

// Flexible pin config
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_MISO 13  // Not used by display, but provided
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST  -1  // Use -1 if reset is not connected

// These are the four touchscreen analog pins
#define YP 4   // must be an analog pin, use "An" notation!
#define XP 5   // must be an analog pin, use "An" notation!
#define YM 6   // can be a digital pin
#define XM 7   // can be a digital pin

// The pin assignments are weird, so this is to correct them so that the wiring can be clean
#define NEW_YP XM
#define NEW_XP YP
#define NEW_YM XP
#define NEW_XM YM

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX -2600
#define TS_MINY -2400
#define TS_MAXX 660
#define TS_MAXY 420

// Create a custom SPI bus
SPIClass spiTFT(FSPI);  // Or VSPI — just avoid overlap with other peripherals

// Pass the custom SPI bus to the display driver
Adafruit_HX8357 tft = Adafruit_HX8357(&spiTFT, TFT_CS, TFT_DC, TFT_RST);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(NEW_XP, NEW_YP, NEW_XM, NEW_YM, 300);

bool updateScreen = true;
enum pages {one, two, three, four};
pages activePage = four;
uint16_t activeTab = static_cast<int>(activePage);

void setup() {
  Serial.begin(115200);
  delay(500);

  spiTFT.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin();

  tft.setRotation(0);
  tft.fillScreen(HX8357_BLACK);
}

void loop() {
  // // for testing
  // static uint64_t i = 0;
  // i += 1;
  // if (i == 10000000) {
  //   if (activePage == one)   activePage = two;
  //   else if (activePage == two)   activePage = three;
  //   else if (activePage == three) activePage = four;
  //   else if (activePage == four)  activePage = one;
  //   updateScreen = true;
  //   i = 0;
  // };

  if (updateScreen) {
    initFramebuffer();

    switch (activePage) {
      case one:
        activeTab = 0;
        drawBG(activeTab);
        break;
      
      case two:
        activeTab = 1;
        drawBG(activeTab);
        break;

      case three:
        activeTab = 2;
        drawBG(activeTab);
        break;

      case four:
        activeTab = 3;
        drawBG(activeTab);
        drawSetup();
        break;
    }

    // Push buffer to screen
    pushFramebuffer(tft);
    updateScreen = false;
  }
}