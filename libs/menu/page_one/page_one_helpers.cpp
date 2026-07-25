#include "../vars.h"
#include "helpers/helpers.h"
#include "page_one_run.h"
#include "page_one_helpers.h"


void bake_SetBakeTempF() {
  sysBakeTempF = max(min(getTempFFromC(sysBakeTempC),BAKE_TEMP_F_MAX), BAKE_TEMP_F_MIN);
  sysBakeTempF = roundToNearest(sysBakeTempF, BAKE_TEMP_F_STEP);
}

void bake_SetBakeTempC() {
  sysBakeTempC = getTempCFromF(max(min(sysBakeTempF, BAKE_TEMP_F_MAX), BAKE_TEMP_F_MIN));
  sysBakeTempC = roundToNearest(sysBakeTempC, BAKE_TEMP_C_STEP);
}

void bake_TempSetting_Plus() {
  if (sysIsTempF) {
    sysBakeTempF += BAKE_TEMP_F_STEP;
  } else {
    sysBakeTempC += BAKE_TEMP_C_STEP;
    bake_SetBakeTempF();
  }

  bake_clampBakeTemp();
}

void bake_TempSetting_Minus() {
  if (sysIsTempF) {
    sysBakeTempF -= BAKE_TEMP_F_STEP;
  } else {
    sysBakeTempC -= BAKE_TEMP_C_STEP;
    bake_SetBakeTempF();
  }

  bake_clampBakeTemp();
}

void bake_TimeSetting_Plus() {
  if (sysBakeTimeS < BAKE_TIME_S_MAX) {
    sysBakeTimeS += BAKE_TIME_S_STEP;
  }
}

void bake_TimeSetting_Minus() {
  if (sysBakeTimeS > BAKE_TIME_S_MIN) {
    sysBakeTimeS -= BAKE_TIME_S_STEP;
  }
}

void bake_StartButton_Press() {
  activePage = PAGE_ONE_PROCESS;
}

void bake_ProcessCancel_Press() {
  activePage = PAGE_ONE;
}

void bake_clampBakeTemp() {
  if (sysIsTempF) {
    if (sysBakeTempF > BAKE_TEMP_F_MAX) {
      sysBakeTempF = BAKE_TEMP_F_MAX;
    }
    if (sysBakeTempF < BAKE_TEMP_F_MIN) {
      sysBakeTempF = BAKE_TEMP_F_MIN;
    }
  } else {
    if (sysBakeTempC > BAKE_TEMP_C_MAX) {
      sysBakeTempC = BAKE_TEMP_C_MAX;
    }
    if (sysBakeTempC < BAKE_TEMP_C_MIN) {
      sysBakeTempC = BAKE_TEMP_C_MIN;
    }
  }
}