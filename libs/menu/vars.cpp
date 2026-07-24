#include "vars.h"
#include "lcd/lcd.h"
#include "helpers/helpers.h"


/***** Control Flow Variables *****/

// Used for initialization of touch zones
bool isInit = true;

// Current menu tab variables
uint16_t activePage = PAGE_TWO;

// Current y position
uint16_t cursor_y;


/***** Color Constants *****/

// Tab outline colors
uint16_t r_outline = color565(31, 0, 0);
uint16_t y_outline = color565(30, 50, 0);
uint16_t b_outline = color565(0, 0, 31);
uint16_t g_outline = color565(0, 40, 0);
uint16_t outlines[4] = {
  r_outline, 
  y_outline, 
  b_outline, 
  g_outline
};

uint16_t r_glow = color565(31, 30, 15);
uint16_t y_glow = color565(31, 60, 15);
uint16_t b_glow = color565(20, 40, 20);
uint16_t g_glow = color565(15, 50, 15);
uint16_t glows[4] = {r_glow, y_glow, b_glow, g_glow};

// Tab outline dark border
uint16_t r_border = color565(15, 0, 0);
uint16_t y_border = color565(15, 25, 0);
uint16_t b_border = color565(0, 0, 15);
uint16_t g_border = color565(0, 20, 0);
uint16_t borders[4] = {r_border, y_border, b_border, g_border};

// Bg colors
uint16_t r_bg = color565(25,15,5);
uint16_t y_bg = color565(24,36,5);
uint16_t b_bg = color565(5,15,15);
uint16_t g_bg = color565(10,30,5);
uint16_t bgs[4] = {r_bg, y_bg, b_bg, g_bg};

// Default gradient colors
uint16_t r_grad[2] = {color565(31, 0, 0), color565(20, 10, 0)};
uint16_t y_grad[2] = {color565(30, 50, 0), color565(25, 40, 0)};
uint16_t b_grad[2] = {color565(0, 0, 31), color565(5, 0, 20)};
uint16_t g_grad[2] = {color565(0, 40, 0), color565(0, 30, 5)};

// Selection gradient colors
uint16_t r_sel_grad[2] = {color565(31, 0, 0), color565(25, 10, 0)};
uint16_t y_sel_grad[2] = {color565(30, 50, 0), color565(25, 40, 0)};
uint16_t b_sel_grad[2] = {color565(0, 0, 31), color565(5, 0, 20)};
uint16_t g_sel_grad[2] = {color565(0, 40, 0), color565(0, 30, 5)};
uint16_t* selection_grads[4] = {
  r_sel_grad, 
  y_sel_grad, 
  b_sel_grad, 
  g_sel_grad
};

// Tab shadow colors
uint16_t r_shadow[2] = {color565(25,15,5), color565(15,5,10)};
uint16_t y_shadow[2] = {color565(24,36,5), color565(12,18,2)};
uint16_t b_shadow[2] = {color565(5,15,15), color565(0,0,5)};
uint16_t g_shadow[2] = {color565(10,30,5), color565(0,15,0)};
uint16_t* shadows[4] = {r_shadow, y_shadow, b_shadow, g_shadow};

// Font colors
uint16_t r_font = color565(31,54,27);
uint16_t y_font = color565(31,63,25);
uint16_t b_font = color565(27,54,31);
uint16_t g_font = color565(25,63,25);
uint16_t font_colors[4] = {r_font, y_font, b_font, g_font};

// Font drop shadow
uint16_t r_border2 = color565(15, 10, 10);
uint16_t y_border2 = color565(15, 25, 4);
uint16_t b_border2 = color565(10, 10, 15);
uint16_t g_border2 = color565(6, 20, 6);


/***** Program Variables *****/

// Setup page variables
bool     sysIsTempF = false;
uint16_t sysVolume = 9;
uint16_t sysCutoffTempF = 300;
uint16_t sysCutoffTempC = roundToNearest(getTempCFromF(sysCutoffTempF), CUTOFF_TEMP_C_STEP);
uint16_t CUTOFF_TEMP_C_MAX = roundToNearest(getTempCFromF(CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_C_STEP);
uint16_t CUTOFF_TEMP_C_MIN = roundToNearest(getTempCFromF(CUTOFF_TEMP_F_MIN), CUTOFF_TEMP_C_STEP);

// Bake page variables
uint16_t sysBakeTempF = 100;
uint16_t sysBakeTempC = roundToNearest(getTempCFromF(sysBakeTempF), BAKE_TEMP_C_STEP);
uint16_t sysBakeTimeS = 120;
uint16_t BAKE_TEMP_C_MAX = roundToNearest(getTempCFromF(BAKE_TEMP_F_MAX), BAKE_TEMP_C_STEP);
uint16_t BAKE_TEMP_C_MIN = roundToNearest(getTempCFromF(BAKE_TEMP_F_MIN), BAKE_TEMP_C_STEP);

// UV page variables
uint16_t sysCureTimeS = 120;
uint16_t sysCurePower = 100;