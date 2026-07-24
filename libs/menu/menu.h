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
bool isProcessActive();
void tabOnePressed();
void tabTwoPressed();
void tabThreePressed();
void tabFourPressed();

// Create touch sensitive regions
void initTabBoxes();

// Functions to draw menu tabs
void drawMenu();
void drawMenu_Shadow();
void drawMenu_GradientFill();
void drawMenu_Outline();
void drawMenu_TabLabels();
void drawMenu_Title();
