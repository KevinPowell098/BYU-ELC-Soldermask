#include "menu.h"


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

