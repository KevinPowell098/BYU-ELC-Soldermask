#pragma once

#include "../vars.h"
#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "fonts/font_stan7.h"
#include "fonts/font_stan9.h"
#include "fonts/font_stan12.h"
#include "fonts/font_stan16.h"
#include "touch/touch.h"

#define CURRENT_PAGE PAGE_FOUR

void drawSetup();
void drawTempScaleSelection();
void drawVolumeSelection();
void drawTempCutoffSelection();
void drawResetButton();
void drawResetConfirmation();