#include "page_four_helpers.h"


void setup_SetCutoffTempF() {
  sysCutoffTempF = max(min(getTempFFromC(sysCutoffTempC), CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN);
  sysCutoffTempF = roundToNearest(sysCutoffTempF, CUTOFF_TEMP_F_STEP);
}

void setup_SetCutoffTempC() {
  sysCutoffTempC = getTempCFromF(max(min(sysCutoffTempF, CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN));
  sysCutoffTempC = roundToNearest(sysCutoffTempC, CUTOFF_TEMP_C_STEP);
}

void setup_TempScaleSelection_SetToF() {
  if (sysCutoffTempC <= CUTOFF_TEMP_C_MIN) {
    sysCutoffTempF = CUTOFF_TEMP_F_MIN;
  } else if (sysCutoffTempC >= CUTOFF_TEMP_C_MAX) {
    sysCutoffTempF = CUTOFF_TEMP_F_MAX;
  } else {
    setup_SetCutoffTempF();
  }

  sysIsTempF = true;
}

void setup_TempScaleSelection_SetToC() {
  if (sysCutoffTempF <= CUTOFF_TEMP_F_MIN) {
    sysCutoffTempC = CUTOFF_TEMP_C_MIN;
  } else if (sysCutoffTempF >= CUTOFF_TEMP_F_MAX) {
    sysCutoffTempC = CUTOFF_TEMP_C_MAX;
  } else {
    setup_SetCutoffTempC();
  }

  sysIsTempF = false;
}

void setup_VolumeSelection_Plus() {
  if (sysVolume < SYS_VOLUME_MAX) {
    sysVolume++;
  }
}

void setup_VolumeSelection_Minus() {
  if (sysVolume > 0) {
    sysVolume--;
  }
}

void setup_TempCutoffSelection_Plus() {
  if (sysIsTempF) {
    sysCutoffTempF += CUTOFF_TEMP_F_STEP;
  } else {
    sysCutoffTempC += CUTOFF_TEMP_C_STEP;
    setup_SetCutoffTempF();
  }

  setup_ClampCutoffTemp();
}

void setup_TempCutoffSelection_Minus() {
  if (sysIsTempF) {
    sysCutoffTempF -= CUTOFF_TEMP_F_STEP;
  } else {
    sysCutoffTempC -= CUTOFF_TEMP_C_STEP;
    setup_SetCutoffTempF();
  }

  setup_ClampCutoffTemp();
}

void setup_ClampCutoffTemp() {
  if (sysIsTempF) {
    if (sysCutoffTempF > CUTOFF_TEMP_F_MAX) {
      sysCutoffTempF = CUTOFF_TEMP_F_MAX;
    }
    if (sysCutoffTempF < CUTOFF_TEMP_F_MIN) {
      sysCutoffTempF = CUTOFF_TEMP_F_MIN;
    }
  } else {
    if (sysCutoffTempC > CUTOFF_TEMP_C_MAX) {
      sysCutoffTempC = CUTOFF_TEMP_C_MAX;
    }
    if (sysCutoffTempC < CUTOFF_TEMP_C_MIN) {
      sysCutoffTempC = CUTOFF_TEMP_C_MIN;
    }
  }
}