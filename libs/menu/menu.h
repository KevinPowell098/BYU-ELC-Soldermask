#include "lcd.h"
#include "fonts.h"
#include "font_stan7.h"
#include "font_stan9.h"
#include "font_stan12.h"
#include "font_stan16.h"

void drawBG(uint16_t activeTab);
void drawShadow(uint16_t activeTab);
void drawGradientFill(uint16_t activeTab);
void drawOutline(uint16_t activeTab);
void drawTabLabels(uint16_t activeTab);
void drawTitle(uint16_t activeTab);

void drawSetup();
void drawTempScaleSelection();
void drawVolumeSelection();