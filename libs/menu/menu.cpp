#include "menu.h"

// Functions to change tabs
bool isProcessActive() { 
  return (activePage == PAGE_ONE_PROCESS) || (activePage == PAGE_TWO_PROCESS); 
}

void tabOnePressed()   { if (!isProcessActive()) activePage = PAGE_ONE; }
void tabTwoPressed()   { if (!isProcessActive()) activePage = PAGE_TWO; }
void tabThreePressed() { if (!isProcessActive()) activePage = PAGE_THREE; }
void tabFourPressed()  { if (!isProcessActive()) activePage = PAGE_FOUR; }

// Create array of function pointers to assign to tabs easily
typedef void (*VoidFuncNoParameter) ();
VoidFuncNoParameter tabChangeFunctions[] = {
  tabOnePressed,
  tabTwoPressed,
  tabThreePressed,
  tabFourPressed
};

void initTabBoxes() {
  Box b(
    expandBox(
      drawRectOutlineFB(0, TFT_HEIGHT - OPTION_H, 
                      TFT_WIDTH / 4, OPTION_H * 2, 
                      1, 1, outlines[0]
      ), 
      0, 0, 0, 20
    )
  );

  AddBoxToArray(b, tabChangeFunctions[0], -1);

  // Draw background tab outlines
  for (int i = 1; i < 4; i++) {
    Box b(
      expandBox(
        drawRectOutlineFB(i * TFT_WIDTH / 4, 
                        TFT_HEIGHT - OPTION_H, 
                        TFT_WIDTH / 4, OPTION_H * 2, 
                        1, 1, outlines[i]
        ), 
        20, 0, 0, 20
      )
    );

    AddBoxToArray(b, tabChangeFunctions[i], -1);
  }
}

void drawMenu() {
  fillScreenFB(bgs[activePage]);
  drawMenu_Shadow();
  drawMenu_GradientFill();
  drawMenu_Outline();
  drawMenu_TabLabels();
  drawMenu_Title();
}

void drawMenu_GradientFill() {
  uint16_t h = TFT_HEIGHT - OPTION_H;
  uint16_t tab_h[] = {h, h, h, h};
  uint16_t* colors[] = {r_grad, y_grad, b_grad, g_grad};
  
  colors[activePage] = selection_grads[activePage];
  tab_h[activePage] = TFT_HEIGHT - SELECTION_H;

  for (uint16_t i = 0; i < 4; i++) {
    drawGradRectFB(i * TFT_WIDTH / 4, tab_h[i],
                    TFT_WIDTH / 4, OPTION_H * 2,
                    OPTION_R, 
                    colors[i], 2
                  );
  }
}

void drawMenu_Shadow() {
  uint16_t* colors = shadows[activePage];
  uint16_t y = TFT_HEIGHT - OPTION_H - TAB_SHADOW_OFFSET;
  uint16_t h = OPTION_H;
  uint16_t tab_y[] = {y, y, y, y};
  uint16_t tab_h[] = {h, h, h, h};

  uint16_t x_offset;
  uint16_t w_offset;

  tab_y[activePage] = TFT_HEIGHT - SELECTION_H - SELECTION_SHADOW_OFFSET;
  tab_h[activePage] = OPTION_H * 2;

  for (uint16_t i = 0; i < 4; i++) {
    if (i && i != (activePage)) {
      x_offset = SHADOW_W;
    } else {
      x_offset = 0;
    }

    if (i && i != (activePage)) {
      w_offset = 2 * SHADOW_W;
    } else if (i != (activePage)) {
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

void drawMenu_Outline() {
  uint16_t tab_order[4][4] = {
    {3,2,1,0},
    {3,0,2,1},
    {0,1,3,2},
    {0,1,2,3}
  };

  // Draw background tab outlines
  for (int i = 0; i < 3; i++) {
    Box b(
      drawRectOutlineFB(tab_order[activePage][i] * TFT_WIDTH / 4, 
                      TFT_HEIGHT - OPTION_H, 
                      TFT_WIDTH / 4, OPTION_H * 2, 
                      OPTION_R, OPTION_WEIGHT, 
                      outlines[tab_order[activePage][i]]
                    )
    );

    AddBoxToArray(b, tabChangeFunctions[i], -1);
  }

  // Draw active tab dark border
  drawRectOutlineFB(activePage * TFT_WIDTH / 4 - BORDER_OFFSET, TFT_HEIGHT - SELECTION_H, 
                    TFT_WIDTH / 4 + (2 * BORDER_OFFSET), SELECTION_H * 2, 
                    SELECTION_R, OPTION_WEIGHT, 
                    borders[activePage]
                  );

  // Draw active tab glow
  drawRectOutlineFB(activePage * TFT_WIDTH / 4, TFT_HEIGHT - SELECTION_H - GLOW_OFFSET, 
                    TFT_WIDTH / 4, SELECTION_H * 2, 
                    SELECTION_R, OPTION_WEIGHT, 
                    glows[activePage]
                  );

  drawRectOutlineFB(activePage * TFT_WIDTH / 4, TFT_HEIGHT - SELECTION_H, 
                    TFT_WIDTH / 4, SELECTION_H * 2, 
                    SELECTION_R, OPTION_WEIGHT, 
                    outlines[activePage]
                  );
}

void drawMenu_TabLabels() {
  uint16_t start_x[] = {17, 109, 171, 252};
  uint16_t start_x_upper[] = {15, 107, 167, 247};
  const char* labels[] = {"heat", "uv", "other", "setup"};
  const char* labels_upper[] = {"HEAT", "UV", "OTHER", "SETUP"};

  uint16_t h = 25;
  uint16_t sel_h = 30;

  // Shadow
  drawWordFB(
    start_x_upper[activePage], TFT_HEIGHT - sel_h - FONT_OFFSET, 
    labels_upper[activePage], 
    borders[activePage], 
    FONT_STAN16
  );

  // Glow
  drawWordFB(
    start_x_upper[activePage], TFT_HEIGHT - sel_h + FONT_OFFSET, 
    labels_upper[activePage], 
    glows[activePage], 
    FONT_STAN16
  );

  // Active Tab
  drawWordFB(
    start_x_upper[activePage], TFT_HEIGHT - sel_h, 
    labels_upper[activePage], 
    font_colors[activePage], 
    FONT_STAN16
  );

  for (int i = 0; i < 4; i++) {
    if (i != activePage) {
      drawWordFB(start_x[i], TFT_HEIGHT - h, labels[i], font_colors[i], FONT_STAN16);
    }
  }
}

void drawMenu_Title() {
  uint16_t title_color = font_colors[activePage];
  uint16_t shadow_color = borders[activePage];

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

  drawWordFB(title_x, title_y + FONT_OFFSET, title_text[activePage], shadow_color, FONT_STAN16);
  drawWordFB(title_x, title_y, title_text[activePage], title_color, FONT_STAN16);

  drawLineFB(
    line_border, line_y + FONT_OFFSET, 
    TFT_WIDTH - (line_border * 2), 0, 
    line_weight, 
    shadow_color
  );
  drawLineFB(line_border, line_y, TFT_WIDTH - (line_border * 2), 0, line_weight, title_color);
}
