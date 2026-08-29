#pragma once

#include <utility>
#include <cstdint>
#include <vector>

// vector of coordinates for graph curve
extern std::vector<std::pair<uint16_t, uint16_t>> points;

// Page setup
void drawHeatProcess();

void drawHeatProcess_BG();
void drawHeatProcess_CurrentTemp();
std::pair<uint16_t, uint16_t> drawHeatProcess_TempGraph();
void drawHeatProcess_Curve();
void drawHeatProcess_ProgressBar();
void drawHeatProcess_Cancel();

// Runtime
void updateIsBakeComplete();
void drawHeatProcess_addPoint();
void drawHeatProcess_bakeComplete();