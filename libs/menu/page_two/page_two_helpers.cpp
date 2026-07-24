#include "page_two_helpers.h"


void UV_PowerSetting_Plus() {
  if (sysCurePower < CURE_POWER_MAX) {
    sysCurePower += CURE_POWER_STEP;
  }
}

void UV_PowerSetting_Minus() {
  if (sysCurePower > CURE_POWER_MIN) {
    sysCurePower -= CURE_POWER_STEP;
  }
}

void UV_TimeSetting_Plus() {
  if (sysCureTimeS < CURE_TIME_S_MAX) {
    sysCureTimeS += CURE_TIME_S_STEP;
  }
}

void UV_TimeSetting_Minus() {
  if (sysCureTimeS > CURE_TIME_S_MIN) {
    sysCureTimeS -= CURE_TIME_S_STEP;
  }
}

void UV_StartButton_Press() {
  activePage = PAGE_TWO_PROCESS;
}

void UV_ProcessCancel_Press() {
  activePage = PAGE_TWO;
}
