#pragma once

#include "vars.h"
#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "fonts/font_stan7.h"
#include "fonts/font_stan9.h"
#include "fonts/font_stan12.h"
#include "fonts/font_stan16.h"

#include "page_one.h"
#include "page_two.h"
#include "page_three.h"
#include "page_four.h"

void drawBG(uint16_t activeTab);
void drawShadow(uint16_t activeTab);
void drawGradientFill(uint16_t activeTab);
void drawOutline(uint16_t activeTab);
void drawTabLabels(uint16_t activeTab);
void drawTitle(uint16_t activeTab);
