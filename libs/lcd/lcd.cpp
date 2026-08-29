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

Box drawLineFB(uint16_t x, uint16_t y, int16_t w, int16_t h, uint16_t weight, uint16_t color, bool roundCaps) {
  float x0 = (float)x;
  float y0 = (float)y;
  float x1 = (float)(x + w);
  float y1 = (float)(y + h);

  float dx = x1 - x0;
  float dy = y1 - y0;
  float length = sqrtf(dx*dx + dy*dy);
  if (length == 0) {
    Box b(0,0,0,0);
    return b;
  }

  // Unit vectors: parallel and perpendicular to line
  float px = dx / length;   // parallel
  float py = dy / length;
  float ux = -py;           // perpendicular
  float uy =  px;

  float half = weight * 0.5f;
  int reach = (int)ceilf(half);

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
    for (int oy = -reach; oy <= reach; oy++) {
      for (int ox = -reach; ox <= reach; ox++) {
        // Perpendicular distance: controls thickness
        float perpDist = fabsf(ox * ux + oy * uy);

        // Parallel distance from each endpoint: controls flat cap
        float paraFromStart = (ox * px + oy * py);
        float paraFromEnd   = length - paraFromStart;  // not used directly, see below

        // Offset of current Bresenham point along the line
        float spineT = ((ix0 - x) * px + (iy0 - y) * py);

        // Total parallel position of this pixel along the full line
        float totalPara = spineT + (ox * px + oy * py);

        if (perpDist <= half &&
            totalPara >= -0.5f &&
            totalPara <= length + 0.5f) {
          drawPixelFB(ix0 + ox, iy0 + oy, color);
        }
      }
    }

    if (ix0 == ix1 && iy0 == iy1) break;

    int e2 = err * 2;
    if (e2 > -ay) { err -= ay; ix0 += sx; }
    if (e2 <  ax) { err += ax; iy0 += sy; }
  }

  // if (roundCaps) {
  //   float angleDeg = atan2f(dy, dx) * 57.29577951f;
  //   uint16_t r = weight / 2;

  //   // uint16_t startA = (uint16_t)fmodf(angleDeg + 90.0f  + 360.0f, 360.0f);
  //   // uint16_t startB = (uint16_t)fmodf(angleDeg + 270.0f + 360.0f, 360.0f);
  //   // drawArcFB(x, y, r, startA, startB, 1, color);
  //   drawCircleFB(x - r, y - r, weight - 1, color);

  //   // uint16_t endA = (uint16_t)fmodf(angleDeg - 90.0f + 360.0f, 360.0f);
  //   // uint16_t endB = (uint16_t)fmodf(angleDeg + 90.0f + 360.0f, 360.0f);
  //   // drawArcFB(x + w, y + h, r, endA, endB, 1, color);
  //   drawCircleFB(x + w - r, y + h - r, weight - 1, color);
  // }

  Box b(x, y, x+w, y+h);
  return b;
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

Box drawRectOutlineFB(uint16_t x, uint16_t y,
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
    
    Box b(x, y, x+w, y+h);
    return b;
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

  Box b(x, y, x+w, y+h);
  return b;
}

Box drawRectFB(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  uint16_t end_x = x + w;
  uint16_t end_y = y + h;
  for (int i = x; i < end_x; i++) {
    for (int j = y; j < end_y; j++) {
      drawPixelFB(i, j, color);
    }
  }

  Box b(x, y, x+w, y+h);
  return b;
}

Box drawGradRectFB(uint16_t x, uint16_t y,
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

  Box b(x, y, x+w, y+h);
  return b;
}

static inline float distf(float x1, float y1, float x2, float y2) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  return sqrtf(dx*dx + dy*dy);
}

static inline float crossf(float ax, float ay, float bx, float by, float px, float py) {
  // cross of AB x AP: (B-A) x (P-A)
  return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
}

static inline bool pointInTriangle(float px, float py,
                                   float ax, float ay,
                                   float bx, float by,
                                   float cx, float cy)
{
  float s1 = crossf(ax, ay, bx, by, px, py);
  float s2 = crossf(bx, by, cx, cy, px, py);
  float s3 = crossf(cx, cy, ax, ay, px, py);

  bool has_neg = (s1 < 0.0f) || (s2 < 0.0f) || (s3 < 0.0f);
  bool has_pos = (s1 > 0.0f) || (s2 > 0.0f) || (s3 > 0.0f);

  return !(has_neg && has_pos);
}

// check if vector vP lies between vA and vB around center (taking orientation into account)
static inline bool pointInSector(float cax, float cay, float cbx, float cby, float cpx, float cpy) {
  // vA = A - C, vB = B - C, vP = P - C
  float vAx = cax;
  float vAy = cay;
  float vBx = cbx;
  float vBy = cby;
  float vPx = cpx;
  float vPy = cpy;

  // cross of vA x vB determines orientation of sector
  float crossAB = vAx * vBy - vAy * vBx;
  float crossAP = vAx * vPy - vAy * vPx;
  float crossPB = vPx * vBy - vPy * vBx;

  const float FP_EPSILON = 1e-6f;

  if (crossAB >= 0.0f) {
    return (crossAP >= -FP_EPSILON) && (crossPB >= -FP_EPSILON);
  } else {
    return (crossAP <= FP_EPSILON) && (crossPB <= FP_EPSILON);
  }
}

Box drawThreePointTriangleFB(uint16_t x1u, uint16_t y1u,
                              uint16_t x2u, uint16_t y2u,
                              uint16_t x3u, uint16_t y3u,
                              uint16_t r_in,
                              uint16_t* colors, size_t color_cnt)
{
  // float coordinates
  float ax = (float)x1u, ay = (float)y1u;
  float bx = (float)x2u, by = (float)y2u;
  float cx = (float)x3u, cy = (float)y3u;
  float r = (float)r_in;

  // bounding box
  int minX = (int)floorf(fminf(fminf(ax, bx), cx));
  int maxX = (int)ceilf (fmaxf(fmaxf(ax, bx), cx));
  int minY = (int)floorf(fminf(fminf(ay, by), cy));
  int maxY = (int)ceilf (fmaxf(fmaxf(ay, by), cy));

  int spanY = maxY - minY;
  if (spanY < 0) spanY = 0;

  // Fillet struct stores tangent points and sector center
  struct Fillet {
    float vcx, vcy;    // vertex coords
    float t1x, t1y;    // tangent point along edge to neighbor1
    float t2x, t2y;    // tangent point along edge to neighbor2
    float cx, cy;      // fillet center
    float r2;          // r*r
    bool valid;
  } fillets[3];

  auto computeFillet = [&](float vx, float vy,
                          float n1x, float n1y,
                          float n2x, float n2y,
                          Fillet &F)
  {
    // vectors from V to neighbors
    float v1x = n1x - vx, v1y = n1y - vy;
    float v2x = n2x - vx, v2y = n2y - vy;
    float len1 = sqrtf(v1x*v1x + v1y*v1y);
    float len2 = sqrtf(v2x*v2x + v2y*v2y);
    if (len1 < 1e-6f || len2 < 1e-6f) { F.valid = false; return; }

    // normalize
    v1x /= len1; v1y /= len1;
    v2x /= len2; v2y /= len2;

    // dot and handle colinear/opposite
    float dot = v1x*v2x + v1y*v2y;
    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;

    // if vectors are opposite (straight line) no fillet
    float addx = v1x + v2x;
    float addy = v1y + v2y;
    float addlen = sqrtf(addx*addx + addy*addy);
    if (addlen < 1e-6f) { F.valid = false; return; }

    // sine and cosine of half angle
    float sin_half = sqrtf(fmaxf(0.0f, (1.0f - dot) * 0.5f));
    float cos_half = sqrtf(fmaxf(0.0f, (1.0f + dot) * 0.5f));
    if (sin_half < 1e-8f || cos_half < 1e-8f) { F.valid = false; return; }

    // clamp local r so fillet fits on edges
    // tangent distance s along each edge: s = r / tan(theta/2) = r * cos_half / sin_half
    float tan_half = sin_half / cos_half;
    if (tan_half < 1e-8f) { F.valid = false; return; }

    float s = r / tan_half;
    float maxS = fminf(len1, len2) * 0.999f; // keep slight margin
    if (s > maxS) {
      // scale r down so s <= maxS
      s = maxS;
      // recompute local r from s: r_local = s * tan_half
      float r_local = s * tan_half;
      if (r_local <= 0.0f) { F.valid = false; return; }
      // use this reduced r for this fillet
      // recompute d = r_local / sin_half
      float d = r_local / sin_half;
      float bisx = addx / addlen;
      float bisy = addy / addlen;
      F.cx = vx + bisx * d;
      F.cy = vy + bisy * d;
      F.r2 = r_local * r_local;
      // tangent points:
      F.t1x = vx + v1x * s;
      F.t1y = vy + v1y * s;
      F.t2x = vx + v2x * s;
      F.t2y = vy + v2y * s;
      F.vcx = vx; F.vcy = vy;
      F.valid = true;
      return;
    }

    // normal case: s fits
    float d = r / sin_half;
    float bisx = addx / addlen;
    float bisy = addy / addlen;

    F.cx = vx + bisx * d;
    F.cy = vy + bisy * d;
    F.r2 = r * r;
    F.t1x = vx + v1x * s;
    F.t1y = vy + v1y * s;
    F.t2x = vx + v2x * s;
    F.t2y = vy + v2y * s;
    F.vcx = vx; F.vcy = vy;
    F.valid = true;
  };

  computeFillet(ax, ay, bx, by, cx, cy, fillets[0]);
  computeFillet(bx, by, cx, cy, ax, ay, fillets[1]);
  computeFillet(cx, cy, ax, ay, bx, by, fillets[2]);

  // optional clamp to screen bounds if TFT_* available
#ifdef TFT_WIDTH
  if (minX < 0) minX = 0;
  if (minY < 0) minY = 0;
  if (maxX > (int)TFT_WIDTH - 1) maxX = (int)TFT_WIDTH - 1;
  if (maxY > (int)TFT_HEIGHT - 1) maxY = (int)TFT_HEIGHT - 1;
#endif

  for (int py = minY; py <= maxY; ++py) {
    for (int px = minX; px <= maxX; ++px) {

      float fx = (float)px + 0.5f;
      float fy = (float)py + 0.5f;

      // inside triangle?
      if (!pointInTriangle(fx, fy, ax, ay, bx, by, cx, cy)) continue;

      bool draw = true;

      // For each fillet: if pixel lies inside the corner triangle (V, T1, T2),
      // then only draw it if it belongs to the circular sector (centered at fillet.cx,fillet.cy).
      for (int i = 0; i < 3; ++i) {
        if (!fillets[i].valid) continue;

        // corner triangle test
        if (pointInTriangle(fx, fy,
                            fillets[i].vcx, fillets[i].vcy,
                            fillets[i].t1x, fillets[i].t1y,
                            fillets[i].t2x, fillets[i].t2y))
        {
          // inside the small corner triangle: keep only if inside sector + inside circle
          float dx = fx - fillets[i].cx;
          float dy = fy - fillets[i].cy;
          float d2 = dx*dx + dy*dy;
          if (d2 > fillets[i].r2) {
            draw = false; // outside the fillet circle => removed (sharp corner cut away)
            break;
          } else {
            // inside the circle, but must also be inside the sector between t1 and t2
            // compute vectors relative to center
            float vAx = fillets[i].t1x - fillets[i].cx;
            float vAy = fillets[i].t1y - fillets[i].cy;
            float vBx = fillets[i].t2x - fillets[i].cx;
            float vBy = fillets[i].t2y - fillets[i].cy;
            float vPx = fx - fillets[i].cx;
            float vPy = fy - fillets[i].cy;

            if (!pointInSector(vAx, vAy, vBx, vBy, vPx, vPy)) {
              // point inside circle but not in the sector -> exclude
              draw = false;
              break;
            } else {
              // point is inside sector -> keep
              draw = true;
              break; // no need to check other fillets
            }
          }
        }
      } // end fillet loop

      if (!draw) continue;

      // gradient place 0..1000 using py relative to minY..maxY
      uint16_t place = 0;
      if (spanY > 0) {
        float rel = ((float)py - (float)minY) / (float)spanY;
        if (rel < 0.0f) rel = 0.0f;
        if (rel > 1.0f) rel = 1.0f;
        place = (uint16_t)(rel * 1000.0f + 0.5f);
        if (place > 1000) place = 1000;
      } else {
        place = 0;
      }

      uint16_t pixel_color = getColorFromGradient(colors, color_cnt, place);
      drawPixelFB((uint16_t)px, (uint16_t)py, pixel_color);
    }
  }

  int16_t x1 = min(min(x1u, x2u), x3u);
  int16_t y1 = min(min(y1u, y2u), y3u);
  int16_t x2 = max(max(x1u, x2u), x3u);
  int16_t y2 = max(max(y1u, y2u), y3u);
  
  Box b(x1, y1, x2, y2);
  return b;
}

void fillScreenFB(uint16_t color) {
  drawRectFB(0, 0, TFT_WIDTH, TFT_HEIGHT, color);
}

void pushFramebuffer(Adafruit_HX8357& tft) {
    tft.drawRGBBitmap(0, 0, framebuffer, TFT_WIDTH, TFT_HEIGHT);
}

Box expandBox(Box b, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  x1 += b._get_x1();
  y1 += b._get_y1();
  x2 += b._get_x2();
  y2 += b._get_y2();

  Box box(x1, y1, x2, y2);
  return box;
}

void drawCircleFB(uint16_t x, uint16_t y, uint16_t d, uint16_t color) {

  const float threshold = (d > 6) ? 0.5 : 0.8;
  uint16_t x_o = 0;

  if (d % 2) {
    x_o = 1;
    for (int i = y; i < y + d; i++) {
      drawPixelFB(x + (d/2), i, color);
    }

    for (int i = x; i < x + d; i++) {
      drawPixelFB(i, y + (d/2), color);
    }
  }
  
  for (int16_t x_c = 0; x_c < d/2; x_c++) {
    float y_c = sqrt((d/2) * (d/2) - x_c * x_c);

    if (y_c - floor(y_c) < threshold) {
      y_c = floor(y_c);
    } else {
      y_c = ceil(y_c);
    }

    for (int16_t i = 0; i < y_c; i++) {
      drawPixelFB(x + d/2 + x_o + x_c, y - y_c + d/2 + i, color);
      drawPixelFB(x + d/2 - x_o + x_o - 1 - x_c, y - y_c + d/2 + i, color);

      drawPixelFB(x + d/2 + x_o + x_c, y + y_c + d/2 + x_o - 1 - i, color);
      drawPixelFB(x + d/2 - x_o + x_o - 1 - x_c, y + y_c + d/2 + x_o - 1 - i, color);
    }
  }
}