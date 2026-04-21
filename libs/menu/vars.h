#ifndef VARS_H
#define VARS_H

#include <cstdint>

// Page numbering constants
#define PAGE_ONE   0
#define PAGE_TWO   1
#define PAGE_THREE 2
#define PAGE_FOUR  3

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
#define SELECTION_SHADOW_OFFSET  5

// High Temperature Cutoff Range
#define CUTOFF_TEMP_F_MAX  400
#define CUTOFF_TEMP_F_MIN  100
#define CUTOFF_TEMP_F_STEP 10
#define CUTOFF_TEMP_C_STEP 5

// Volume Max
#define SYS_VOLUME_MAX 10

// Current menu tab
extern uint16_t activeTab;

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

// Program variables
extern bool     sysIsTempF;
extern uint16_t sysVolume;
extern int16_t  sysCutoffTempF;
extern int16_t  sysCutoffTempC;

#endif