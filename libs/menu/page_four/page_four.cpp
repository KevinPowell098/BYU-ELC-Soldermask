#include "page_four.h"
#include "helpers/helpers.h"
#include "page_four_helpers.h"

static const uint16_t CURRENT_PAGE = PAGE_FOUR;
static const uint16_t FONT_COLOR = font_colors[CURRENT_PAGE];
static const uint16_t SHADOW_COLOR = borders[CURRENT_PAGE];

void drawSetup() {
  drawSetup_TempScaleSelection();
  drawSetup_VolumeSelection();
  drawSetup_TempCutoffSelection();
  drawSetup_ResetButton();
}

void drawSetup_TempScaleSelection() {
  const char* text = "Temperature Scale";
  uint16_t title_y = 55;

  drawWordFB(TEXT_PADDING, title_y + FONT_OFFSET, text, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(TEXT_PADDING, title_y, text, FONT_COLOR, FONT_STAN16);

  // Box constants
  uint16_t box2_x = (TFT_WIDTH + TEXT_PADDING) / 2;
  uint16_t box_y = 85;
  uint16_t box_w = (TFT_WIDTH - (3 * TEXT_PADDING)) / 2;
  uint16_t box_h = 60;
  uint16_t box_r = 10;
  uint16_t line_w = 2;
  uint16_t shadow_x = TEXT_PADDING;

  uint16_t box_grad[] = {g_bg, g_bg};
  uint16_t selection_grad[] = {color565(8,26,3), g_bg};

  uint16_t* c_colors = selection_grad;
  uint16_t* f_colors = box_grad;

  if (sysIsTempF) {
    f_colors = selection_grad;
    c_colors = box_grad;
    shadow_x = box2_x;
  }

  drawRectOutlineFB(shadow_x, box_y + 2, box_w, box_h, box_r, line_w, g_border2);
  
  Box b_scale_f = expandBox(
    drawGradRectFB(TEXT_PADDING, box_y, box_w, box_h, box_r, f_colors, 2), 
    0, -10, 0, 10
  );
  Box b_scale_c = expandBox(
    drawGradRectFB(box2_x, box_y, box_w, box_h, box_r, c_colors, 2), 
    0, -10, 100, 10
  );

  // Add buttons to touch screen sensing
  AddBoxToArray(b_scale_f, setup_TempScaleSelection_SetToF, CURRENT_PAGE);
  AddBoxToArray(b_scale_c, setup_TempScaleSelection_SetToC, CURRENT_PAGE);

  drawRectOutlineFB(TEXT_PADDING, box_y, box_w, box_h, box_r, line_w, g_font);
  drawRectOutlineFB(box2_x, box_y, box_w, box_h, box_r, line_w, g_font);

  const char* text_f = "^F";
  const char* text_c = "^C";
  uint16_t label_offset = 10;
  uint16_t label_f_x = TEXT_PADDING + (box_w / 2) - label_offset;
  uint16_t label_c_x = (TEXT_PADDING * 2) + (box_w * 1.5) - label_offset;
  uint16_t label_y = 107;

  if (sysIsTempF) {
    drawWordFB(label_f_x, label_y + FONT_OFFSET, text_f, SHADOW_COLOR, FONT_STAN16);
  } else {
    drawWordFB(label_c_x, label_y + FONT_OFFSET, text_c, SHADOW_COLOR, FONT_STAN16);
  }

  drawWordFB(label_f_x, label_y, text_f, FONT_COLOR, FONT_STAN16);
  drawWordFB(label_c_x, label_y, text_c, FONT_COLOR, FONT_STAN16);
}

void drawSetup_VolumeSelection() {
  // volume ranges from 0-10

  // Create volume text from variable
  if (sysVolume > SYS_VOLUME_MAX) sysVolume = SYS_VOLUME_MAX;
  char text_v[64];
  uint16_t len = 0;
  const char* append = sysVolume ? "0%%" : "%%";
  len += snprintf(text_v + len, sizeof(text_v) - len, "%d", sysVolume);
  len += snprintf(text_v + len, sizeof(text_v) - len, append);

  const char* text = "System Volume";
  uint16_t title_y = 170;
  uint16_t text_x = TFT_WIDTH - TEXT_PADDING - getWordLength(text_v, FONT_STAN12);
  uint16_t text_y = title_y + 2;
  uint16_t bar_y = 200;
  uint16_t bar_h = 30;
  uint16_t bar_r = 6;
  uint16_t w = 2;
  uint16_t button_pad = 10;
  uint16_t button_r = bar_r;
  uint16_t button_w = 1;
  uint16_t button_c = g_font;
  uint16_t bar_offset = TEXT_PADDING + bar_h + button_pad;
  uint16_t bar_x = bar_offset;
  uint16_t bar_w = TFT_WIDTH - 2 * bar_offset;
  uint16_t button2_x = bar_x + bar_w + button_pad;
  uint16_t volume_w = sysVolume * bar_w / SYS_VOLUME_MAX;

  // colors
  uint16_t bar_color[] = {color565(3,16,3), color565(5,20,5)};
  uint16_t symbol_c[] = {color565(18,40,18), color565(18,40,18)};
  uint16_t max_bg[] = {color565(6,22,1), color565(8,26,3)};
  uint16_t shadow_c[] = {g_border2, g_border2};
  uint16_t volume_color[] = {color565(22,57,22), color565(19,51,19)};
  uint16_t g_bg_grad[] = {g_bg, g_bg};
  uint16_t* minus_bg;
  uint16_t* plus_bg;

  if (!sysVolume) {
    minus_bg = g_bg_grad;
    plus_bg = max_bg;
  } else if (sysVolume == SYS_VOLUME_MAX) {
    minus_bg = max_bg;
    plus_bg = g_bg_grad; 
  } else {
    minus_bg = g_bg_grad;
    plus_bg = g_bg_grad;
  }

  uint16_t symbol_w = 3;
  uint16_t symbol_h = 17;
  uint16_t symbol_r = 1;
  uint16_t shadow_w = 2;
  uint16_t minus_x = 1 + TEXT_PADDING + ((bar_h - symbol_h) / 2);
  uint16_t plus1_x = 1 + button2_x + ((bar_h - symbol_h) / 2);
  uint16_t plus1_y = bar_y + ((bar_h - symbol_w) / 2);
  uint16_t plus2_x = 1 + button2_x + ((bar_h - symbol_w) / 2);
  uint16_t plus2_y = bar_y + ((bar_h - symbol_h) / 2);

  uint16_t shadow_y = bar_y - 1;
  uint16_t shadow_h = bar_h + 1;
  uint16_t shadow_r = button_r + 2;

  // Title
  drawWordFB(TEXT_PADDING, title_y + FONT_OFFSET, text, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(TEXT_PADDING, title_y, text, FONT_COLOR, FONT_STAN16);

  // percentage reading
  drawWordFB(text_x, text_y + FONT_OFFSET, text_v, SHADOW_COLOR, FONT_STAN12);
  drawWordFB(text_x, text_y, text_v, FONT_COLOR, FONT_STAN12);

  // percentage bar
  drawGradRectFB(bar_x, bar_y, bar_w, bar_h, bar_r, bar_color, 2);
  if (sysVolume) {
    // primary volume bar
    drawGradRectFB(bar_x, bar_y, volume_w, bar_h, bar_r, volume_color, 2);

    // modified coordinates for square volume bar overlay
    uint16_t rect_offset = bar_r * 2;
    bar_x += rect_offset;
    volume_w -= rect_offset;

    if (sysVolume >= 6) {
      bar_r = 3;
    } else {
      bar_r = 0;
    }

    if (sysVolume < SYS_VOLUME_MAX) {
      // square volume bar overlay
      drawGradRectFB(bar_x, bar_y, volume_w, bar_h, bar_r, volume_color, 2);
    }
  }

  // volume buttons shadow
  if (sysVolume) {
    drawGradRectFB(
      TEXT_PADDING, shadow_y + SHADOW_OFFSET, 
      shadow_h, shadow_h, 
      shadow_r, shadow_c, 2
    );
  }
  if (sysVolume != SYS_VOLUME_MAX) {
    drawGradRectFB(
      button2_x, shadow_y + SHADOW_OFFSET, 
      shadow_h, shadow_h, 
      shadow_r, shadow_c, 2
    );
  }

  // volume buttons bg
  drawGradRectFB(TEXT_PADDING, shadow_y, bar_h, bar_h, button_r, plus_bg, 2);
  drawGradRectFB(button2_x, shadow_y, bar_h, bar_h, button_r, minus_bg, 2);

  // volume buttons outline
  Box b_volume_minus = expandBox(
    drawRectOutlineFB(TEXT_PADDING, shadow_y, bar_h, bar_h, button_r, button_w, button_c),
    -20, 0, 10, 10
  );
  Box b_volume_plus  = expandBox(
    drawRectOutlineFB(button2_x, shadow_y, bar_h, bar_h, button_r, button_w, button_c),
    -20, 0, 80, 10
  );

  // add to touch sensing
  AddBoxToArray(b_volume_minus, setup_VolumeSelection_Minus, CURRENT_PAGE);
  AddBoxToArray(b_volume_plus, setup_VolumeSelection_Plus, CURRENT_PAGE);

  // minus symbol
  drawGradRectFB(minus_x, plus1_y - 1, symbol_h, symbol_w, symbol_r, symbol_c, 2);

  // plus symbol
  drawGradRectFB(plus1_x, plus1_y - 1, symbol_h, symbol_w, symbol_r, symbol_c, 2);
  drawGradRectFB(plus2_x, plus2_y - 1, symbol_w, symbol_h, symbol_r, symbol_c, 2);
}

void drawSetup_TempCutoffSelection() {
  const char* text = "Temperature Cutoff";
  uint16_t title_y = 252;
  uint16_t p_y = 280;
  uint16_t p_h = 36;
  uint16_t p_w = p_h;
  uint16_t p_r = 4;
  uint16_t p_p = 20;
  uint16_t p_col[] = {color565(22,57,22), color565(19,51,19)};
  uint16_t p_grey[] = {color565(18,49,18), color565(15,43,15)};
  uint16_t shadow_c[] = {g_border2, g_border2};

  uint16_t r_p = 25;
  uint16_t r_r = 3;
  uint16_t r_o = 2;
  uint16_t r_color[] = {color565(5,24,5), color565(7,28,7)};

  // Create temp text from variable
  char w_text[64];
  uint16_t len = 0;
  const char* append = sysIsTempF ? " ^F" : " ^C";
  int16_t temp = sysIsTempF ? sysCutoffTempF : sysCutoffTempC;
  len += snprintf(w_text + len, sizeof(w_text) - len, "%d", temp);
  len += snprintf(w_text + len, sizeof(w_text) - len, append);

  uint16_t w_x = (TFT_WIDTH - getWordLength(w_text, FONT_STAN16))/2;
  uint16_t w_o = 2;

  drawWordFB(TEXT_PADDING, title_y + FONT_OFFSET, text, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(TEXT_PADDING, title_y, text, FONT_COLOR, FONT_STAN16);

  // Test if temp is at max values for animation
  bool isMin = (sysIsTempF) ? sysCutoffTempF <= CUTOFF_TEMP_F_MIN : sysCutoffTempC <= CUTOFF_TEMP_C_MIN;
  bool isMax = (sysIsTempF) ? sysCutoffTempF >= CUTOFF_TEMP_F_MAX : sysCutoffTempC >= CUTOFF_TEMP_C_MAX;

  // left arrow shadow
  if (!isMin) {
    drawThreePointTriangleFB(
      TEXT_PADDING + p_p, p_y + p_h/2 + SHADOW_OFFSET, 
      TEXT_PADDING + p_p + p_w, p_y + SHADOW_OFFSET, 
      TEXT_PADDING + p_p + p_w, p_y + p_h + SHADOW_OFFSET, 
      p_r, shadow_c, 2
    );
  }

  // right arrow shadow
  if (!isMax) {
    drawThreePointTriangleFB(
      TFT_WIDTH - TEXT_PADDING - p_p, p_y + p_h/2 + SHADOW_OFFSET, 
      TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + SHADOW_OFFSET, 
      TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + p_h + SHADOW_OFFSET, 
      p_r, shadow_c, 2
    );
  }

  // left arrow
  Box b_cutoff_minus = expandBox(
    drawThreePointTriangleFB(
      TEXT_PADDING + p_p, p_y + p_h/2, 
      TEXT_PADDING + p_p + p_w, p_y, 
      TEXT_PADDING + p_p + p_w, p_y + p_h, 
      p_r, 
      (!isMin) ? p_col : p_grey, 2
    ),
    -40, 20, 20, 10
  );

  // right arrow
  Box b_cutoff_plus = expandBox(
    drawThreePointTriangleFB(
      TFT_WIDTH - TEXT_PADDING - p_p, p_y + p_h/2, 
      TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y, 
      TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + p_h, 
      p_r, 
      (!isMax) ? p_col : p_grey, 2
    ), 
    -20, 20, 80, 20
  );

  AddBoxToArray(b_cutoff_minus, setup_TempCutoffSelection_Minus, CURRENT_PAGE);
  AddBoxToArray(b_cutoff_plus, setup_TempCutoffSelection_Plus, CURRENT_PAGE);

  // background rectangle
  drawGradRectFB(
    TEXT_PADDING + p_p + p_w + r_p, p_y + r_o,
    TFT_WIDTH - 2 * (TEXT_PADDING + p_p + p_w + r_p), p_h - 2 * r_o,
    r_r, r_color, 2
  );

  // temperature text shadow
  drawWordFB(
    w_x, p_y + p_h/2 - FONT_STAN16.HEIGHT/2 + w_o, 
    w_text, SHADOW_COLOR, FONT_STAN16
  );

  // temperature text
  drawWordFB(
    w_x, p_y + p_h/2 - FONT_STAN16.HEIGHT/2, 
    w_text, FONT_COLOR, FONT_STAN16
  );
}

void drawSetup_ResetConfirmation() {
  // If confirmed, set all values to defaults
}

void drawSetup_ResetButton() {
  // If clicked call drawResetConfirmation()
}
