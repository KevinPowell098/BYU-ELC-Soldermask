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
#include <Adafruit_MAX31856.h>

#include "TouchScreen.h"

#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "menu/menu.h"
#include "touch/touch.h"
#include "menu/vars.h"

#include "menu/page_one/page_one.h"
#include "menu/page_two/page_two.h"
#include "menu/page_three/page_three.h"
#include "menu/page_four/page_four.h"

#include "menu/page_one/page_one_run.h"
#include "menu/page_two/page_two_run.h"

// TODO: remove this, for testing without tc only
#include <math.h>

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

// Pin config for MAX31856 temperature sensor
#define TEMP_CS  -1
#define TEMP_SDI -1
#define TEMP_SDO -1
#define TEMP_SCK -1

// Temperature board 'data ready' pin
#define DRDY_PIN 5


// Create a custom SPI bus
SPIClass spiTFT(FSPI);  // Or VSPI — just avoid overlap with other peripherals

// Pass the custom SPI bus to the display driver
Adafruit_HX8357 tft = Adafruit_HX8357(&spiTFT, TFT_CS, TFT_DC, TFT_RST);

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(TEMP_CS, TEMP_SDI, TEMP_SDO, TEMP_SCK);

// Used to poll MAX31856 only when DRDY pin has first gone low
bool tempIsReady = false;
bool tempWasReady = false;

// Track if control flow recently switched to a process to start timer
bool wasProcessRunning = false;
uint32_t startTimeMillis = 0;
uint32_t timeBufferMillis = 0;
uint32_t processTimeMillis = 0;
uint16_t timeRemainder = 0;

// Becomes true when an element is pressed
bool updateScreen = true;

void setup() {
  Serial.begin(115200);
  delay(500);

  spiTFT.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin();
  tft.setRotation(0);

  initFramebuffer();
  initMenu();

  // pin to sense if temp data is ready
  pinMode(DRDY_PIN, INPUT);

  // Initiate thermocouple
  if (!thermocouple.begin()) {
    Serial.println("Could not initialize thermocouple.");
    while (1) delay(10);
  }

  // Set thermocouple mode
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_K);
  thermocouple.setConversionMode(MAX31856_CONTINUOUS);
}

void loop() {
  digitalRead(DRDY_PIN);

  // New reading is available when DRDY pin is low
  tempWasReady = tempIsReady;
  tempIsReady = !digitalRead(DRDY_PIN);
  if (!tempWasReady && tempIsReady) {
    sysTempInC = thermocouple.readThermocoupleTemperature();
  }

  if (updateScreen) {
    initFramebuffer();

    switch (activePage) {
      case PAGE_ONE:
        drawMenu();
        drawHeat();
        break;

      case PAGE_TWO:
        drawMenu();
        drawUV();
        break;

      case PAGE_THREE:
        drawMenu();
        break;

      case PAGE_FOUR:
        drawMenu();
        drawSetup();
        break;

      case PAGE_ONE_PROCESS:
        drawHeatProcess();
        break;

      case PAGE_TWO_PROCESS:
        drawUVProcess();
        break;
    }

    // Push buffer to screen
    pushFramebuffer(tft);
    updateScreen = false;
  }

  if (isProcessActive()) {
    if (!wasProcessRunning) {
      startTimeMillis = millis();
      timeRemainder = startTimeMillis % 1000;

      // Update to avoid timer reset
      wasProcessRunning = true;
    }

    timeBufferMillis = (millis() - startTimeMillis);

    // Update screen once every second
    if (timeBufferMillis >= processTimeMillis + 1000) {
      updateScreen = true;
      processTimeMillis = timeBufferMillis;
      processTimeInS = (timeBufferMillis - timeRemainder) / 1000 + 1;

      // TODO: remove this, for testing without tc only
      sysTempInC = 10 * sin(processTimeInS / 10.0) + 40;
      // sysTempInC = 40;
      drawHeatProcess_addPoint();
    }
  } else {
    if (wasProcessRunning) {
      timeBufferMillis = 0;
      processTimeMillis = 0;
      processTimeInS = 0;

      // Allow timer reset
      wasProcessRunning = false;
    }
  }

  // test for touch input every loop, 
  // redraw screen if touch detected inside object
  updateScreen = updateScreen || isElementTouched();
}
