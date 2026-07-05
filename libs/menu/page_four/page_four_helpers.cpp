#include "page_four_helpers.h"

void setCutoffTempF() {
  sysCutoffTempF = max(min(getTempFFromC(sysCutoffTempC), CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN);
  sysCutoffTempF = roundToNearest(sysCutoffTempF, CUTOFF_TEMP_F_STEP);
}

void setCutoffTempC() {
  sysCutoffTempC = getTempCFromF(max(min(sysCutoffTempF, CUTOFF_TEMP_F_MAX), CUTOFF_TEMP_F_MIN));
  sysCutoffTempC = roundToNearest(sysCutoffTempC, CUTOFF_TEMP_C_STEP);
}

void TempScaleSelection_SetToF() {
  if (sysCutoffTempC <= CUTOFF_TEMP_C_MIN) {
    sysCutoffTempF = CUTOFF_TEMP_F_MIN;
  } else if (sysCutoffTempC >= CUTOFF_TEMP_C_MAX) {
    sysCutoffTempF = CUTOFF_TEMP_F_MAX;
  } else {
    setCutoffTempF();
  }

  sysIsTempF = true;
}

void TempScaleSelection_SetToC() {
  if (sysCutoffTempF <= CUTOFF_TEMP_F_MIN) {
    sysCutoffTempC = CUTOFF_TEMP_C_MIN;
  } else if (sysCutoffTempF >= CUTOFF_TEMP_F_MAX) {
    sysCutoffTempC = CUTOFF_TEMP_C_MAX;
  } else {
    setCutoffTempC();
  }

  sysIsTempF = false;
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
    setCutoffTempF();
  }

  clampTemp();
}

void TempCutoffSelection_CutoffMinus() {
  if (sysIsTempF) {
    sysCutoffTempF -= CUTOFF_TEMP_F_STEP;
  } else {
    sysCutoffTempC -= CUTOFF_TEMP_C_STEP;
    setCutoffTempF();
  }

  clampTemp();
}

void clampTemp() {
  Serial.println("temp clamped");
  
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