#include "page_one_run.h"
#include "page_one_helpers.h"

#include "../vars.h"
#include "lcd/lcd.h"
#include "fonts/fonts.h"
#include "fonts/font_stan7.h"
#include "fonts/font_stan9.h"
#include "fonts/font_stan12.h"
#include "fonts/font_stan16.h"
#include "touch/touch.h"

static const uint16_t CURRENT_PAGE = PAGE_ONE_PROCESS;


void drawHeatProcess() {
  cursor_y = CURSOR_Y_DEFAULT;

  drawHeatProcess_BG();
  drawHeatProcess_Cancel();
}

void drawHeatProcess_BG() {
  fillScreenFB(bgs[PAGE_ONE]);

  // Title constants
  uint16_t title_x = TEXT_PADDING;
  uint16_t title_y = 15;

  // Line constants
  uint16_t line_y = 40;
  uint16_t shadow_y = line_y + FONT_OFFSET;
  uint16_t line_border = TEXT_PADDING;
  uint16_t line_weight = 1;

  const char* text = "Bake Process";

  drawWordFB(title_x, title_y + FONT_OFFSET, text, r_border, FONT_STAN16);
  drawWordFB(title_x, title_y, text, r_font, FONT_STAN16);

  drawLineFB(line_border, shadow_y, TFT_WIDTH - (line_border * 2), 0, line_weight, r_border);
  drawLineFB(line_border, line_y, TFT_WIDTH - (line_border * 2), 0, line_weight, r_font);

  cursor_y = line_y;
}

void drawHeatProcess_Cancel() {
  // Box constants
  uint16_t padding = 30;
  uint16_t box_x = 2 * TEXT_PADDING;
  uint16_t box_w = (TFT_WIDTH - (2 * box_x)) - 1;
  uint16_t box_h = 80;
  uint16_t box_r = 10;
  uint16_t line_w = 2;
  uint16_t shadow_x = box_x;

  cursor_y = TFT_HEIGHT - padding - box_h - SHADOW_OFFSET;

  uint16_t box_grad[] = {r_bg, r_bg};
  uint16_t* colors = box_grad;

  drawRectOutlineFB(shadow_x, cursor_y + 2, box_w, box_h, box_r, line_w, r_border2);
  
  Box b_scale_f = expandBox(
    drawGradRectFB(box_x, cursor_y, box_w, box_h, box_r, colors, 2), 
    0, 0, 0, 0
  );

  // Add buttons to touch screen sensing
  AddBoxToArray(b_scale_f, bake_ProcessCancel_Press, CURRENT_PAGE);

  drawRectOutlineFB(box_x, cursor_y, box_w, box_h, box_r, line_w, r_font);

  const char* text = "Cancel";
  uint16_t label_x = (TFT_WIDTH - getWordLength(text, FONT_STAN16)) / 2;
  uint16_t label_h = FONT_STAN16.HEIGHT;
  uint16_t label_y = cursor_y + (box_h - label_h) / 2;

  drawWordFB(label_x, label_y + FONT_OFFSET, text, r_border2, FONT_STAN16);
  drawWordFB(label_x, label_y, text, r_font, FONT_STAN16);

  cursor_y += box_h;
}

/*
New screen
- target temp
- current temp
- time progress

Bake finished
- title: "Bake finished - Cooling"
- Cancel button -> "Return to Menu"
*/