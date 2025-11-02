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

#include "lcd.h"
#include "fonts.h"
#include "font_stan7.h"
#include "font_stan9.h"
#include "font_stan12.h"
#include "font_stan16.h"

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

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup() {
  Serial.println("DEBUG - Entering setup()");

  Serial.begin(115200);
  delay(500);

  spiTFT.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin(); // Don't use 'if (!tft.begin())'
  Serial.println("HX8357 init OK");

  tft.setRotation(0);
  tft.fillScreen(HX8357_BLACK);

  Serial.println("DEBUG - Leaving setup()");
}

void loop()
{
  static int x = 0;

  initFramebuffer();
  fillScreenFB(0x000);

  x = (x + 5) % 200;

  // Draw a red pixel into the buffer
  drawRectFB(x, 0, BOXSIZE, BOXSIZE, 0x5AFF);

  drawWordFB(10, 50, "ABC", 0xFFFF, FONT_STAN16);
  drawWordFB(10, 80, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 0xFFFF, FONT_STAN12);
  drawWordFB(10, 130, "Hi there, Kevin!", 0xFFFF, FONT_STAN9);
  drawWordFB(10, 150, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 0xFFFF, FONT_STAN7);

  // Push buffer to screen
  pushFramebuffer(tft);
}
