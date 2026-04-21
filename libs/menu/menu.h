#pragma once

#include "vars.h"
#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "fonts/font_stan7.h"
#include "fonts/font_stan9.h"
#include "fonts/font_stan12.h"
#include "fonts/font_stan16.h"

#include "page_one/page_one.h"
#include "page_two/page_two.h"
#include "page_three/page_three.h"
#include "page_four/page_four.h"

// Functions to change tabs
void tabOnePressed();
void tabTwoPressed();
void tabThreePressed();
void tabFourPressed();

// Functions to draw menu tabs
void drawBG(uint16_t activeTab);
void drawShadow(uint16_t activeTab);
void drawGradientFill(uint16_t activeTab);
void drawOutline(uint16_t activeTab);
void drawTabLabels(uint16_t activeTab);
void drawTitle(uint16_t activeTab);
