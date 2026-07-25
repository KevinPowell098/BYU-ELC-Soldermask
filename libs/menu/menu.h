#pragma once

// Functions to change tabs
bool isProcessActive();
void tabOnePressed();
void tabTwoPressed();
void tabThreePressed();
void tabFourPressed();

// Create touch sensitive regions
void initTabBoxes();

// Functions to draw menu tabs
void drawMenu();
void drawMenu_Shadow();
void drawMenu_GradientFill();
void drawMenu_Outline();
void drawMenu_TabLabels();
void drawMenu_Title();
