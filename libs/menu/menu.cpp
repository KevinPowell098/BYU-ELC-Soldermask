#include "menu.h"


// Page layout constants
const int SELECTION_H = 45;
const int SELECTION_R = 10;
const int GLOW_OFFSET = 1;
const int BORDER_OFFSET = 1;
const int OPTION_H = 30;
const int OPTION_R = 7;
const int OPTION_WIEGHT = 3;
const int TAB_SHADOW_OFFSET = 10;
const int SHADOW_OFFSET = 2;
const int SELECTION_SHADOW_OFFSET = 5;
const int SHADOW_R = 12;
const int SHADOW_W = 6;
const int FONT_OFFSET = 1;
const int TEXT_PADDING = 12;

// Tab outline colors
uint16_t r_outline = color565(31, 0, 0);
uint16_t y_outline = color565(30, 50, 0);
uint16_t b_outline = color565(0, 0, 31);
uint16_t g_outline = color565(0, 40, 0);
uint16_t outlines[] = {r_outline, y_outline, b_outline, g_outline};

uint16_t r_glow = color565(31, 30, 15);
uint16_t y_glow = color565(31, 60, 15);
uint16_t b_glow = color565(20, 40, 20);
uint16_t g_glow = color565(15, 50, 15);
uint16_t glows[] = {r_glow, y_glow, b_glow, g_glow};

// Tab outline dark border
uint16_t r_border = color565(15, 0, 0);
uint16_t y_border = color565(15, 25, 0);
uint16_t b_border = color565(0, 0, 15);
uint16_t g_border = color565(0, 20, 0);
uint16_t borders[] = {r_border, y_border, b_border, g_border};

// Bg colors
uint16_t r_bg = color565(25,15,5);
uint16_t y_bg = color565(25,40,10);
uint16_t b_bg = color565(5,15,15);
uint16_t g_bg = color565(10,30,5);
uint16_t bgs[] = {r_bg, y_bg, b_bg, g_bg};

// Default gradient colors
uint16_t r_grad[] = {color565(31, 0, 0), color565(20, 10, 0)};
uint16_t y_grad[] = {color565(30, 50, 0), color565(25, 40, 0)};
uint16_t b_grad[] = {color565(0, 0, 31), color565(5, 0, 20)};
uint16_t g_grad[] = {color565(0, 40, 0), color565(0, 30, 5)};

// Selection gradient colors
uint16_t r_sel_grad[] = {color565(31, 0, 0), color565(25, 10, 0)};
uint16_t y_sel_grad[] = {color565(30, 50, 0), color565(25, 40, 0)};
uint16_t b_sel_grad[] = {color565(0, 0, 31), color565(5, 0, 20)};
uint16_t g_sel_grad[] = {color565(0, 40, 0), color565(0, 30, 5)};
uint16_t* selection_grads[] = {r_sel_grad, y_sel_grad, b_sel_grad, g_sel_grad};

// Tab shadow colors
uint16_t r_shadow[] = {color565(25,15,5), color565(15,5,10)};
uint16_t y_shadow[] = {color565(25,40,10), color565(12,20,5)};
uint16_t b_shadow[] = {color565(5,15,15), color565(0,0,5)};
uint16_t g_shadow[] = {color565(10,30,5), color565(0,15,0)};
uint16_t* shadows[] = {r_shadow, y_shadow, b_shadow, g_shadow};

// Font colors
uint16_t r_font = color565(31,50,25);
uint16_t y_font = color565(31,63,20);
uint16_t b_font = color565(25,50,31);
uint16_t g_font = color565(25,63,25);
uint16_t font_colors[] = {r_font, y_font, b_font, g_font};

// Font drop shadow
uint16_t r_border2 = color565(15, 10, 10);
uint16_t y_border2 = color565(15, 25, 10);
uint16_t b_border2 = color565(10, 10, 15);
uint16_t g_border2 = color565(6, 20, 6);


void drawBG(uint16_t activeTab) {
  fillScreenFB(bgs[activeTab]);
  drawShadow(activeTab);
  drawGradientFill(activeTab);
  drawOutline(activeTab);
  drawTabLabels(activeTab);
  drawTitle(activeTab);
}

void drawGradientFill(uint16_t activeTab) {
  uint16_t h = TFT_HEIGHT - OPTION_H;
  uint16_t tab_h[] = {h, h, h, h};
  uint16_t* colors[] = {r_grad, y_grad, b_grad, g_grad};
  
  colors[activeTab] = selection_grads[activeTab];
  tab_h[activeTab] = TFT_HEIGHT - SELECTION_H;

  for (uint16_t i = 0; i < 4; i++) {
    drawGradRectFB(i * TFT_WIDTH / 4, tab_h[i],
                    TFT_WIDTH / 4, OPTION_H * 2,
                    OPTION_R, 
                    colors[i], 2
                  );
  }
}

void drawShadow(uint16_t activeTab) {
  uint16_t* colors = shadows[activeTab];
  uint16_t y = TFT_HEIGHT - OPTION_H - TAB_SHADOW_OFFSET;
  uint16_t h = OPTION_H;
  uint16_t tab_y[] = {y, y, y, y};
  uint16_t tab_h[] = {h, h, h, h};

  uint16_t x_offset;
  uint16_t w_offset;

  tab_y[activeTab] = TFT_HEIGHT - SELECTION_H - SELECTION_SHADOW_OFFSET;
  tab_h[activeTab] = OPTION_H * 2;

  for (uint16_t i = 0; i < 4; i++) {
    if (i && i != (activeTab)) {
      x_offset = SHADOW_W;
    } else {
      x_offset = 0;
    }

    if (i && i != (activeTab)) {
      w_offset = 2 * SHADOW_W;
    } else if (i != (activeTab)) {
      w_offset = SHADOW_W;
    } else {
      w_offset = 0;
    }

    drawGradRectFB((i * TFT_WIDTH / 4) - x_offset, tab_y[i],
                    (TFT_WIDTH / 4) + w_offset, tab_h[i],
                    SHADOW_R, 
                    colors, 2
                  );
  }
}

void drawOutline(uint16_t activeTab) {
  uint16_t tab_order[4][4] = {
    {3,2,1,0},
    {3,0,2,1},
    {0,1,3,2},
    {0,1,2,3}
  };

  // Draw background tab outlines
  for (int i = 0; i < 3; i++) {
    drawRectOutlineFB(tab_order[activeTab][i] * TFT_WIDTH / 4, TFT_HEIGHT - OPTION_H, 
                      TFT_WIDTH / 4, OPTION_H * 2, 
                      OPTION_R, OPTION_WIEGHT, 
                      outlines[tab_order[activeTab][i]]
                    );
  }

  // Draw active tab dark border
  drawRectOutlineFB(activeTab * TFT_WIDTH / 4 - BORDER_OFFSET, TFT_HEIGHT - SELECTION_H, 
                    TFT_WIDTH / 4 + (2 * BORDER_OFFSET), SELECTION_H * 2, 
                    SELECTION_R, OPTION_WIEGHT, 
                    borders[activeTab]
                  );

  // Draw active tab glow
  drawRectOutlineFB(activeTab * TFT_WIDTH / 4, TFT_HEIGHT - SELECTION_H - GLOW_OFFSET, 
                    TFT_WIDTH / 4, SELECTION_H * 2, 
                    SELECTION_R, OPTION_WIEGHT, 
                    glows[activeTab]
                  );

  drawRectOutlineFB(activeTab * TFT_WIDTH / 4, TFT_HEIGHT - SELECTION_H, 
                    TFT_WIDTH / 4, SELECTION_H * 2, 
                    SELECTION_R, OPTION_WIEGHT, 
                    outlines[activeTab]
                  );
}

void drawTabLabels(uint16_t activeTab) {
  uint16_t start_x[] = {17, 109, 171, 252};
  uint16_t start_x_upper[] = {15, 107, 167, 247};
  const char* labels[] = {"heat", "uv", "other", "setup"};
  const char* labels_upper[] = {"HEAT", "UV", "OTHER", "SETUP"};

  uint16_t h = 25;
  uint16_t sel_h = 30;

  // Shadow
  drawWordFB(
    start_x_upper[activeTab], TFT_HEIGHT - sel_h - FONT_OFFSET, 
    labels_upper[activeTab], 
    borders[activeTab], 
    FONT_STAN16
  );

  // Glow
  drawWordFB(
    start_x_upper[activeTab], TFT_HEIGHT - sel_h + FONT_OFFSET, 
    labels_upper[activeTab], 
    glows[activeTab], 
    FONT_STAN16
  );

  // Active Tab
  drawWordFB(
    start_x_upper[activeTab], TFT_HEIGHT - sel_h, 
    labels_upper[activeTab], 
    font_colors[activeTab], 
    FONT_STAN16
  );

  for (int i = 0; i < 4; i++) {
    if (i != activeTab) {
      drawWordFB(start_x[i], TFT_HEIGHT - h, labels[i], font_colors[i], FONT_STAN16);
    }
  }
}

void drawTitle(uint16_t activeTab) {
  uint16_t title_color = font_colors[activeTab];
  uint16_t shadow_color = borders[activeTab];

  // Title constants
  uint16_t title_x = TEXT_PADDING;
  uint16_t title_y = 15;

  // Line constants
  uint16_t line_y = 40;
  uint16_t line_border = TEXT_PADDING;
  uint16_t line_weight = 1;

  const char* title_text[] = {
    "Heat Dry",
    "UV Curing",
    "Something else",
    "Setup and Configuration"
  };


  drawWordFB(title_x, title_y + FONT_OFFSET, title_text[activeTab], shadow_color, FONT_STAN16);
  drawWordFB(title_x, title_y, title_text[activeTab], title_color, FONT_STAN16);

  drawLineFB(
    line_border, line_y + FONT_OFFSET, 
    TFT_WIDTH - (line_border * 2), 0, 
    line_weight, 
    shadow_color
  );
  drawLineFB(line_border, line_y, TFT_WIDTH - (line_border * 2), 0, line_weight, title_color);
}

void drawSetup() {
  drawTempScaleSelection();
  drawVolumeSelection();
}

void drawTempScaleSelection() {
  // Update to use global variable !!!!!
  bool isTempF = true;

  const char* text = "Temperature Scale";
  uint16_t title_y = 55;

  drawWordFB(TEXT_PADDING, title_y + FONT_OFFSET, text, g_border2, FONT_STAN16);
  drawWordFB(TEXT_PADDING, title_y, text, g_font, FONT_STAN16);

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

  if (isTempF) {
    f_colors = selection_grad;
    c_colors = box_grad;
    shadow_x = box2_x;
  }

  drawRectOutlineFB(shadow_x, box_y + 2, box_w, box_h, box_r, line_w, g_border2);
  
  drawGradRectFB(TEXT_PADDING, box_y, box_w, box_h, box_r, f_colors, 2);
  drawGradRectFB(box2_x, box_y, box_w, box_h, box_r, c_colors, 2);

  drawRectOutlineFB(TEXT_PADDING, box_y, box_w, box_h, box_r, line_w, g_font);
  drawRectOutlineFB(box2_x, box_y, box_w, box_h, box_r, line_w, g_font);

  const char* text_f = "^F";
  const char* text_c = "^C";
  uint16_t label_offset = 10;
  uint16_t label_f_x = TEXT_PADDING + (box_w / 2) - label_offset;
  uint16_t label_c_x = (TEXT_PADDING * 2) + (box_w * 1.5) - label_offset;
  uint16_t label_y = 107;

  if (isTempF) {
    drawWordFB(label_f_x, label_y + FONT_OFFSET, text_f, g_border2, FONT_STAN16);
  } else {
    drawWordFB(label_c_x, label_y + FONT_OFFSET, text_c, g_border2, FONT_STAN16);
  }

  drawWordFB(label_f_x, label_y, text_f, g_font, FONT_STAN16);
  drawWordFB(label_c_x, label_y, text_c, g_font, FONT_STAN16);
}

void drawVolumeSelection() {
  // Update to use global variable !!!!!
  // volume ranges from 0-10
  uint16_t volume = 10;

  // Create volume text from variable
  if (volume > 10) volume = 10;
  char text_v[64];
  uint16_t len = 0;
  const char* append = volume ? "0%%" : "%%";
  len += snprintf(text_v + len, sizeof(text_v) - len, "%d", volume);
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
  uint16_t volume_w = volume * bar_w / 10.0;

  // colors
  uint16_t bar_color[] = {color565(3,16,3), color565(5,20,5)};
  // uint16_t symbol_c[] = {g_font, g_font};
  uint16_t symbol_c[] = {color565(18,40,18), color565(18,40,18)};
  uint16_t max_bg[] = {color565(6,22,1), color565(8,26,3)};
  uint16_t shadow_c[] = {g_border2, g_border2};
  uint16_t volume_color[] = {color565(22,57,22), color565(19,51,19)};
  uint16_t g_bg_grad[] = {g_bg, g_bg};
  uint16_t* minus_bg;
  uint16_t* plus_bg;

  if (!volume) {
    minus_bg = g_bg_grad;
    plus_bg = max_bg;
  } else if (volume == 10) {
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
  drawWordFB(TEXT_PADDING, title_y + FONT_OFFSET, text, g_border2, FONT_STAN16);
  drawWordFB(TEXT_PADDING, title_y, text, g_font, FONT_STAN16);

  // percentage reading
  drawWordFB(text_x, text_y + FONT_OFFSET, text_v, g_border2, FONT_STAN12);
  drawWordFB(text_x, text_y, text_v, g_font, FONT_STAN12);

  // percentage bar
  drawGradRectFB(bar_x, bar_y, bar_w, bar_h, bar_r, bar_color, 2);
  if (volume) {
    drawGradRectFB(bar_x, bar_y, volume_w, bar_h, bar_r, volume_color, 2);
  }

  // volume buttons shadow
  if (volume) {
    drawGradRectFB(TEXT_PADDING, shadow_y + SHADOW_OFFSET, shadow_h, shadow_h, shadow_r, shadow_c, 2);
  }
  if (volume != 10) {
    drawGradRectFB(button2_x, shadow_y + SHADOW_OFFSET, shadow_h, shadow_h, shadow_r, shadow_c, 2);
  }

  // volume buttons bg
  drawGradRectFB(TEXT_PADDING, shadow_y, bar_h, bar_h, button_r, plus_bg, 2);
  drawGradRectFB(button2_x, shadow_y, bar_h, bar_h, button_r, minus_bg, 2);

  // volume buttons outline
  drawRectOutlineFB(TEXT_PADDING, shadow_y, bar_h, bar_h, button_r, button_w, button_c);
  drawRectOutlineFB(button2_x, shadow_y, bar_h, bar_h, button_r, button_w, button_c);

  // minus symbol
  drawGradRectFB(minus_x, plus1_y - 1, symbol_h, symbol_w, symbol_r, symbol_c, 2);

  // plus symbol
  drawGradRectFB(plus1_x, plus1_y - 1, symbol_h, symbol_w, symbol_r, symbol_c, 2);
  drawGradRectFB(plus2_x, plus2_y - 1, symbol_w, symbol_h, symbol_r, symbol_c, 2);
}