#ifndef VARS_H
#define VARS_H

#include <cstdint>


/***** Rendering Constants *****/

// Page numbering constants
#define PAGE_ONE   0
#define PAGE_TWO   1
#define PAGE_THREE 2
#define PAGE_FOUR  3

#define PAGE_ONE_PROCESS 4
#define PAGE_TWO_PROCESS 5

// Page layout constants
#define SELECTION_H       45
#define SELECTION_R       10
#define GLOW_OFFSET       1
#define BORDER_OFFSET     1
#define OPTION_H          30
#define OPTION_R          7
#define OPTION_WEIGHT     3
#define TAB_SHADOW_OFFSET 10
#define SHADOW_R          12
#define SHADOW_W          6
#define FONT_OFFSET       1
#define TEXT_PADDING      12
#define SHADOW_OFFSET     2
#define MARGIN_X          12
#define SELECTION_SHADOW_OFFSET  5

#define CURSOR_Y_DEFAULT 0


/***** Setup Page Variables *****/

// High Temperature Cutoff Range
#define CUTOFF_TEMP_F_MAX  400
#define CUTOFF_TEMP_F_MIN  100
#define CUTOFF_TEMP_F_STEP 10
#define CUTOFF_TEMP_C_STEP 5

// Volume Max
#define SYS_VOLUME_MAX 10


/***** Bake Page Variables *****/

// Bake Temperature Range
#define BAKE_TEMP_F_MAX  200
#define BAKE_TEMP_F_MIN  100
#define BAKE_TEMP_F_STEP 10
#define BAKE_TEMP_C_STEP 5

// Bake Time Range in Seconds
#define BAKE_TIME_S_MAX  600
#define BAKE_TIME_S_MIN  60
#define BAKE_TIME_S_STEP 30


/***** UV Cure Page Variables *****/

// Cure Time Range in Seconds
#define CURE_TIME_S_MAX  600
#define CURE_TIME_S_MIN  60
#define CURE_TIME_S_STEP 30

// Cure Power Range in Percent
#define CURE_POWER_MAX  100
#define CURE_POWER_MIN  10
#define CURE_POWER_STEP 10


/***** Control Flow Variables *****/

// Used for initialization of touch zones
extern bool isInit;

// Current menu tab
extern uint16_t activePage;

// Current y position
extern uint16_t cursor_y;


/***** Color Constants *****/

// Tab outline colors
extern uint16_t r_outline;
extern uint16_t y_outline;
extern uint16_t b_outline;
extern uint16_t g_outline;
extern uint16_t outlines[4];

extern uint16_t r_glow;
extern uint16_t y_glow;
extern uint16_t b_glow;
extern uint16_t g_glow;
extern uint16_t glows[4];

// Tab outline dark border
extern uint16_t r_border;
extern uint16_t y_border;
extern uint16_t b_border;
extern uint16_t g_border;
extern uint16_t borders[4];

// Bg colors
extern uint16_t r_bg;
extern uint16_t y_bg;
extern uint16_t b_bg;
extern uint16_t g_bg;
extern uint16_t bgs[4];

// Default gradient colors
extern uint16_t r_grad[2];
extern uint16_t y_grad[2];
extern uint16_t b_grad[2];
extern uint16_t g_grad[2];

// Selection gradient colors
extern uint16_t r_sel_grad[2];
extern uint16_t y_sel_grad[2];
extern uint16_t b_sel_grad[2];
extern uint16_t g_sel_grad[2];
extern uint16_t* selection_grads[4];

// Tab shadow colors
extern uint16_t r_shadow[2];
extern uint16_t y_shadow[2];
extern uint16_t b_shadow[2];
extern uint16_t g_shadow[2];
extern uint16_t* shadows[4];

// Font colors
extern uint16_t r_font;
extern uint16_t y_font;
extern uint16_t b_font;
extern uint16_t g_font;
extern uint16_t font_colors[4];

// Font drop shadow
extern uint16_t r_border2;
extern uint16_t y_border2;
extern uint16_t b_border2;
extern uint16_t g_border2;


/***** Program Variables *****/

// Setup page variables
extern bool     sysIsTempF;
extern uint16_t sysVolume;
extern uint16_t sysCutoffTempF;
extern uint16_t sysCutoffTempC;
extern uint16_t CUTOFF_TEMP_C_MAX;
extern uint16_t CUTOFF_TEMP_C_MIN;

// Bake page variables
extern uint16_t sysBakeTempF;
extern uint16_t sysBakeTempC;
extern uint16_t sysBakeTimeS;
extern uint16_t BAKE_TEMP_C_MAX;
extern uint16_t BAKE_TEMP_C_MIN;

// UV page variables
extern uint16_t sysCureTimeS;
extern uint16_t sysCurePower;

#endif