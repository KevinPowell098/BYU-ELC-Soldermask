#include "page_four_helpers.h"

void TempScaleSelection_SetToF() {
  sysIsTempF = true;
  sysCutoffTempF = max(min(getTempFFromC(sysCutoffTempC), CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN);
}

void TempScaleSelection_SetToC() {
  sysIsTempF = false;
  sysCutoffTempC = getTempCFromF(max(min(sysCutoffTempF, CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN));
}

void VolumeSelection_VolumePlus() {
  if (sysVolume < SYS_VOLUME_MAX) {
    sysVolume++;
  }
}

void VolumeSelection_VolumeMinus() {
  if (sysVolume > 0) {
    sysVolume--;
  }
}

void TempCutoffSelection_CutoffPlus() {
  if (sysIsTempF) {
    sysCutoffTempF += CUTOFF_TEMP_F_STEP;
  } else {
    sysCutoffTempC += CUTOFF_TEMP_C_STEP;
  }
}

void TempCutoffSelection_CutoffMinus() {
  if (sysIsTempF) {
    sysCutoffTempF -= CUTOFF_TEMP_F_STEP;
  } else {
    sysCutoffTempC -= CUTOFF_TEMP_C_STEP;
  }
}
