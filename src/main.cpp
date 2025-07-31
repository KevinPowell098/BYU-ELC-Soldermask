/***************************************************
This is my attempt to get touch and temp going
 ****************************************************/

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "thermister.h"

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
#define TS_MAXY 150

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

//Temp box
#define NUMBOX_X (tft.width() - 50)
#define NUMBOX_Y (tft.height() - 50)
#define NUMBOX_SIZE 50


// Add this at the top, before setup()
unsigned long testText();

void drawNumberBox(float num) {
  // Draw the box
  tft.fillRect(NUMBOX_X, NUMBOX_Y, NUMBOX_SIZE, NUMBOX_SIZE, HX8357_WHITE);
  tft.drawRect(NUMBOX_X, NUMBOX_Y, NUMBOX_SIZE, NUMBOX_SIZE, HX8357_BLACK);

  // Draw the number
  tft.setTextColor(HX8357_BLACK);
  tft.setTextSize(1);
  tft.setCursor(NUMBOX_X + 10, NUMBOX_Y + 15);  // Adjust for center-ish text
  tft.print(num, 2);
}

void setup() {
  Serial.begin(115200);
  delay(5000);

  spiTFT.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin(); // Don't use 'if (!tft.begin())'
  Serial.println("HX8357 init OK");

  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);

  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_MAGENTA);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_BLACK);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
 
  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
  currentcolor = HX8357_RED;
 
  Therm_setup();

  drawNumberBox(get_temperature());
  delay(5000);
  Serial.println("In delay, any box?");
  delay(1000);
}

void loop(){
  // Retrieve a point  
  TSPoint p = ts.getPoint();

  if (p.z == 0 || p.x < -3000) {
    return;
  }

  Serial.print("X: ");
  Serial.print(p.x);
  Serial.print(" Y: ");
  Serial.print(p.y);
  Serial.print(" Pressure: ");
  Serial.println(p.z);  
   
  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  if (p.x > NUMBOX_X && p.y > NUMBOX_Y) {
    drawNumberBox(get_temperature());
  }
  else if (p.y < BOXSIZE) {
     oldcolor = currentcolor;

     if (p.x < BOXSIZE) { 
       currentcolor = HX8357_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*2) {
       currentcolor = HX8357_YELLOW;
       tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*3) {
       currentcolor = HX8357_GREEN;
       tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*4) {
       currentcolor = HX8357_CYAN;
       tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*5) {
       currentcolor = HX8357_BLUE;
       tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*6) {
       currentcolor = HX8357_MAGENTA;
       tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*7) {
       currentcolor = HX8357_WHITE;
       tft.drawRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_RED);
     } else if (p.x < BOXSIZE*8) {
       currentcolor = HX8357_BLACK;
       tft.drawRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     }

     if (oldcolor != currentcolor) {
        if (oldcolor == HX8357_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
        if (oldcolor == HX8357_YELLOW) 
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);
        if (oldcolor == HX8357_GREEN) 
          tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_GREEN);
        if (oldcolor == HX8357_CYAN) 
          tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_CYAN);
        if (oldcolor == HX8357_BLUE) 
          tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_BLUE);
        if (oldcolor == HX8357_MAGENTA) 
          tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_MAGENTA);
        if (oldcolor == HX8357_WHITE) 
          tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
        if (oldcolor == HX8357_BLACK) 
          tft.fillRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, HX8357_BLACK);
     }
  }
  
  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  } else if (((p.x-PENRADIUS) > BOXSIZE*8) && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }
}
