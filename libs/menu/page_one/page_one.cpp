#include "page_one.h"
#include "helpers/helpers.h"
#include "page_one_helpers.h"

static const uint16_t CURRENT_PAGE = PAGE_ONE;
static const uint16_t FONT_COLOR = font_colors[CURRENT_PAGE];
static const uint16_t SHADOW_COLOR = borders[CURRENT_PAGE];

void drawHeat() {
  cursor_y = CURSOR_Y_DEFAULT;

  drawHeat_TimeSetting();
  drawHeat_TempSetting();
  drawHeat_StartButton();
}

void drawHeat_TimeSetting() {
  // const char* title = "Bake Time (Seconds)";
  uint16_t title_padding = 55;
  cursor_y += title_padding;

  const char* title = "Bake Time";
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
  uint16_t p_col[] = {color565(31,54,27), color565(24,44,22)};
  uint16_t p_grey[] = {color565(31,38,19), color565(28,32,16)};
  uint16_t shadow_c[] = {r_border2, r_border2};

  // rectangle variables
  uint16_t r_p = 25;
  uint16_t r_r = 3;
  uint16_t r_o = 2;
  uint16_t r_color[] = {color565(17,5,2), color565(21,10,4)};

  // Create temp text from variable
  char w_text[64];
  uint16_t len = 0;
  uint16_t min = sysBakeTimeS / 60;
  uint16_t sec = sysBakeTimeS % 60;
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
  bool isMin = sysBakeTimeS <= BAKE_TIME_S_MIN;
  bool isMax = sysBakeTimeS >= BAKE_TIME_S_MAX;

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

  if (isInit) {
    AddBoxToArray(b_cutoff_minus, bake_TimeSetting_Minus, CURRENT_PAGE);
    AddBoxToArray(b_cutoff_plus, bake_TimeSetting_Plus, CURRENT_PAGE);
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

void drawHeat_TempSetting() {
  uint16_t offset_h = 55;
  uint16_t text_h = 30;
  cursor_y += offset_h;

  const char* title = "Temperature Set";
  uint16_t shadow_y = cursor_y + FONT_OFFSET;

  drawWordFB(MARGIN_X, shadow_y, title, SHADOW_COLOR, FONT_STAN16);
  drawWordFB(MARGIN_X, cursor_y, title, FONT_COLOR, FONT_STAN16);

  cursor_y += text_h;

  // Test if temp is at max values for animation
  bool isMin = (sysIsTempF) ? sysBakeTempF <= BAKE_TEMP_F_MIN : sysBakeTempC <= BAKE_TEMP_C_MIN;
  bool isMax = (sysIsTempF) ? sysBakeTempF >= BAKE_TEMP_F_MAX : sysBakeTempC >= BAKE_TEMP_C_MAX;

  // pointer variables
  uint16_t p_y = cursor_y;
  uint16_t p_h = 36;
  uint16_t p_w = p_h;
  uint16_t p_r = 4;
  uint16_t p_p = 20;
  uint16_t p_col[] = {color565(31,54,27), color565(24,44,22)};
  uint16_t p_grey[] = {color565(31,38,19), color565(28,32,16)};
  uint16_t shadow_c[] = {r_border2, r_border2};

  cursor_y += p_h;

  // rectangle variables
  uint16_t r_p = 25;
  uint16_t r_r = 3;
  uint16_t r_o = 2;
  uint16_t r_color[] = {color565(17,5,2), color565(21,10,4)};

  // Create temp text from variable
  char w_text[64];
  uint16_t len = 0;
  const char* append = sysIsTempF ? " ^F" : " ^C";
  int16_t temp = sysIsTempF ? sysBakeTempF : sysBakeTempC;
  len += snprintf(w_text + len, sizeof(w_text) - len, "%d", temp);
  len += snprintf(w_text + len, sizeof(w_text) - len, append);

  uint16_t w_x = (TFT_WIDTH - getWordLength(w_text, FONT_STAN16))/2;
  uint16_t w_o = 2;

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

  if (isInit) {
    AddBoxToArray(b_cutoff_minus, bake_TempSetting_Minus, CURRENT_PAGE);
    AddBoxToArray(b_cutoff_plus, bake_TempSetting_Plus, CURRENT_PAGE);
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

void drawHeat_StartButton() {
  // Box constants
  uint16_t padding = 110;
  uint16_t box_x = 2 * TEXT_PADDING;
  uint16_t box_w = (TFT_WIDTH - (2 * box_x)) - 1;
  uint16_t box_h = 80;
  uint16_t box_r = 10;
  uint16_t line_w = 2;
  uint16_t shadow_x = box_x;

  cursor_y += padding;

  uint16_t box_grad[] = {r_bg, r_bg};
  uint16_t* colors = box_grad;

  drawRectOutlineFB(shadow_x, cursor_y + 2, box_w, box_h, box_r, line_w, r_border2);
  
  Box b_scale_f = expandBox(
    drawGradRectFB(box_x, cursor_y, box_w, box_h, box_r, colors, 2), 
    0, 0, 0, 0
  );

  // Add buttons to touch screen sensing
  AddBoxToArray(b_scale_f, bake_StartButton_Press, CURRENT_PAGE);

  drawRectOutlineFB(box_x, cursor_y, box_w, box_h, box_r, line_w, r_font);

  const char* text = "Start Process";
  uint16_t label_x = (TFT_WIDTH - getWordLength(text, FONT_STAN16)) / 2;
  uint16_t label_h = FONT_STAN16.HEIGHT;
  uint16_t label_y = cursor_y + (box_h - label_h) / 2;

  drawWordFB(label_x, label_y + FONT_OFFSET, text, r_border2, FONT_STAN16);
  drawWordFB(label_x, label_y, text, r_font, FONT_STAN16);

  cursor_y += box_h;
}
