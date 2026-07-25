#pragma once

#include <cstdint>

int16_t getTempFFromC(int16_t TempC);
int16_t getTempCFromF(int16_t TempF);

int16_t roundToNearest(int16_t val, uint16_t step);
int16_t clampInt(int16_t val, int16_t min, int16_t max);

int16_t min(int16_t n, int16_t m);
int16_t max(int16_t n, int16_t m);

void initMenu();

void updateTemp();