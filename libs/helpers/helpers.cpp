#include "helpers/helpers.h"
#include "menu/vars.h"
#include "menu/menu.h"

int16_t getTempFFromC(int16_t TempC) {
  return ((TempC * 9.0) / 5.0) + 32;
}

int16_t getTempCFromF(int16_t TempF) {
  return ((TempF - 32) * 5.0) / 9.0;
}

int16_t roundToNearest(int16_t val, uint16_t step) {
  int16_t r = val % step;
  if (r < step / 2) {
    val -= r;
  } else {
    val += step - r;
  }
  return val;
}

int16_t clampInt(int16_t val, int16_t min, int16_t max) {
  if (val > max) val = max;
  if (val < min) val = min;
  return val;
}

int16_t min(int16_t n, int16_t m) {
  if (n < m) {
    return n;
  } else {
    return m;
  }
}

int16_t max(int16_t n, int16_t m) {
  if (n > m) {
    return n;
  } else {
    return m;
  }
}

// TODO: delete this. Here for debugging
// void drawHeatProcess() {

// }

void initMenu() {
  initTabBoxes();

  // Init each page
  drawHeat();   // 1
  drawUV();     // 2
  // 3
  drawSetup();  // 4

  // Init each process page
  drawHeatProcess();  // 1
  drawUVProcess();    // 2

  // Ensure touch elements are not generated again
  isInit = false;
}
