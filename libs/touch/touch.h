#include "lcd/lcd.h"

#define MAX_BOXES 100

Box boxes[MAX_BOXES];
uint16_t boxCount = 0;

void AddBox(Box b);
void AddBox(Box b, void (*func)(), int16_t page);
