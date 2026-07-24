#include "page_two.h"
#include "helpers/helpers.h"

static const uint16_t CURRENT_PAGE = PAGE_TWO;
static const uint16_t FONT_COLOR = font_colors[CURRENT_PAGE];
static const uint16_t SHADOW_COLOR = borders[CURRENT_PAGE];
static const uint16_t BG_COLOR = bgs[CURRENT_PAGE];

void drawUV() {
  cursor_y = CURSOR_Y_DEFAULT;

  drawUV_TimeSetting();
  drawUV_PowerSetting();
  drawUV_StartButton();
}

void drawUV_TimeSetting() {
  // const char* title = "Bake Time (Seconds)";
  uint16_t title_padding = 55;
  cursor_y += title_padding;

  const char* title = "Cure Time";
  uint16_t shadow_y = cursor_y + FONT_OFFSET;
  uint16_t title_h = 30;

  drawWordFB(MARGIN_X, shadow_y, title, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(MARGIN_X, cursor_y, title, FONT_COLOR, FONT_STAN16);

  cursor_y += title_h;

  // pointer variables
  uint16_t p_y = cursor_y;
  uint16_t p_h = 36;
  uint16_t p_w = p_h;
  uint16_t p_r = 4;
  uint16_t p_p = 20;
  uint16_t p_col[] = {color565(30,58,22), color565(24,44,18)};
  uint16_t p_grey[] = {color565(28,50,16), color565(24,42,14)};
  uint16_t shadow_c[] = {y_border2, y_border2};

  // rectangle variables
  uint16_t r_p = 25;
  uint16_t r_r = 3;
  uint16_t r_o = 2;
  uint16_t r_color[] = {color565(16,20,0), color565(20,28,2)};

  // Create temp text from variable
  char w_text[64];
  uint16_t len = 0;
  uint16_t min = sysCureTimeS / 60;
  uint16_t sec = sysCureTimeS % 60;
  len += snprintf(w_text + len, sizeof(w_text) - len, "%d", min);
  len += snprintf(w_text + len, sizeof(w_text) - len, ":");
  // Format seconds place
  if (sec) {
    len += snprintf(w_text + len, sizeof(w_text) - len, "%d", sec); 
  } else {
    len += snprintf(w_text + len, sizeof(w_text) - len, "00"); 
  }

  uint16_t w_x = (TFT_WIDTH - getWordLength(w_text, FONT_STAN16))/2;
  uint16_t w_o = 2;

  // Test if temp is at max values for animation
  bool isMin = sysCureTimeS <= CURE_TIME_S_MIN;
  bool isMax = sysCureTimeS >= CURE_TIME_S_MAX;

  // left arrow shadow if not at min value
  if (!isMin) {
    drawThreePointTriangleFB(
      TEXT_PADDING + p_p, p_y + p_h/2 + SHADOW_OFFSET, 
      TEXT_PADDING + p_p + p_w, p_y + SHADOW_OFFSET, 
      TEXT_PADDING + p_p + p_w, p_y + p_h + SHADOW_OFFSET, 
      p_r, shadow_c, 2
    );
  }

  // right arrow shadow if not at max value
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

  if (isInit) {
    AddBoxToArray(b_cutoff_minus, UV_TimeSetting_Minus, CURRENT_PAGE);
    AddBoxToArray(b_cutoff_plus, UV_TimeSetting_Plus, CURRENT_PAGE);
  }

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

// void drawUV_PowerSetting() {
//   uint16_t offset_h = 55;
//   uint16_t text_h = 30;
//   cursor_y += offset_h;

//   const char* title = "Power Set";
//   uint16_t shadow_y = cursor_y + FONT_OFFSET;

//   drawWordFB(MARGIN_X, shadow_y, title, SHADOW_COLOR, FONT_STAN16);
//   drawWordFB(MARGIN_X, cursor_y, title, FONT_COLOR, FONT_STAN16);

//   cursor_y += text_h;

//   // Test if temp is at max values for animation
//   bool isMin = sysCurePower <= CURE_POWER_MIN;
//   bool isMax = sysCurePower >= CURE_POWER_MAX;

//   // pointer variables
//   uint16_t p_y = cursor_y;
//   uint16_t p_h = 36;
//   uint16_t p_w = p_h;
//   uint16_t p_r = 4;
//   uint16_t p_p = 20;
//   uint16_t p_col[] = {color565(30,58,22), color565(24,44,18)};
//   uint16_t p_grey[] = {color565(28,50,16), color565(24,42,14)};
//   uint16_t shadow_c[] = {y_border2, y_border2};

//   cursor_y += p_h;

//   // rectangle variables
//   uint16_t r_p = 25;
//   uint16_t r_r = 3;
//   uint16_t r_o = 2;
//   uint16_t r_color[] = {color565(16,20,0), color565(20,28,2)};

//   // Create temp text from variable
//   char w_text[64];
//   uint16_t len = 0;
//   int16_t power = sysCurePower;
//   len += snprintf(w_text + len, sizeof(w_text) - len, "%d", power);

//   uint16_t w_x = (TFT_WIDTH - getWordLength(w_text, FONT_STAN16))/2;
//   uint16_t w_o = 2;

//   // left arrow shadow
//   if (!isMin) {
//     drawThreePointTriangleFB(
//       TEXT_PADDING + p_p, p_y + p_h/2 + SHADOW_OFFSET, 
//       TEXT_PADDING + p_p + p_w, p_y + SHADOW_OFFSET, 
//       TEXT_PADDING + p_p + p_w, p_y + p_h + SHADOW_OFFSET, 
//       p_r, shadow_c, 2
//     );
//   }

//   // right arrow shadow
//   if (!isMax) {
//     drawThreePointTriangleFB(
//       TFT_WIDTH - TEXT_PADDING - p_p, p_y + p_h/2 + SHADOW_OFFSET, 
//       TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + SHADOW_OFFSET, 
//       TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + p_h + SHADOW_OFFSET, 
//       p_r, shadow_c, 2
//     );
//   }

//   // left arrow
//   Box b_cutoff_minus = expandBox(
//     drawThreePointTriangleFB(
//       TEXT_PADDING + p_p, p_y + p_h/2, 
//       TEXT_PADDING + p_p + p_w, p_y, 
//       TEXT_PADDING + p_p + p_w, p_y + p_h, 
//       p_r, 
//       (!isMin) ? p_col : p_grey, 2
//     ),
//     -40, 20, 20, 10
//   );

//   // right arrow
//   Box b_cutoff_plus = expandBox(
//     drawThreePointTriangleFB(
//       TFT_WIDTH - TEXT_PADDING - p_p, p_y + p_h/2, 
//       TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y, 
//       TFT_WIDTH - TEXT_PADDING - p_p - p_w, p_y + p_h, 
//       p_r, 
//       (!isMax) ? p_col : p_grey, 2
//     ), 
//     -20, 20, 80, 20
//   );

//   if (isInit) {
//     AddBoxToArray(b_cutoff_minus, UV_PowerSetting_Minus, CURRENT_PAGE);
//     AddBoxToArray(b_cutoff_plus, UV_PowerSetting_Plus, CURRENT_PAGE);
//   }

//   // background rectangle
//   drawGradRectFB(
//     TEXT_PADDING + p_p + p_w + r_p, p_y + r_o,
//     TFT_WIDTH - 2 * (TEXT_PADDING + p_p + p_w + r_p), p_h - 2 * r_o,
//     r_r, r_color, 2
//   );

//   // temperature text shadow
//   drawWordFB(
//     w_x, p_y + p_h/2 - FONT_STAN16.HEIGHT/2 + w_o, 
//     w_text, SHADOW_COLOR, FONT_STAN16
//   );

//   // temperature text
//   drawWordFB(
//     w_x, p_y + p_h/2 - FONT_STAN16.HEIGHT/2, 
//     w_text, FONT_COLOR, FONT_STAN16
//   );
// }

void drawUV_PowerSetting() {
  // volume ranges from 0-10

  // Create volume text from variable
  char text_v[64];
  uint16_t len = 0;
  len += snprintf(text_v + len, sizeof(text_v) - len, "%d", sysCurePower);
  len += snprintf(text_v + len, sizeof(text_v) - len, "%%");

  const char* text = "Power Setting";
  uint16_t pad_y = 55;
  uint16_t text_x = TFT_WIDTH - TEXT_PADDING - getWordLength(text_v, FONT_STAN12);
  uint16_t text_y = 3;
  uint16_t bar_y = 30;
  uint16_t bar_h = 30;
  uint16_t bar_r = 6;
  uint16_t w = 2;
  uint16_t button_pad = 10;
  uint16_t button_r = bar_r + 2;
  uint16_t outline_r = bar_r;
  uint16_t button_w = 1;
  uint16_t outline_c = FONT_COLOR;
  uint16_t bar_offset = TEXT_PADDING + bar_h + button_pad;
  uint16_t bar_x = bar_offset;
  uint16_t bar_w = TFT_WIDTH - 2 * bar_offset;
  uint16_t button2_x = bar_x + bar_w + button_pad;
  uint16_t volume_w = sysCurePower * bar_w / CURE_POWER_MAX;

  // colors
  uint16_t bg_color[] = {color565(16,20,0), color565(20,28,2)};
  uint16_t symbol_c[] = {color565(22,44,18), color565(22,44,18)};
  uint16_t max_bg[] = {color565(18,26,0), color565(22,34,0)};
  uint16_t shadow_c[] = {SHADOW_COLOR, SHADOW_COLOR};
  uint16_t bar_color[] = {color565(30,56,20), color565(24,44,16)};
  uint16_t bg_grad[] = {BG_COLOR, BG_COLOR};
  uint16_t* minus_bg;
  uint16_t* plus_bg;

  // Set button colors conditionally
  if (sysCurePower == CURE_POWER_MIN) {
    minus_bg = bg_grad;
    plus_bg = max_bg;
  } else if (sysCurePower == CURE_POWER_MAX) {
    minus_bg = max_bg;
    plus_bg = bg_grad; 
  } else {
    minus_bg = bg_grad;
    plus_bg = bg_grad;
  }

  uint16_t symbol_w = 3;
  uint16_t symbol_h = 17;
  uint16_t symbol_r = 1;
  uint16_t shadow_w = 2;
  uint16_t minus_x = 1 + TEXT_PADDING + ((bar_h - symbol_h) / 2);
  uint16_t plus1_x = 1 + button2_x + ((bar_h - symbol_h) / 2);
  uint16_t plus1_y = ((bar_h - symbol_w) / 2);
  uint16_t plus2_x = 1 + button2_x + ((bar_h - symbol_w) / 2);
  uint16_t plus2_y = ((bar_h - symbol_h) / 2);

  uint16_t shadow_y = -1;
  uint16_t shadow_h = bar_h + 1;
  uint16_t shadow_r = button_r + 2;

  // Title
  cursor_y += pad_y;
  drawWordFB(TEXT_PADDING, cursor_y + FONT_OFFSET, text, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(TEXT_PADDING, cursor_y, text, FONT_COLOR, FONT_STAN16);

  // percentage reading
  cursor_y += text_y;
  drawWordFB(text_x, cursor_y + FONT_OFFSET, text_v, SHADOW_COLOR, FONT_STAN12);
  drawWordFB(text_x, cursor_y, text_v, FONT_COLOR, FONT_STAN12);

  // percentage bar
  cursor_y += bar_y;
  drawGradRectFB(bar_x, cursor_y, bar_w, bar_h, bar_r, bg_color, 2);
  if (sysVolume) {
    // primary volume bar
    drawGradRectFB(bar_x, cursor_y, volume_w, bar_h, bar_r, bar_color, 2);

    // modified coordinates for square volume bar overlay
    uint16_t rect_offset = bar_r * 2;
    bar_x += rect_offset;
    volume_w -= rect_offset;

    if (sysCurePower >= 6) {
      bar_r = 3;
    } else {
      bar_r = 0;
    }

    if (sysCurePower < CURE_POWER_MAX) {
      // square volume bar overlay
      drawGradRectFB(bar_x, cursor_y, volume_w, bar_h, bar_r, bar_color, 2);
    }
  }

  // volume buttons shadow
  cursor_y += shadow_y;
  if (sysCurePower > CURE_POWER_MIN) {
    drawGradRectFB(
      TEXT_PADDING, cursor_y + SHADOW_OFFSET, 
      shadow_h, shadow_h, 
      shadow_r, shadow_c, 2
    );
  }
  if (sysCurePower < CURE_POWER_MAX) {
    drawGradRectFB(
      button2_x, cursor_y + SHADOW_OFFSET, 
      shadow_h, shadow_h, 
      shadow_r, shadow_c, 2
    );
  }

  // volume buttons bg
  drawGradRectFB(TEXT_PADDING, cursor_y, bar_h, bar_h, button_r, plus_bg, 2);
  drawGradRectFB(button2_x, cursor_y, bar_h, bar_h, button_r, minus_bg, 2);

  // volume buttons outline
  Box b_volume_minus = expandBox(
    drawRectOutlineFB(TEXT_PADDING, cursor_y, bar_h, bar_h, outline_r, button_w, outline_c),
    -20, 0, 10, 10
  );
  Box b_volume_plus  = expandBox(
    drawRectOutlineFB(button2_x, cursor_y, bar_h, bar_h, outline_r, button_w, outline_c),
    -20, 0, 80, 10
  );

  // add to touch sensing
  AddBoxToArray(b_volume_minus, UV_PowerSetting_Minus, CURRENT_PAGE);
  AddBoxToArray(b_volume_plus, UV_PowerSetting_Plus, CURRENT_PAGE);

  // minus symbol
  cursor_y += plus1_y;
  drawGradRectFB(minus_x, cursor_y, symbol_h, symbol_w, symbol_r, symbol_c, 2);

  // plus symbol
  drawGradRectFB(plus1_x, cursor_y, symbol_h, symbol_w, symbol_r, symbol_c, 2);
  cursor_y += plus2_y - plus1_y;
  drawGradRectFB(plus2_x, cursor_y, symbol_w, symbol_h, symbol_r, symbol_c, 2);
}

void drawUV_StartButton() {
  // Box constants
  uint16_t padding = 140;
  uint16_t box_x = 2 * TEXT_PADDING;
  uint16_t box_w = (TFT_WIDTH - (2 * box_x)) - 1;
  uint16_t box_h = 80;
  uint16_t box_r = 10;
  uint16_t line_w = 2;
  uint16_t shadow_x = box_x;

  cursor_y += padding;

  uint16_t box_grad[] = {y_bg, y_bg};
  uint16_t* colors = box_grad;

  // box shadow
  drawRectOutlineFB(shadow_x, cursor_y + 2, box_w, box_h, box_r, line_w, y_border2);
  
  // box body
  Box b_scale_f = expandBox(
    drawGradRectFB(box_x, cursor_y, box_w, box_h, box_r, colors, 2), 
    0, 0, 0, 0
  );

  // Add buttons to touch screen sensing
  AddBoxToArray(b_scale_f, UV_StartButton_Press, CURRENT_PAGE);

  // box outline
  drawRectOutlineFB(box_x, cursor_y, box_w, box_h, box_r, line_w, FONT_COLOR);

  const char* text = "Start Process";
  uint16_t label_x = (TFT_WIDTH - getWordLength(text, FONT_STAN16)) / 2;
  uint16_t label_h = FONT_STAN16.HEIGHT;
  uint16_t label_y = cursor_y + (box_h - label_h) / 2;

  drawWordFB(label_x, label_y + FONT_OFFSET, text, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(label_x, label_y, text, FONT_COLOR, FONT_STAN16);

  cursor_y += box_h;
}
