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

#define GRAPH_H 180
#define GOAL_H  150
#define GRAPH_W (TFT_WIDTH - (2 * TEXT_PADDING) - 3)
#define POINT_W 3
#define POINT_R (POINT_W / 2)

static const uint16_t CURRENT_PAGE = PAGE_ONE_PROCESS;
static const uint16_t FONT_COLOR = font_colors[CURRENT_PAGE % 4];
static const uint16_t SHADOW_COLOR = borders[CURRENT_PAGE % 4];
static const uint16_t BG_COLOR = bgs[CURRENT_PAGE % 4];

// vector of coordinates for graph curve
std::vector<std::pair<uint16_t, uint16_t>> points = {};

// Graph origin (x, y), w, h
std::pair<uint16_t, uint16_t> origin;


void drawHeatProcess() {
  cursor_y = CURSOR_Y_DEFAULT;

  updateIsBakeComplete();

  drawHeatProcess_BG();
  drawHeatProcess_CurrentTemp();
  origin = drawHeatProcess_TempGraph();
  drawHeatProcess_Curve();
  drawHeatProcess_ProgressBar();
  drawHeatProcess_Cancel();

  if (show_bakeComplete) {
    drawHeatProcess_bakeComplete();
  }
}

void updateIsBakeComplete() {
  if (processTimeInS >= sysBakeTimeS) {
    isBakeComplete = true;

    if (!wasBakeComplete) {
      show_bakeComplete = true;
    }
  }
}

void drawHeatProcess_BG() {
  fillScreenFB(BG_COLOR);

  // Title constants
  uint16_t title_x = TEXT_PADDING;
  uint16_t title_y = 15;

  // Line constants
  uint16_t line_y = 40;
  uint16_t shadow_y = line_y + FONT_OFFSET;
  uint16_t line_border = TEXT_PADDING;
  uint16_t line_weight = 1;

  char label[64];
  uint16_t len = 0;

  if (isBakeComplete) {
    const char* text = "Bake Process";
    len += snprintf(label + len, sizeof(label) - len, text);
  } else {
    const char* text = "Bake Process Complete"; 
    len += snprintf(label + len, sizeof(label) - len, text);
  }

  drawWordFB(title_x, title_y + FONT_OFFSET, label, r_border, FONT_STAN16);
  drawWordFB(title_x, title_y, label, r_font, FONT_STAN16);

  drawLineFB(line_border, shadow_y, TFT_WIDTH - (line_border * 2), 0, line_weight, r_border);
  drawLineFB(line_border, line_y, TFT_WIDTH - (line_border * 2), 0, line_weight, r_font);

  cursor_y = line_y;
}

void drawHeatProcess_CurrentTemp() {
  uint16_t padding = 20;
  uint16_t text_x = TEXT_PADDING;
  uint16_t shadow_y = 1;
  Font text_f = FONT_STAN12;

  const char* current = "Temp: ";
  const char* append = (sysIsTempF) ? "^F" : "^C";
  char cur_t[64];
  uint16_t len = 0;
  uint16_t temp = (sysIsTempF) ? getTempFFromC(sysTempInC) : sysTempInC;
  len += snprintf(cur_t + len, sizeof(cur_t) - len, current);
  len += snprintf(cur_t + len, sizeof(cur_t) - len, "%d", temp);
  len += snprintf(cur_t + len, sizeof(cur_t) - len, append);

  cursor_y += padding;
  drawWordFB(text_x, cursor_y + shadow_y, cur_t, SHADOW_COLOR, text_f);
  drawWordFB(text_x, cursor_y, cur_t, FONT_COLOR, text_f);

  const char* goal = "Target: ";
  char goal_t[64];
  temp = (sysIsTempF) ? sysBakeTempF : sysBakeTempC;
  len = 0;
  len += snprintf(goal_t + len, sizeof(goal_t) - len, goal);
  len += snprintf(goal_t + len, sizeof(goal_t) - len, "%d", temp);
  len += snprintf(goal_t + len, sizeof(goal_t) - len, append);

  text_x = TFT_WIDTH - TEXT_PADDING - getWordLength(goal_t, text_f);

  drawWordFB(text_x, cursor_y + shadow_y, goal_t, SHADOW_COLOR, text_f);
  drawWordFB(text_x, cursor_y, goal_t, FONT_COLOR, text_f);

  cursor_y += text_f.HEIGHT;
}

std::pair<uint16_t, uint16_t> drawHeatProcess_TempGraph() {
  uint16_t padding = 20;
  uint16_t line_w = 3;
  uint16_t frame_x = TEXT_PADDING + 1;
  uint16_t frame_c = color565(31,50,25);

  cursor_y += padding;
  drawLineFB(frame_x, cursor_y, 0, GRAPH_H, line_w, frame_c);
  cursor_y += GRAPH_H;
  drawLineFB(frame_x, cursor_y, GRAPH_W, 0, line_w, frame_c);

  uint16_t color = color565(28,32,16);
  uint16_t space_l = 10;
  uint16_t line_l = 15;
  uint16_t goal_r = 2;
  uint16_t goal_h = GRAPH_H - GOAL_H;
  uint16_t goal_c[] = {color, color};
  uint16_t goal_start_x = frame_x + line_w + (space_l / 2);
  uint16_t goal_end_x = frame_x + GRAPH_W;
  line_w = 5;
  
  // Create dotted line
  cursor_y += goal_h - GRAPH_H - line_w / 2;
  for (uint16_t x = goal_start_x; x < goal_end_x; x += line_l + space_l) {
    if (x + line_l > goal_end_x) line_l = goal_end_x - x;
    drawGradRectFB(x, cursor_y, line_l, line_w, goal_r, goal_c, 2);
  }

  char goal_t[64];
  const char* append = (sysIsTempF) ? "^F" : "^C";
  uint16_t temp = (sysIsTempF) ? sysBakeTempF : sysBakeTempC;
  uint16_t len = 0;
  len += snprintf(goal_t + len, sizeof(goal_t) - len, "%d", temp);
  len += snprintf(goal_t + len, sizeof(goal_t) - len, append);

  Font label_f = FONT_STAN12;
  uint16_t label_y = 10;
  uint16_t label_x = TFT_WIDTH - frame_x - getWordLength(goal_t, label_f);
  uint16_t label_c = color565(31,38,19);

  cursor_y += (line_w / 2) + label_y;
  drawWordFB(label_x, cursor_y, goal_t, label_c, label_f);

  cursor_y += GRAPH_H - goal_h - label_y;

  uint16_t origin_x = frame_x + line_w / 2;
  uint16_t origin_y = cursor_y - line_w / 2;
  return {origin_x, origin_y};
}

void drawHeatProcess_ProgressBar() {
  uint16_t padding = 15;
  uint16_t bg_x = TEXT_PADDING;
  uint16_t bg_w = TFT_WIDTH - (2 * TEXT_PADDING);
  uint16_t bg_h = 8;
  uint16_t bg_r = 2;
  uint16_t bg_color = color565(21,10,4);
  uint16_t bg_c[] = {bg_color, bg_color};

  uint16_t box_min_w = 4;
  uint16_t box_w = box_min_w + ((bg_w - box_min_w) * processTimeInS) / sysBakeTimeS;
  uint16_t box_color = color565(31,54,27);
  uint16_t box_c[] = {box_color, box_color};

  // Clamp loading bar width
  if (box_w > bg_w) box_w = bg_w;

  cursor_y += padding;
  drawGradRectFB(bg_x, cursor_y, bg_w, bg_h, bg_r, bg_c, 2);
  drawGradRectFB(bg_x, cursor_y, box_w, bg_h, bg_r, box_c, 2);


  // Create text from variable for current time
  char time_text[64];
  uint16_t len = 0;
  uint16_t min = processTimeInS / 60;
  uint16_t sec = processTimeInS % 60;
  len += snprintf(time_text + len, sizeof(time_text) - len, "%d", min);
  len += snprintf(time_text + len, sizeof(time_text) - len, ":");
  // Format seconds place
  if (!sec) {
    len += snprintf(time_text + len, sizeof(time_text) - len, "00"); 
  } else if (sec < 10) {
    len += snprintf(time_text + len, sizeof(time_text) - len, "0"); 
    len += snprintf(time_text + len, sizeof(time_text) - len, "%d", sec); 
  } else {
    len += snprintf(time_text + len, sizeof(time_text) - len, "%d", sec); 
  } 

  uint16_t bar_padding = 20;
  Font text_font = FONT_STAN12;
  uint16_t time_text_x = TEXT_PADDING;
  uint16_t shadow_y = 1;

  cursor_y += bar_padding;
  drawWordFB(time_text_x, cursor_y + shadow_y, time_text, SHADOW_COLOR, text_font);
  drawWordFB(time_text_x, cursor_y, time_text, FONT_COLOR, text_font);


  // Create text from variable for end time
  char end_text[64];
  len = 0;
  min = sysBakeTimeS / 60;
  sec = sysBakeTimeS % 60;
  len += snprintf(end_text + len, sizeof(end_text) - len, "%d", min);
  len += snprintf(end_text + len, sizeof(end_text) - len, ":");
  // Format seconds place
  if (!sec) {
    len += snprintf(end_text + len, sizeof(end_text) - len, "00"); 
  } else if (sec < 10) {
    len += snprintf(end_text + len, sizeof(end_text) - len, "0"); 
    len += snprintf(end_text + len, sizeof(end_text) - len, "%d", sec); 
  } else {
    len += snprintf(end_text + len, sizeof(end_text) - len, "%d", sec); 
  } 

  uint16_t end_text_x = TFT_WIDTH - TEXT_PADDING - getWordLength(end_text, text_font);

  drawWordFB(end_text_x, cursor_y + shadow_y, end_text, SHADOW_COLOR, text_font);
  drawWordFB(end_text_x, cursor_y, end_text, FONT_COLOR, text_font);
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

  char label[64];
  uint16_t len = 0;

  if (!isBakeComplete) {
    const char* text = "Cancel";  
    len += snprintf(label + len, sizeof(label) - len, text);
  } else {
    const char* text = "Return";
    len += snprintf(label + len, sizeof(label) - len, text);
  }

  uint16_t label_x = (TFT_WIDTH - getWordLength(label, FONT_STAN16)) / 2;
  uint16_t label_h = FONT_STAN16.HEIGHT;
  uint16_t label_y = cursor_y + (box_h - label_h) / 2;

  drawWordFB(label_x, label_y + FONT_OFFSET, label, r_border2, FONT_STAN16);
  drawWordFB(label_x, label_y, label, r_font, FONT_STAN16);

  cursor_y += box_h;
}

void drawHeatProcess_Curve() {
  uint16_t color = FONT_COLOR;
  uint16_t point_c[] = {color, color};

  for (uint16_t i = 0; i < points.size(); i++) {
    // drawGradRectFB(points[i].first, points[i].second, POINT_W, POINT_W, POINT_R, point_c, 2);

    uint16_t x = points[i].first;
    uint16_t y = points[i].second;

    drawPixelFB(x, y, color);
    drawPixelFB(x+1, y+1, color);
    drawPixelFB(x+1, y, color);
    drawPixelFB(x, y+1, color);

    drawPixelFB(x-1, y, color);
    drawPixelFB(x-1, y+1, color);
    drawPixelFB(x, y-1, color);
    drawPixelFB(x+1, y-1, color);
    drawPixelFB(x+2, y, color);
    drawPixelFB(x+2, y+1, color);
    drawPixelFB(x, y+2, color);
    drawPixelFB(x+1, y+2, color);
  }
}

void drawHeatProcess_addPoint() {
  uint16_t point_x = origin.first + ((GRAPH_W * processTimeInS) / sysBakeTimeS) - POINT_R;
  uint16_t point_y = origin.second - ((GOAL_H * sysTempInC) / sysBakeTempC) - POINT_R;

  if (point_x < GRAPH_W + origin.first) {
    points.push_back({point_x, point_y});
  }
}

void drawHeatProcess_bakeComplete() {
  uint16_t box_y = 55;
  uint16_t box_x = TEXT_PADDING + 15;
  uint16_t box_h = 270;
  uint16_t box_w = TFT_WIDTH - 2 * box_x;
  uint16_t box_r = 10;
  uint16_t color = BG_COLOR;
  uint16_t box_c[] = {color, color};

  uint16_t line_c = FONT_COLOR;
  uint16_t line_w = 3;

  uint16_t line_2_c = color565(30,30,10);
  uint16_t line_2_t = 1;
  uint16_t line_2_x = box_x - line_2_t;
  uint16_t line_2_y = box_y - line_2_t;
  uint16_t line_2_w = box_w + 2 * line_2_t;
  uint16_t line_2_h = box_h + 2 * line_2_t;
  uint16_t line_2_r = box_r + line_2_t - 1;

  uint16_t line_3_c = color565(20,10,0);
  uint16_t line_3_t = 2;
  uint16_t line_3_x = box_x - line_3_t;
  uint16_t line_3_y = box_y - line_3_t;
  uint16_t line_3_w = box_w + 2 * line_3_t;
  uint16_t line_3_h = box_h + 2 * line_3_t;
  uint16_t line_3_r = box_r + line_3_t - 1;

  uint16_t shadow_h = line_3_h + SHADOW_OFFSET;

  drawGradRectFB(box_x, box_y, box_w, box_h, box_r, box_c, 2);
  drawRectOutlineFB(line_3_x, line_3_y, line_3_w, shadow_h, line_3_r, line_w, SHADOW_COLOR);
  drawRectOutlineFB(line_3_x, line_3_y, line_3_w, line_3_h, line_3_r, line_w, line_3_c);
  drawRectOutlineFB(line_2_x, line_2_y, line_2_w, line_2_h, line_2_r, line_w, line_2_c);
  drawRectOutlineFB(box_x, box_y, box_w, box_h, box_r, line_w, line_c);

  const char* title = "Finished!";
  const Font title_f = FONT_STAN16;
  uint16_t title_x = box_x + 20;
  uint16_t title_y = box_y + 25;

  drawWordFB(title_x, title_y + SHADOW_OFFSET, title, SHADOW_COLOR, title_f);
  drawWordFB(title_x, title_y, title, FONT_COLOR, title_f);

  uint16_t x_w = 20;
  uint16_t x_x = box_x + box_w - x_w - 20;
  uint16_t x_y = title_y + (FONT_STAN16.HEIGHT / 2) - (x_w / 2);
  uint16_t x_t = 8;

  Box b_close_alert = expandBox(
    drawLineFB(x_x, x_y, x_w, x_w, x_t, FONT_COLOR, true),
    0, 0, 0, 0
  );
  drawLineFB(x_x, x_y + x_w, x_w, -x_w, x_t, FONT_COLOR, true);

  Serial.print("(");
  Serial.print(b_close_alert._get_x1()); Serial.print(", ");
  Serial.print(b_close_alert._get_y1()); Serial.print("), (");
  Serial.print(b_close_alert._get_x2()); Serial.print(", ");
  Serial.print(b_close_alert._get_y2()); Serial.println(")");

  AddBoxToArray(b_close_alert, bake_bakeComplete_close, PAGE_ONE_PROCESS);
}

/*
Bake finished
- title: "Bake finished - Cooling"
- Cancel button -> "Return to Menu"
*/