#include "lcd.h"
#include <esp_heap_caps.h>

uint16_t* framebuffer = nullptr;

void initFramebuffer() {
  if (!framebuffer) {
    framebuffer = new uint16_t[TFT_WIDTH * TFT_HEIGHT];
  }
  memset(framebuffer, 0, TFT_WIDTH * TFT_HEIGHT * sizeof(uint16_t));
}

uint16_t getColorFromGradient(const uint16_t* colors, size_t color_cnt, uint16_t place) {

    if (color_cnt == 0) return 0;
    if (place == 0) return colors[0];
    if (place >= 1000) return colors[color_cnt - 1];

    // Identify segment
    uint32_t temp = place * (color_cnt - 1);
    size_t i = temp / 1000;
    uint16_t t = temp - i * 1000;  // 0–999

    uint16_t c0 = colors[i];
    uint16_t c1 = colors[i+1];

    // extract 565 channels
    uint16_t r0 = (c0 >> 11) & 0x1F;   // 0–31
    uint16_t g0 = (c0 >> 5)  & 0x3F;   // 0–63
    uint16_t b0 =  c0        & 0x1F;   // 0–31

    uint16_t r1 = (c1 >> 11) & 0x1F;
    uint16_t g1 = (c1 >> 5)  & 0x3F;
    uint16_t b1 =  c1        & 0x1F;

    // integer linear interpolation
    uint16_t r = (r0 * (1000 - t) + r1 * t) / 1000;
    uint16_t g = (g0 * (1000 - t) + g1 * t) / 1000;
    uint16_t b = (b0 * (1000 - t) + b1 * t) / 1000;

    // repack into 565
    return (r << 11) | (g << 5) | b;
}

void drawPixelFB(uint16_t x, uint16_t y, uint16_t color) {
  if (x < 0 || x >= TFT_WIDTH || y < 0 || y >= TFT_HEIGHT) return;
  framebuffer[y * TFT_WIDTH + x] = color;
}

void drawLineFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t weight, uint16_t color) {
  float x0 = x;
  float y0 = y;
  float x1 = x + w;
  float y1 = y + h;

  float dx = x1 - x0;
  float dy = y1 - y0;

  float length = sqrtf(dx*dx + dy*dy);
  if (length == 0) return;

  // unit perpendicular vector scaled to weight/2
  float ux = -dy / length;
  float uy =  dx / length;

  float half = weight * 0.5f;

  // Bresenham core
  int ix0 = x;
  int iy0 = y;
  int ix1 = x + w;
  int iy1 = y + h;

  int sx = (ix1 > ix0) ? 1 : -1;
  int sy = (iy1 > iy0) ? 1 : -1;
  int ax = abs(ix1 - ix0);
  int ay = abs(iy1 - iy0);
  int err = ax - ay;

  while (true)
  {
    int half = weight / 2;

    for (int t = -half; t <= half; t++) {
      // even-width correction
      float offset = (weight % 2 == 0) ? (t + 0.5f) : t;

      int xi = ix0 + (int)(ux * offset);
      int yi = iy0 + (int)(uy * offset);
      drawPixelFB(xi, yi, color);
    }

    if (ix0 == ix1 && iy0 == iy1) break;

    int e2 = err * 2;
    if (e2 > -ay) { err -= ay; ix0 += sx; }
    if (e2 <  ax) { err += ax; iy0 += sy; }
  }
}

void drawArcFB(uint16_t x, uint16_t y,
               uint16_t r,
               uint16_t a, uint16_t b,
               uint16_t weight,
               uint16_t color)
{
  a %= 360;
  b %= 360;

  int16_t span = b - a;
  if (span < 0) span += 360;

  bool even = (weight % 2 == 0);
  int16_t half = weight / 2;

  for (uint16_t angle = 0; angle <= span; angle++) {

    float rad = (a + angle) * 0.017453292f;

    float c = cosf(rad);
    float s = sinf(rad);

    for (int16_t t = -half; t <= half; t++) {

      // Skip extra sample for even thickness
      if (even && t == half) break;

      float offset = even ? (t + 0.5f) : t;
      float rr = r + offset;

      int px = (int)(x + c * rr);
      int py = (int)(y + s * rr);

      drawPixelFB(px, py, color);
    }
  }
}

void drawRectOutlineFB(uint16_t x, uint16_t y,
                       uint16_t w, uint16_t h,
                       uint16_t r,
                       uint16_t weight,
                       uint16_t color)
{
  // Clamp radius so it can't be larger than half the rect size
  if (r > w / 2) r = w / 2;
  if (r > h / 2) r = h / 2;

  // No rounded corners → simple rectangle outline
  if (r == 0) {
    // Top edge
    drawLineFB(x, y, w, 0, weight, color);
    // Bottom edge
    drawLineFB(x, y + h, w, 0, weight, color);
    // Left edge
    drawLineFB(x, y, 0, h, weight, color);
    // Right edge
    drawLineFB(x + w, y, 0, h, weight, color);
    return;
  }

  // DRAW ROUNDED OUTLINE (r > 0)

  // ----- STRAIGHT EDGES -----
  uint16_t offset = (weight + 1) % 2;

  // Top edge (from top-left arc end to top-right arc start)
  drawLineFB(x + r, y - offset, w - 2 * r, 0, weight, color);

  // Bottom edge
  drawLineFB(x + r, y + h, w - 2 * r, 0, weight, color);

  // Left edge
  drawLineFB(x, y + r, 0, h - 2 * r, weight, color);

  // Right edge
  drawLineFB(x + w + offset, y + r, 0, h - 2 * r, weight, color);

  // Corner Arcs
  // Top-left corner (180° → 270°)
  drawArcFB(x + r + 1, y + r + 1, r, 180, 270, weight, color);

  // Top-right corner (270° → 360°)
  drawArcFB(x + w - r, y + r + 1, r, 270, 360, weight, color);

  // Bottom-right corner (0° → 90°)
  drawArcFB(x + w - r, y + h - r, r, 0, 90, weight, color);

  // Bottom-left corner (90° → 180°)
  drawArcFB(x + r + 1, y + h - r, r, 90, 180, weight, color);
}

void drawRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  uint16_t end_x = x + w;
  uint16_t end_y = y + h;
  for (int i = x; i < end_x; i++) {
    for (int j = y; j < end_y; j++) {
      drawPixelFB(i, j, color);
    }
  }
}

void drawGradRectFB(uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint16_t r,
                    uint16_t* colors, size_t color_cnt)
{
  uint16_t end_x = x + w;
  uint16_t end_y = y + h;

  // Clamp radius so it never exceeds half dimension
  if (r > w / 2) r = w / 2;
  if (r > h / 2) r = h / 2;

  for (int i = x; i < end_x; i++) {
    for (int j = y; j < end_y; j++) {

      // --- Rounded-corner clipping ---
      bool inside = true;

      if (r > 0) {
        // Relative coordinates inside box
        int rx = i - x;
        int ry = j - y;

        // // Top-left corner test
        // if (rx < r && ry < r) {
        //   int dx = r - rx;
        //   int dy = r - ry;
        //   if (dx*dx + dy*dy > r*r) inside = false;
        // }

        // // Top-right corner test
        // else if (rx > w - r && ry < r) {
        //   int dx = rx - (w - r) + 1;
        //   int dy = r - ry;
        //   if (dx*dx + dy*dy > r*r) inside = false;
        // }

        // // Bottom-left corner test
        // else if (rx < r && ry > h - r) {
        //   int dx = r - rx;
        //   int dy = ry - (h - r) + 1;
        //   if (dx*dx + dy*dy > r*r) inside = false;
        // }

        // // Bottom-right corner test
        // else if (rx > w - r && ry > h - r) {
        //   int dx = rx - (w - r) + 1;
        //   int dy = ry - (h - r) + 1;
        //   if (dx*dx + dy*dy > r*r) inside = false;
        // }

        float fx = rx + 0.5f;
        float fy = ry + 0.5f;
        float cx = (rx < r) ? r :
                  (rx >= w - r) ? w - r :
                  -1;

        float cy = (ry < r) ? r :
                  (ry >= h - r) ? h - r :
                  -1;

        if (cx >= 0 && cy >= 0) {
          float dx = fx - cx;
          float dy = fy - cy;
          if (dx*dx + dy*dy > r*r) inside = false;
        }
      }

      if (!inside) continue;

      // --- Vertical gradient color ---
      uint16_t pixel_color =
        getColorFromGradient(colors, color_cnt, (1000.0 * (j - y)) / h);

      drawPixelFB(i, j, pixel_color);
    }
  }
}

void fillScreenFB(uint16_t color) {
  drawRectFB(0, 0, TFT_WIDTH, TFT_HEIGHT, color);
}

void pushFramebuffer(Adafruit_HX8357& tft) {
    tft.drawRGBBitmap(0, 0, framebuffer, TFT_WIDTH, TFT_HEIGHT);
}
